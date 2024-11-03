//
// Created by mzitr on 04.11.2024.
//

// You may need to build the project (run Qt uic code generator) to get "ui_BasicClass.h" resolved

#include "basicclass.h"
#include "ui_BasicClass.h"


BasicClass::BasicClass(UserManager* userManager, QWidget *parent, const std::string& username)
        : QMainWindow(parent), userManager(userManager), username(username) {

}

BasicClass::~BasicClass() {

}

std::string BasicClass::getEmail(const std::string &username) {
    try {
        pqxx::connection C("host=localhost dbname=database user=mzitr password=yourpassword");
        pqxx::work W(C);

        pqxx::result R = W.exec("SELECT email FROM users WHERE username = " + W.quote(username));
        if (!R.empty()) {
            return R[0][0].as<std::string>();
        } else {
            return "";
        }
    } catch (const std::system_error &e) {
        std::cerr << e.what() << std::endl;
        return "";
    }
}

std::string BasicClass::getName(const std::string &username) {
    try {
        pqxx::connection C("host=localhost dbname=database user=mzitr password=yourpassword");
        pqxx::work W(C);

        pqxx::result R = W.exec("SELECT name FROM users WHERE username = " + W.quote(username));
        if (!R.empty()) {
            return R[0][0].as<std::string>();
        } else {
            return "";
        }
    } catch (const std::system_error &e) {
        std::cerr << e.what() << std::endl;
        return "";
    }
}

std::string BasicClass::getSurname(const std::string &username) {
    try {
        pqxx::connection C("host=localhost dbname=database user=mzitr password=yourpassword");
        pqxx::work W(C);

        pqxx::result R = W.exec("SELECT surname FROM users WHERE username = " + W.quote(username));
        if (!R.empty()) {
            return R[0][0].as<std::string>();
        } else {
            return "";
        }
    } catch (const std::system_error &e) {
        std::cerr << e.what() << std::endl;
        return "";
    }
}
