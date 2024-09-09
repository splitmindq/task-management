#include "sendingEmail.h"

User::User(int id, const std::string& email) : id(id), email(std::make_unique<std::string>(email)) {

}


void UserManager::delete_all_users() {
    users.clear();
}

UserManager::UserManager() : next_id(0) {}



void UserManager::create_user() {
    std::string email_input;
    std::cout << "Enter user email: ";
    std::cin >> email_input;

    // Создайте пользователя и увеличьте next_id только один раз
    users.push_back(std::make_unique<User>(next_id++, email_input));
    std::cout << "User created with ID: " << next_id - 1 << std::endl;
}

void UserManager::read_users() const {
    if (users.empty()) {
        std::cout << "No users found." << std::endl;
        return;
    }

    std::cout << "List of users:\n";
    for (const auto& user : users) {
        std::cout << "ID: " << user->id << ", Email: " << *(user->email) << std::endl;
    }
}

void UserManager::update_user() {
    int id;
    std::cout << "Enter user ID to update: ";
    std::cin >> id;

    User* user = find_user_by_id(id);
    if (user) {
        std::string new_email;
        std::cout << "Enter new email: ";
        std::cin >> new_email;

        user->email = std::make_unique<std::string>(new_email);
        std::cout << "User with ID " << id << " updated." << std::endl;
    } else {
        std::cout << "User with ID " << id << " not found." << std::endl;
    }
}

void UserManager::delete_user() {
    int id;
    std::cout << "Enter user ID to delete: ";
    std::cin >> id;

    for (auto it = users.begin(); it != users.end(); ++it) {
        if ((*it)->id == id) {
            std::cout << "User with ID " << id << " and email " << *((*it)->email) << " has been deleted." << std::endl;
            users.erase(it);
            return;
        }
    }
    std::cout << "User with ID " << id << " not found." << std::endl;
}

User* UserManager::find_user_by_id(int id) {
    for (auto& user : users) {
        if (user->id == id) {
            return user.get();
        }
    }
    return nullptr;
}

size_t EmailSender::payload_source(char *ptr, size_t size, size_t nmemb, upload_status *upload_ctx) {
    const char *data;

    if ((size == 0) || (nmemb == 0) || ((size * nmemb) < 1)) {
        return 0;
    }

    data = upload_ctx->payload_text[upload_ctx->lines_read];

    if (data) {
        size_t len = strlen(data);
        memcpy(ptr, data, len);
        upload_ctx->lines_read++;
        return len;
    }

    return 0;
}

EmailSender::EmailSender(const std::string &from, const std::string &smtp_url,
                         const std::string &username, const std::string &password,
                         const std::string &second_password)
        : from(from), smtp_url(smtp_url), username(username),
          password(password), second_password(second_password), curl(curl_easy_init()) {}

EmailSender::~EmailSender() {
    if (curl) {
        curl_easy_cleanup(curl);
    }
}

void EmailSender::send_email(const std::string &to, const std::string &subject, const std::string &message) {
    upload_status upload_ctx = {0};

    // Формирование заголовков и тела сообщения
    std::string to_header = "To: " + to + "\r\n";
    std::string from_header = "From: " + from + "\r\n";
    std::string subject_header = "Subject: " + subject + "\r\n";
    std::string message_body = message + "\r\n";

    // Инициализация std::array через список инициализации
    upload_ctx.payload_text = {to_header.c_str(), from_header.c_str(), subject_header.c_str(), "\r\n", message_body.c_str()};

    // Настройка CURL
    curl_easy_setopt(curl, CURLOPT_USERNAME, username.c_str());
    curl_easy_setopt(curl, CURLOPT_PASSWORD, password.c_str());
    curl_easy_setopt(curl, CURLOPT_URL, smtp_url.c_str());
    curl_easy_setopt(curl, CURLOPT_USE_SSL, CURLUSESSL_ALL);
    curl_easy_setopt(curl, CURLOPT_MAIL_FROM, from.c_str());

    // Отправка письма
    struct curl_slist *recipients = nullptr;
    recipients = curl_slist_append(recipients, to.c_str());
    curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, recipients);
    curl_easy_setopt(curl, CURLOPT_READFUNCTION, payload_source);
    curl_easy_setopt(curl, CURLOPT_READDATA, &upload_ctx);
    curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        std::cerr << "First try failed: " << curl_easy_strerror(res) << std::endl;
        curl_easy_setopt(curl, CURLOPT_PASSWORD, second_password.c_str());
        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            std::cerr << "Second try failed: " << curl_easy_strerror(res) << std::endl;
        }
    }

    curl_slist_free_all(recipients);
}


void display_menu() {
    std::cout << "\nMenu:\n";
    std::cout << "1. Create user\n";
    std::cout << "2. Read users\n";
    std::cout << "3. Update user\n";
    std::cout << "4. Delete user\n";
    std::cout << "5. Send email to user\n";
    std::cout << "6. Exit\n";
}

void handle_menu_choice(int choice, UserManager& userManager, EmailSender& emailSender) {
    switch (choice) {
        case 1:
            userManager.create_user();
            break;
        case 2:
            userManager.read_users();
            break;
        case 3:
            userManager.update_user();
            break;
        case 4:
            userManager.delete_user();
            break;
        case 5: {
            int user_id;
            std::cout << "Enter user ID to send email: ";
            std::cin >> user_id;

            User* user = userManager.find_user_by_id(user_id);
            if (user != nullptr) {
                emailSender.send_email(*(user->email), "Subject: Test Email", "This is a test email.");
                std::cout << "Email sent to " << *(user->email) << std::endl;
            } else {
                std::cout << "User not found." << std::endl;
            }
            break;
        }
        case 6:
            exit(0);
        default:
            std::cout << "Invalid choice, please try again." << std::endl;
    }
}