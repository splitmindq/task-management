//
// Created by mzitr on 17.11.2024.
//

#include "../header/UserContainer.h"

void UserContainer::loadUsersFromDatabase() {
    try {
        pqxx::connection conn(connStr);
        pqxx::work txn(conn);

        std::string sql = "SELECT id, email, username, password, name, surname FROM users WHERE ";
        bool first = true;
        for (const auto& filter : filters) {
            if (!first) {
                sql += " AND ";
            }
            sql += filter();
            first = false;
        }

        pqxx::result res = txn.exec(sql);

        for (const auto& row : res) {
            User user(
                    row["id"].as<int>(),
                    row["email"].as<std::string>(),
                    row["username"].as<std::string>(),
                    row["password"].as<std::string>(),
                    row["name"].as<std::string>(),
                    row["surname"].as<std::string>()
            );
            addUser(user);
        }

        txn.commit();
    }
    catch (const std::exception& e) {
        std::cerr << "Ошибка при загрузке пользователей: " << e.what() << std::endl;
    }
}
