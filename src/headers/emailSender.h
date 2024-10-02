//
// Created by mzitr on 16.09.2024.
//

#ifndef LABS_EMAIL_SENDER_H
#define LABS_EMAIL_SENDER_H

#include <string>
#include <array>
#include <curl/curl.h>

class EmailSender {
private:
    std::string from;
    std::string smtpUrl;
    std::string username;
    std::string password;
    std::string secondPassword;
    CURL *curl;

    struct UploadStatus {
        int linesRead;
        std::array<const char*, 5> payloadText;
    };

    static size_t payloadSource(char *ptr, size_t size, size_t nmemb, UploadStatus *userp);

public:
    EmailSender(const std::string &from, const std::string &smtpUrl,
                const std::string &username, const std::string &password,
                const std::string &secondPassword);
    ~EmailSender();
    void sendEmail(const std::string &to, const std::string &subject, const std::string &message);
};

#endif // LABS_EMAIL_SENDER_H