//
// Created by mzitr on 08.09.2024.
//

#ifndef LABS_SENDINGEMAIL_H
#define LABS_SENDINGEMAIL_H
#include <iostream>
#include <vector>
#include <string>
#include "curl/curl.h"
#include <memory>
#include <array>
class User {
public:
    int id;
    std::unique_ptr<std::string>email;

    User(int id, const std::string& email);
    ~User() = default;
};


class UserManager {
private:
    std::vector<std::unique_ptr<User>> users;
    int next_id;

    void delete_all_users();

public:
    UserManager();
    ~UserManager() = default;

    void create_user();
    void read_users() const;
    void update_user();
    void delete_user();
    User* find_user_by_id(int id);
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
        std::array<const char*, 5> payload_text;
    };

    static size_t payload_source(char *ptr, size_t size, size_t nmemb, upload_status *userp);

public:
    EmailSender(const std::string &from, const std::string &smtp_url,
                const std::string &username, const std::string &password,
                const std::string &second_password);
    ~EmailSender();
    void send_email(const std::string &to, const std::string &subject, const std::string &message);
};

//-------------------------

void display_menu();
void handle_menu_choice(int choice, UserManager& userManager, EmailSender& emailSender);




#endif //LABS_SENDINGEMAIL_H
