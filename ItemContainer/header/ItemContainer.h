#ifndef TASK_MANAGEMENT_ITEMCONTAINER_H
#define TASK_MANAGEMENT_ITEMCONTAINER_H

#include <vector>
#include <string>
#include <functional>
#include <pqxx/pqxx>
#include "User.h"
#include "../Task/header/Task.h"
#include "../ItemsIterator/header/ItemsIterator.h"

template <typename T>
class ItemContainer {
private:
    std::vector<T> items;
    std::vector<std::function<std::string()>> filters;
    std::string connStr = "host=localhost dbname=database user=mzitr password=yourpassword";

public:
    void addItem(const T &item) {
        items.push_back(item);
    }

    void removeItem(size_t index) {
        if (index < items.size()) {
            items.erase(items.begin() + index);
        }
    }

    size_t size() const {
        return items.size();
    }

    auto begin() {
        return Iterator<T>(items.begin());
    }

    auto end() {
        return Iterator<T>(items.end());
    }

    void addFilter(const std::function<std::string()> &filter) {
        filters.push_back(filter);
    }

    void loadUsersFromDatabase();
    void loadTasksFromDatabase();

    static std::function<std::string()> filterByRole(const std::string &role) {
        return [role]() {
            return std::format("role = '{}'",role);
        };
    }

    static std::function<std::string()> filterByCompanyId(int companyId) {
        return [companyId]() {
            return std::format("companyId = '{}'", companyId);
        };
    }

    static std::function<std::string()> filterTasksByUserId(int userId) {
        return [userId]() {
            return std::format("user_id = '{}'", userId);
        };
    }

    static std::function<std::string()> filterTasksByStatus(int status) {
        return [status]() {
            return std::format("status = '{}'", status);
        };
    }

    static std::function<std::string()> filterByLimit(int limit) {
        return [limit]() {
            return std::format("LIMIT {}", limit);
        };
    }

    static std::function<std::string()> filterByOffset(int offset) {
        return [offset]() {
            return std::format("OFFSET {}", offset);
        };
    }

    static std::function<std::string()> filtredByTime(int remainingDays) {
        return [remainingDays]() {
            return std::format("deadline <= NOW() + INTERVAL '{} days' AND deadline >= NOW()", remainingDays);        };
    }



};


using UserContainer = ItemContainer<User>;
using TaskContainer = ItemContainer<Task>;

#endif //TASK_MANAGEMENT_ITEMCONTAINER_H