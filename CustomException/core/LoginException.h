//
// Created by mzitr on 14.11.2024.
//

#ifndef TASK_MANAGEMENT_LOGINEXCEPTION_H
#define TASK_MANAGEMENT_LOGINEXCEPTION_H


#include <iostream>
#include <fstream>
#include <stdexcept>
#include <string>
#include "LoginException.h"

class LoginException : public std::exception {
private:
    std::string message;

public:
    explicit LoginException(const std::string &msg)
            : message(msg) {};

    const char *what() const noexcept override {
        return message.c_str();
    }


};


#endif //TASK_MANAGEMENT_LOGINEXCEPTION_H
