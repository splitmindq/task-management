//
// Created by mzitr on 16.10.2024.
//
#include <iostream>
#include "UserManager.h"
#include <string>
#include <memory>
#include <array>
#include <locale>
#include "pqxx/pqxx"


UserManager::UserManager(const std::string &connStr) : connectionString(connStr) {}


bool UserManager::isUsernameTaken(const std::string &username) {

    try {
        pqxx::connection C(connectionString);
        pqxx::work W(C);
        pqxx::result R = W.exec("SELECT COUNT(*) FROM users WHERE username = " + W.quote(username));
        return R[0][0].as<int>() > 0;
    }
    catch (const std::system_error &e) {
        std::cerr << e.what() << std::endl;
        return false;
    }
}






void UserManager::loadUser(const std::string &username) {
    try {
        pqxx::connection C(connectionString);
        pqxx::work W(C);

        pqxx::result R = W.exec(
                "SELECT id, email, username, password, name, surname, role, companyId FROM users WHERE username = " +
                W.quote(username));

        if (R.empty()) {
            std::cout << "Пользователь с именем пользователя '" << username << "' не найден." << std::endl;
            return;
        }

        const auto &row = R[0];
        int id = row[0].as<int>();
        std::string email = row[1].as<std::string>();
        std::string password = row[3].as<std::string>();
        std::string name = row[4].as<std::string>();
        std::string surname = row[5].as<std::string>();
        std::string role = row[6].as<std::string>();
        int companyId = row[7].as<int>();

        auto user = std::make_unique<User>(id, email, username, password, name, surname);
        user->role = role;
        user->companyId = companyId;

        users.push_back(std::move(user));
        nextId = std::max(nextId, id + 1);

        W.commit(); // Коммитим транзакцию
    } catch (const std::system_error &e) {
        std::cerr << "Ошибка при загрузке пользователя: " << e.what() << std::endl;
    }
}


void UserManager::loadUsers() {

    try {
        pqxx::connection C(connectionString);
        pqxx::work W(C);

        pqxx::result R = W.exec("SELECT id, email, username, password, name, surname, role, companyId FROM users");

        if (R.empty()) {
            nextId = 1;
        } else {
            for (const auto &row: R) {
                int id = row[0].as<int>();
                std::string email = row[1].as<std::string>();
                std::string username = row[2].as<std::string>();
                std::string password = row[3].as<std::string>();
                std::string name = row[4].as<std::string>();
                std::string surname = row[5].as<std::string>();
                std::string role = row[6].as<std::string>();
                int companyId = row[7].as<int>();

                auto user = std::make_unique<User>(id, email, username, password, name, surname);
                user->role = role;
                user->companyId = companyId;

                users.push_back(std::move(user));
                nextId = std::max(nextId, id + 1);
            }
        }

        W.commit();
    } catch (const std::system_error &e) {
        std::cerr << e.what() << std::endl;
    }
}

void UserManager::saveUser(const User &user) {

    try {
        pqxx::connection C(connectionString);
        pqxx::work W(C);
        W.exec(std::format(
                "INSERT INTO users (id, email, username, password, name, surname, role, companyId) VALUES ({}, '{}', '{}', '{}', '{}', '{}', '{}', {})",
                user.id, user.email, user.username, user.password, user.name, user.surname, user.role, user.companyId
        ));

        W.commit();
    } catch (const std::system_error &e) {
        std::cerr << e.what() << std::endl;
    }
}



int getMaxUserId(const std::string &connectionString) {
    int maxId = 1;

    try {
        pqxx::connection C(connectionString);
        pqxx::work W(C);

        pqxx::result R = W.exec("SELECT MAX(id) FROM users");

        if (!R.empty() && !R[0][0].is_null()) {
            maxId = R[0][0].as<int>();
        } else {
            std::cout << "Пользователи не найдены." << std::endl;
        }

        W.commit();
    } catch (const std::system_error &e) {
        std::cerr << "Ошибка при выполнении запроса: " << e.what() << std::endl;
    }

    return maxId + 1;
}


