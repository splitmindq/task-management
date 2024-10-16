//
// Created by mzitr on 16.09.2024.
//

#ifndef LABS_USER_MANAGER_H
#define LABS_USER_MANAGER_H

#include <vector>
#include <memory>
#include "User.h"

class UserManager {
private:
    std::vector<std::unique_ptr<User>> users;
    int nextId = 1;
    std::string connectionString;

    void loadUsers();
    void saveUser(const User& user);
    bool isUsernameTaken(const std::string& username);


public:

    explicit UserManager(const std::string& connStr);
    ~UserManager() = default;

    void createUser();
    void readUsers() const;
    void updateUser();
    void deleteUser();
    User* findUserById(int id);
    bool login(const std::string& username, const std::string& password);
    void authenticate();
    UserManager();
};

#endif // LABS_USER_MANAGER_H
