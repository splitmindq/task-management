//
// Created by mzitr on 16.09.2024.
//

#ifndef LABS_EMAIL_SENDER_H
#define LABS_EMAIL_SENDER_H

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




#endif //LABS_EMAIL_SENDER_H
