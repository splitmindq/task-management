//
// Created by mzitr on 16.09.2024.
//

#ifndef LABS_USER_H
#define LABS_USER_H

class User {
public:
    int id;
    std::unique_ptr<std::string>email;

    User(int id, const std::string& email);
    ~User() = default;
};


#endif //LABS_USER_H