void UserManager::createUser() {
    std::string username;

    do {
        std::cout << "Введите логин: ";
        std::cin >> username;

        if (isUsernameTaken(username)) {
            std::cout << "Этот логин уже занят. Пожалуйста, выберите другой." << std::endl;
        }
    } while (isUsernameTaken(username));

    std::string email;
    std::cout << "Введите email: ";
    std::cin >> email;
    std::string password;
    std::cout << "Введите пароль: ";
    std::cin >> password;
    std::string name;
    std::cout << "Введите имя: ";
    std::cin >> name;
    std::string surname;
    std::cout << "Введите фамилию: ";
    std::cin >> surname;

    int id = getMaxUserId(connectionString);
    User newUser(id, email, username, password, name, surname);
    saveUser(newUser);
    users.push_back(std::make_unique<User>(
            newUser.id,
            newUser.email,
            newUser.username,
            newUser.password,
            newUser.name,
            newUser.surname
    ));
    std::cout << "Создан пользователь: " << newUser << std::endl;

}

int UserManager::getId(const std::string &username) {

    try {
        pqxx::connection C(connectionString);
        pqxx::work W(C);

        pqxx::result R = W.exec("SELECT id FROM users WHERE username = " + W.quote(username));

        if (R.size() == 1) {
            return R[0][0].as<int>();
        } else {
            return -1;
        }
    }
    catch (const std::system_error &e) {
        std::cerr << e.what() << std::endl;
        return -1;

    }

}

int UserManager::getCompanyId(const std::string &username)  {

    try {
        pqxx::connection C(connectionString);
        pqxx::work W(C);

        pqxx::result R = W.exec("SELECT companyId FROM users WHERE username = " + W.quote(username));

        if (R.size() == 1) {
            return R[0][0].as<int>();
        } else {
            return -1;
        }
    }
    catch (const std::system_error &e) {
        std::cerr << e.what() << std::endl;
        return -1;

    }
}


std::string UserManager::getRole(const std::string &username) {

    try {
        pqxx::connection C(connectionString);
        pqxx::work W(C);

        pqxx::result R = W.exec("SELECT role FROM users WHERE username = " + W.quote(username));

        if (!R.empty()) {
            return R[0][0].as<std::string>();
        } else {
            return "";
        }
    }
    catch (const std::system_error &e) {
        std::cerr << e.what() << std::endl;
        return "";

    }

}

void UserManager::authenticate() {
    std::string username;
    std::cout << "Введите логин: ";
    std::cin >> username;
    std::string password;
    std::cout << "Введите пароль: ";
    std::cin >> password;


    if (login(username, password)) {
        std::cout << "Авторизация прошла успешно!" << std::endl;
        int userId = getId(username);
        loadUser(username);
        User *currentUser = findUserById(userId);

        if (currentUser) {
            currentSession = std::make_unique<Session>(*currentUser);
            currentSession->sessionMenu();
        }


    } else {
        std::cout << "Неверный логин или пароль." << std::endl;
    }

}

void UserManager::readUsers()  {
    loadUsers();
    if (users.empty()) {
        std::cout << "Нету такого студента" << std::endl;
        return;
    }

    std::cout << "List of users:\n";
    for (const auto &user: users) {
        std::cout << *user << std::endl;
    }
}

void UserManager::updateUser() {
    int id;
    std::cout << "Введите ID пользователя ";
    std::cin >> id;

    User *user = findUserById(id);
    if (user) {
        std::string newEmail;
        std::cout << "Введите новый email: ";
        std::cin >> newEmail;

        user->email = newEmail;

        try {
            pqxx::connection C(connectionString);
            pqxx::work W(C);
            W.exec("UPDATE users SET email = " + W.quote(newEmail) + " WHERE id = " + std::to_string(id));
            W.commit();
        } catch (const std::system_error &e) {
            std::cerr << e.what() << std::endl;
        }

        std::cout << "Пользователь с ID: " << id << " обновлен." << std::endl;
    } else {
        std::cout << "Пользователь с ID: " << id << " не найден." << std::endl;
    }
}

void UserManager::deleteUser() {
    int id;
    std::cout << "Введите ID пользователя для удаления: ";
    std::cin >> id;

    for (auto it = users.begin(); it != users.end(); ++it) {
        if ((*it)->id == id) {
            std::cout << "Пользователь с ID " << id << " с почтой " << (*it)->email << " удалён." << std::endl;

            try {
                pqxx::connection C(connectionString);
                pqxx::work W(C);
                W.exec(std::format("DELETE FROM users WHERE id = {}", std::to_string(id)));
                W.commit();
            } catch (const std::system_error &e) {
                std::cerr << e.what() << std::endl;
            }
            users.erase(it);
            return;
        }
    }
    std::cout << "Пользователь с ID " << id << " не найден." << std::endl;

}
