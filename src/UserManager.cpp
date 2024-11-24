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
        W.commit();
    } catch (const std::system_error &e) {
        std::cerr << "Ошибка при загрузке пользователя: " << e.what() << std::endl;
    }
}

void UserManager::loadUserById(int userId) {
    try {
        pqxx::connection C(connectionString);
        pqxx::work W(C);

        pqxx::result R = W.exec(
                "SELECT id, email, username, password, name, surname, role, companyId FROM users WHERE id = " +
                W.quote(userId));

        if (R.empty()) {
            std::cout << "Пользователь с id '" << userId << "' не найден." << std::endl;
            return;
        }

        const auto &row = R[0];
        std::string email = row[1].as<std::string>();
        std::string username = row[2].as<std::string>();
        std::string password = row[3].as<std::string>();
        std::string name = row[4].as<std::string>();
        std::string surname = row[5].as<std::string>();
        std::string role = row[6].as<std::string>();
        int companyId = row[7].as<int>();

        auto user = std::make_unique<User>(userId, email, username, password, name, surname);
        user->role = role;
        user->companyId = companyId;

        users.push_back(std::move(user));
        W.commit();
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
                "INSERT INTO users (id, email, username, password, name, surname, role, companyId) "
                "VALUES ({}, '{}', '{}', '{}', '{}', '{}', '{}', {}) "
                "ON CONFLICT (id) DO UPDATE "
                "SET email = EXCLUDED.email, "
                "username = EXCLUDED.username, "
                "password = EXCLUDED.password, "
                "name = EXCLUDED.name, "
                "surname = EXCLUDED.surname, "
                "role = EXCLUDED.role, "
                "companyId = EXCLUDED.companyId",
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

void UserManager::createUser(const std::string &name, const std::string &surname, const std::string &email,
                             const std::string &login, const std::string &password) {


    int id = getMaxUserId(connectionString);
    User newUser(id, email, login, password, name, surname);
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

int UserManager::getCompanyId(const std::string &username) {

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

bool UserManager::isAdmin(int id) {
    try {
        pqxx::connection C(connectionString);
        pqxx::work W(C);

        pqxx::result R = W.exec(
                "SELECT role FROM users WHERE id = " + W.quote(id)
        );

        if (!R.empty()) {
            std::string role = R[0][0].as<std::string>();
            return role == "admin";
        }
        return false;
    } catch (const std::system_error &e) {
        std::cerr << e.what() << std::endl;
        return false;
    }
}

void UserManager::updateUserInDb(const User &user) {
    try {
        pqxx::connection conn(connectionString);
        pqxx::work txn(conn);

        std::string query =
                "UPDATE users SET "
                "username = " + txn.quote(user.username) + ", "
                                                           "role = " + txn.quote(user.role) + ", "
                                                                                              "companyId = " +
                txn.quote(user.companyId) + " "
                                            "WHERE id = " + txn.quote(user.id) + ";";

        txn.exec(query);
        txn.commit();
    }
    catch (const std::system_error &e) {
        std::cerr << "Error updating user in database: " << e.what() << std::endl;
    }
}