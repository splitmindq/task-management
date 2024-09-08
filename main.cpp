#include <iostream>
#include <vector>
#include <string>
#include "curl/curl.h"

class User {
public:
    int id;
    std::string* email;

    User(int id, const std::string& email) : id(id) {
        this->email = new std::string(email);
    }

    ~User() {
        delete email;
    }
};

class UserManager {
private:
    std::vector<User*> users;
    int next_id;

public:
    UserManager() : next_id(1) {}

    ~UserManager() {
        for (auto user : users) {
            delete user;
        }
    }

    void create_user() {
        std::string email_input;
        std::cout << "Enter user email: ";
        std::cin >> email_input;

        User* new_user = new User(next_id++, email_input);
        users.push_back(new_user);
        std::cout << "User created with ID: " << new_user->id << std::endl;
    }

    void read_users() const {
        if (users.empty()) {
            std::cout << "No users found." << std::endl;
            return;
        }

        std::cout << "List of users:\n";
        for (const auto& user : users) {
            std::cout << "ID: " << user->id << ", Email: " << *(user->email) << std::endl;
        }
    }

    void update_user() {
        int id;
        std::cout << "Enter user ID to update: ";
        std::cin >> id;

        for (auto& user : users) {
            if (user->id == id) {
                std::string new_email;
                std::cout << "Enter new email: ";
                std::cin >> new_email;
                delete user->email;
                user->email = new std::string(new_email);
                std::cout << "User with ID " << id << " updated." << std::endl;
                return;
            }
        }
        std::cout << "User with ID " << id << " not found." << std::endl;
    }

    void delete_user() {
        int id;
        std::cout << "Enter user ID to delete: ";
        std::cin >> id;

        for (auto it = users.begin(); it != users.end(); ++it) {
            if ((*it)->id == id) {
                std::cout << "User with ID " << id << " and email " << *((*it)->email) << " has been deleted." << std::endl;
                delete *it;
                users.erase(it);
                return;
            }
        }
        std::cout << "User with ID " << id << " not found." << std::endl;
    }

    User* find_user_by_id(int id) {
        for (auto& user : users) {
            if (user->id == id) {
                return user;
            }
        }
        return nullptr;
    }
};

class EmailSender {
private:
    std::string from;
    std::string smtp_url;
    std::string username;
    std::string password;
    std::string second_password;
    CURL *curl;

    struct upload_status {
        int lines_read;
        const char **payload_text;
    };

    static size_t payload_source(void *ptr, size_t size, size_t nmemb, void *userp) {
        upload_status *upload_ctx = static_cast<upload_status *>(userp);
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

public:
    EmailSender(const std::string &from, const std::string &smtp_url,
                const std::string &username, const std::string &password,
                const std::string &second_password)
            : from(from), smtp_url(smtp_url), username(username),
              password(password), second_password(second_password), curl(curl_easy_init()) {}

    ~EmailSender() {
        if (curl) {
            curl_easy_cleanup(curl);
        }
    }

    void send_email(const std::string &to, const std::string &subject, const std::string &message) {
        if (!curl) {
            std::cerr << "Failed to initialize CURL" << std::endl;
            return;
        }

        struct curl_slist *recipients = nullptr;
        upload_status upload_ctx = {0};

        const char *payload_text[5];
        std::string to_header = "To: " + to + "\r\n";
        std::string subject_header = "Subject: " + subject + "\r\n";
        std::string message_body = message + "\r\n";

        payload_text[0] = to_header.c_str();
        payload_text[1] = ("From: " + from + "\r\n").c_str();
        payload_text[2] = subject_header.c_str();
        payload_text[3] = "\r\n";
        payload_text[4] = message_body.c_str();

        upload_ctx.payload_text = payload_text;

        curl_easy_setopt(curl, CURLOPT_USERNAME, username.c_str());
        curl_easy_setopt(curl, CURLOPT_PASSWORD, password.c_str());
        curl_easy_setopt(curl, CURLOPT_URL, smtp_url.c_str());
        curl_easy_setopt(curl, CURLOPT_USE_SSL, CURLUSESSL_ALL);
        curl_easy_setopt(curl, CURLOPT_MAIL_FROM, from.c_str());

        recipients = curl_slist_append(recipients, to.c_str());
        curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, recipients);
        curl_easy_setopt(curl, CURLOPT_READFUNCTION, payload_source);
        curl_easy_setopt(curl, CURLOPT_READDATA, &upload_ctx);
        curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);

        CURLcode res = curl_easy_perform(curl);

        if (res != CURLE_OK) {
            std::cerr << "First try failed: " << curl_easy_strerror(res) << std::endl;
            curl_easy_setopt(curl, CURLOPT_PASSWORD, second_password.c_str());
            std::cout << "Retrying with second password..." << std::endl;

            res = curl_easy_perform(curl);

            if (res != CURLE_OK) {
                std::cerr << "Second try failed: " << curl_easy_strerror(res) << std::endl;
            }
        }

        curl_slist_free_all(recipients);
    }
};

int main() {
    UserManager userManager;
    EmailSender emailSender("<emailsender11@mail.ru>", "smtp://smtp.mail.ru:587",
                            "emailsender11@mail.ru", "XPm2jZ2YS0ECx5q5PKzW", "Kge6zBXU5SFsMnNsSkfk");
    int choice;

    while (true) {
        std::cout << "\nMenu:\n";
        std::cout << "1. Create user\n";
        std::cout << "2. Read users\n";
        std::cout << "3. Update user\n";
        std::cout << "4. Delete user\n";
        std::cout << "5. Send email to user\n";
        std::cout << "6. Exit\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;

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
                return 0;
            default:
                std::cout << "Invalid choice, please try again." << std::endl;
        }
    }

    return 0;
}
