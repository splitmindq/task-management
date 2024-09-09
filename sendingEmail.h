//
// Created by mzitr on 08.09.2024.
//

#ifndef LABS_SENDINGEMAIL_H
#define LABS_SENDINGEMAIL_H
#include <iostream>
#include <vector>
#include <string>
#include "curl/curl.h"

class User {
public:
    int id;
    std::string* email;

    User(int id, const std::string& email);
    ~User();
};


class UserManager {
private:
    std::vector<User*> users;
    int next_id;

    void delete_all_users();

public:
    UserManager();
    ~UserManager();

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
        const char **payload_text;
    };

    static size_t payload_source(void *ptr, size_t size, size_t nmemb, void *userp);

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
