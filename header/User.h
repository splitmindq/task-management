#ifndef LABS_USER_H
#define LABS_USER_H

#include <iostream>
#include <memory>
#include <string>
#include "Person.h"
class User: public Person{
public:
    int id;
    std::string email;
    std::string username;
    std::string password;
    std::string name;
    std::string surname;
    std::string role = "user"; // Например, "user", "admin"
    int companyId = -1; // -1 если не принадлежит компании

    void displayInfo() const override {
        std::cout << "Пользователь: " << username << ", Email: " << email << std::endl;
    }

    User(int id, const std::string& email, const std::string& username,
         const std::string& password, const std::string& name,
         const std::string& surname);

    bool operator==(int otherId) const {
        return this->id == otherId;
    }

    friend std::ostream& operator<<(std::ostream& os, const User& user) {
        os << "ID: " << user.id << ", Email: " << user.email << ", Username: " << user.username
           << ", Name: " << user.name << ", Surname: " << user.surname << ", Role: " << user.role;
        return os;
    }

    int getId() const { return id; }
    std::string getRole() const { return role; }

    ~User() = default;
};

#endif // LABS_USER_H