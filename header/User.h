//
// Created by mzitr on 16.09.2024.
//

#ifndef LABS_USER_H
#define LABS_USER_H

#include <memory>
#include <string>

class User {
public:
    int id;
    std::string email;
    std::string username;
    std::string password;
    std::string name;
    std::string surname;
    std::string role; // Например, "user", "admin"
    int companyId; // -1 если не принадлежит компании

    User(int id, const std::string& email, const std::string& username, const std::string& password, const std::string& name, const std::string& surname);

    bool operator==(int otherId) const {
        return this->id == otherId;
    }

    friend std::ostream& operator<<(std::ostream& os, const User& user) {
        os << "ID: " << user.id << ", Email: " << user.email << ", Username: " << user.username
           << ", Name: " << user.name << ", Surname: " << user.surname << ", Role: " << user.role;
        return os;
    }

    friend std::istream& operator>>(std::istream& is, User& user) {
        std::cout << "Введите email: ";
        is >> user.email;
        std::cout << "Введите имя: ";
        is >> user.name;
        std::cout << "Введите фамилию: ";
        is >> user.surname;
        std::cout << "Введите логин: ";
        is >> user.username;
        std::cout << "Введите пароль: ";
        is >> user.password;
        return is;
    }


    ~User() = default;
};

#endif // LABS_USER_H