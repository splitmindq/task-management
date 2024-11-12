//
// Created by mzitr on 04.11.2024.
//

// You may need to build the project (run Qt uic code generator) to get "ui_BasicClass.h" resolved

#include "basicclass.h"
#include "ui_BasicClass.h"


BasicClass::BasicClass(UserManager* userManager, QWidget *parent, User *user)
        : QMainWindow(parent), userManager(userManager),user(user) {
}

BasicClass::~BasicClass() = default;

std::string BasicClass::getEmail(const std::string &login) {

    return user->email;
}

std::string BasicClass::getName(const std::string &login) {

    return user->name;
}

std::string BasicClass::getSurname(const std::string &login) {

    return user->surname;
}
