#ifndef TASK_MANAGEMENT_USERCONTAINER_H
#define TASK_MANAGEMENT_USERCONTAINER_H

#include "vector"
#include "User.h"
#include "string"
#include "pqxx/pqxx"
#include "UserManager.h"
#include "../UsersIterator/header/UsersIterator.h"

class UserContainer {
private:
    std::vector<User> users;
    std::vector<std::function<std::string()>> filters;
    std::string connStr = "host=localhost dbname=database user=mzitr password=yourpassword";

public:
    void addUser(const User &user) {
        users.push_back(user);
    }

    void removeUser(size_t index) {
        if (index < users.size()) {
            users.erase(users.begin() + index);
        }
    }

    size_t size() const {
        return users.size();
    }

    auto begin() {
        return UsersIterator(users.begin());
    }

    auto end() {
        return UsersIterator(users.end());
    }

    void addFilter(const std::function<std::string()> filter) {
        filters.push_back(filter);
    }

    void loadUsersFromDatabase();

    static std::function<std::string()> filterByRole(const std::string &role) {
        return [role]() {
            return "role = '" + role + "'";
        };
    }

    static std::function<std::string()> filterByCompanyId(int companyId) {
        return [companyId]() {
            return "companyId = '" + std::to_string(companyId) + "'";
        };
    }
};

#endif //TASK_MANAGEMENT_USERCONTAINER_H
