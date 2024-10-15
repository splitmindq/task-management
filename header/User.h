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

    User(int id, const std::string& email);

    bool operator==(int otherId) const {
        return this->id == otherId;
    }

    friend std::ostream& operator<<(std::ostream& os, const User& user){
        os << "ID: "<<user.id<<", Email:"<<user.email;
        return os;
    }

    friend std::istream& operator>>(std::istream& is, User& user){

        std::cout<<"Введите email: ";
        std::string emailInput;
        is>> emailInput;
        user.email = emailInput;
        return is;

    }

    ~User() = default;
};

#endif // LABS_USER_H