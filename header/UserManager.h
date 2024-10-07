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
    std::string connectionString; // Строка подключения к базе данных

    void loadUsers(); // Загружает пользователей из базы данных
    void saveUser(const User& user); // Сохраняет пользователя в базе данных


public:
    UserManager(const std::string& connStr); // Конструктор с параметром
    ~UserManager() = default;

    void createUser();
    void readUsers() const;
    void updateUser();
    void deleteUser();
    User* findUserById(int id);
};

#endif // LABS_USER_MANAGER_H