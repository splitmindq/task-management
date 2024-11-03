//
// Created by mzitr on 16.10.2024.
//

//
#include "User.h"
#include "UserManager.h"
User::User(int id, const std::string& email, const std::string& username,
           const std::string& password, const std::string& name,
           const std::string& surname)
        : id(id), email(email), username(username), password(password),
          name(name), surname(surname) {
}

User *UserManager::findUserById(int id) {
    for (const auto &user: users) {
        if (*user == id) {
            return user.get();
        }
    }
    return nullptr;
}
