#include "headers/sendingEmail.h"

User::User(int id, const std::string& email) : id(id), email(std::make_unique<std::string>(email)) { }

void UserManager::deleteAllUsers() {
    users.clear();
}

UserManager::UserManager() = default;

void UserManager::createUser() {
    std::string emailInput;
    std::cout << "Введите вашу почту: ";
    std::cin >> emailInput;
    auto newUser = std::make_unique<User>(nextId, emailInput);
    ++nextId;
    users.push_back(std::move(newUser));
    std::cout << "Создан пользователь с ID: " << users.back()->id << std::endl;
}

void UserManager::readUsers() const {
    if (users.empty()) {
        std::cout << "Нету такого студента" << std::endl;
        return;
    }

    std::cout << "List of users:\n";
    for (const auto& user : users) {
        std::cout << "ID: " << user->id << ", Email: " << *(user->email) << std::endl;
    }
}

void UserManager::updateUser() {
    int id;
    std::cout << "Введите ID пользователя ";
    std::cin >> id;

    User* user = findUserById(id);
    if (user) {
        std::string newEmail;
        std::cout << "Enter new email: ";
        std::cin >> newEmail;

        user->email = std::make_unique<std::string>(newEmail);
        std::cout << "Пользователь с ID: " << id << " обновлен." << std::endl;
    } else {
        std::cout << "Пользователь с ID: " << id << " не найден." << std::endl;
    }
}

void UserManager::deleteUser() {
    int id;
    std::cout << "Введите ID пользователя для удаления: ";
    std::cin >> id;

    for (auto it = users.begin(); it != users.end(); ++it) {
        if ((*it)->id == id) {
            std::cout << "Пользователь с ID " << id << " с почтой " << *((*it)->email) << " удалён." << std::endl;
            users.erase(it);
            return;
        }
    }
    std::cout << "Пользователь с ID " << id << " не найден." << std::endl;
}

User* UserManager::findUserById(int id) {
    for (const auto& user : users) {
        if (user->id == id) {
            return user.get();
        }
    }
    return nullptr;
}

size_t EmailSender::payloadSource(char *ptr, size_t size, size_t nmemb, UploadStatus *uploadCtx) {
    const char *data;

    if ((size == 0) || (nmemb == 0) || ((size * nmemb) < 1)) {
        return 0;
    }

    data = uploadCtx->payloadText[uploadCtx->linesRead];

    if (data != nullptr) {
        size_t len = strlen(data);
        if (len > 0) {
            memcpy(ptr, data, len);
            uploadCtx->linesRead++;
            return len;
        }
    }
    return 0;
}

EmailSender::EmailSender(const std::string &from, const std::string &smtpUrl,
                         const std::string &username, const std::string &password,
                         const std::string &secondPassword)
        : from(from), smtpUrl(smtpUrl), username(username),
          password(password), secondPassword(secondPassword), curl(curl_easy_init()) {}

EmailSender::~EmailSender() {
    if (curl) {
        curl_easy_cleanup(curl);
    }
}

void EmailSender::sendEmail(const std::string &to, const std::string &subject, const std::string &message) {
    UploadStatus uploadCtx = {0};

    std::string toHeader = "To: " + to + "\r\n";
    std::string fromHeader = "From: " + from + "\r\n";
    std::string subjectHeader = "Subject: " + subject + "\r\n";
    std::string messageBody = message + "\r\n";

    uploadCtx.payloadText = {toHeader.c_str(), fromHeader.c_str(), subjectHeader.c_str(), "\r\n", messageBody.c_str()};

    curl_easy_setopt(curl, CURLOPT_USERNAME, username.c_str());
    curl_easy_setopt(curl, CURLOPT_PASSWORD, password.c_str());
    curl_easy_setopt(curl, CURLOPT_URL, smtpUrl.c_str());
    curl_easy_setopt(curl, CURLOPT_USE_SSL, CURLUSESSL_ALL);
    curl_easy_setopt(curl, CURLOPT_MAIL_FROM, from.c_str());

    struct curl_slist *recipients = nullptr;
    recipients = curl_slist_append(recipients, to.c_str());
    curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, recipients);
    curl_easy_setopt(curl, CURLOPT_READFUNCTION, payloadSource);
    curl_easy_setopt(curl, CURLOPT_READDATA, &uploadCtx);
    curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        std::cerr << "\nFirst try failed: " << curl_easy_strerror(res) << std::endl;
        curl_easy_setopt(curl, CURLOPT_PASSWORD, secondPassword.c_str());
        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            std::cerr << "\nSecond try failed: " << curl_easy_strerror(res) << std::endl;
        }
    }
    Sleep(1000);

    curl_slist_free_all(recipients);
}

void displayMenu() {
    std::cout << "\nMenu:\n";
    std::cout << "1. Создать пользователя\n";
    std::cout << "2. Считать пользователей\n";
    std::cout << "3. Обновить пользователей\n";
    std::cout << "4. Удалить пользователя\n";
    std::cout << "5. Отправить письмо на почту\n";
    std::cout << "6. Выход\n";
}

void handleMenuChoice(int choice, UserManager& userManager, EmailSender& emailSender) {
    switch (choice) {
        setlocale(LC_ALL,"RUS");
        case 1:
            userManager.createUser();
            break;
        case 2:
            userManager.readUsers();
            break;
        case 3:
            userManager.updateUser();
            break;
        case 4:
            userManager.deleteUser();
            break;
        case 5: {
            int userId;
            std::cout << "Введите ID пользователя: ";
            std::cin >> userId;
            if (const User* user = userManager.findUserById(userId)) {
                emailSender.sendEmail(*(user->email), "Тема: лабораторная 1", "Я обожаю sonar!!");
                std::cout << "Письмо отправлено: " << *(user->email) << std::endl;
            } else {
                std::cout << "Пользователь не найден." << std::endl;
            }
            break;
        }
        case 6:
            exit(0);
        default:
            std::cout << "Неверный выбор." << std::endl;
    }
}