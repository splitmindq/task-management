#include "sendingEmail.h"

size_t safe_strlen(const char *str) {
    return str ? strlen(str) : 0;
}

size_t EmailSender::payloadSource(char *ptr, size_t size, size_t nmemb, UploadStatus *userp) {
    const char *data;
    if (userp->linesRead >= userp->payloadText.size()) {
        return 0;
    }
    data = userp->payloadText[userp->linesRead];
    if (data) {
        size_t len = safe_strlen(data);
        memcpy(ptr, data, len);
        userp->linesRead++;
        return len;
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

bool EmailSender::sendEmail(const std::string &to, const std::string &subject, const std::string &message) {
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
            return false;
        }
    }
    return true;
    curl_slist_free_all(recipients);
}

