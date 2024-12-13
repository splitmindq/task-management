#include <iostream>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <pqxx/pqxx>
#include "../header/ItemContainer.h"

std::chrono::system_clock::time_point parseDeadline(const std::string& deadlineStr) {
    std::tm tm = {};
    std::istringstream ss(deadlineStr);
    ss >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S");
    return std::chrono::system_clock::from_time_t(std::mktime(&tm));
}

template<>
void UserContainer::loadUsersFromDatabase() {
    try {
        pqxx::connection conn(connStr);
        pqxx::work txn(conn);

        std::string sql = "SELECT id, email, username, password, name, surname FROM users";
        std::ostringstream whereClause;
        bool firstFilter = true;

        for (const auto& filter : filters) {
            std::string filterStr = filter();
            if (!filterStr.empty() && filterStr.find("LIMIT") == std::string::npos && filterStr.find("OFFSET") == std::string::npos) {
                if (firstFilter) {
                    whereClause << " WHERE " << filterStr;
                    firstFilter = false;
                } else {
                    whereClause << " AND " << filterStr;
                }
            }
        }

        sql += whereClause.str();

        std::string limitStr, offsetStr;
        for (const auto& filter : filters) {
            std::string filterStr = filter();
            if (filterStr.find("LIMIT") != std::string::npos) {
                limitStr = filterStr;
            }
            if (filterStr.find("OFFSET") != std::string::npos) {
                offsetStr = filterStr;
            }
        }

        if (!limitStr.empty()) {
            sql += " " + limitStr;
        }
        if (!offsetStr.empty()) {
            sql += " " + offsetStr;
        }

        pqxx::result res = txn.exec(sql);
        for (const auto &row: res) {
            User user(
                    row["id"].as<int>(),
                    row["email"].as<std::string>(),
                    row["username"].as<std::string>(),
                    row["password"].as<std::string>(),
                    row["name"].as<std::string>(),
                    row["surname"].as<std::string>()
            );
            addItem(user);
        }

        txn.commit();
    } catch (const std::exception &e) {
        std::cerr << "Ошибка при загрузке пользователей: " << e.what() << std::endl;
    }
}

template<>
void TaskContainer::loadTasksFromDatabase() {
    try {
        pqxx::connection conn(connStr);
        pqxx::work txn(conn);

        std::string sql = "SELECT id, user_id, company_id, aim, deadline, status FROM tasks";
        std::ostringstream whereClause;
        bool firstFilter = true;

        for (const auto& filter : filters) {
            std::string filterStr = filter();
            if (!filterStr.empty() && filterStr.find("LIMIT") == std::string::npos && filterStr.find("OFFSET") == std::string::npos) {
                if (firstFilter) {
                    whereClause << " WHERE " << filterStr;
                    firstFilter = false;
                } else {
                    whereClause << " AND " << filterStr;
                }
            }
        }

        sql += whereClause.str();
        pqxx::result res = txn.exec(sql);
        for (const auto &row: res) {
            Task task(
                    row["id"].as<int>(),
                    row["user_id"].as<int>(),
                    row["company_id"].as<int>(),
                    row["aim"].as<std::string>(),
                    parseDeadline(row["deadline"].as<std::string>())
            );
            task.setStatus(row["status"].as<int>());
            addItem(task);
        }

        txn.commit();
    } catch (const std::exception &e) {
        std::cerr << "Ошибка при загрузке задач: " << e.what() << std::endl;
    }
}