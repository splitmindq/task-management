//
// Created by mzitr on 27.11.2024.
//

#ifndef TASK_MANAGEMENT_TASKMANAGER_H
#define TASK_MANAGEMENT_TASKMANAGER_H

#include "Task.h"
#include "UserManager.h"
#include "User.h"
#include "../ItemContainer/header/ItemContainer.h"
class TaskManager {

private:

    std::string connectionString;
    pqxx::connection conn;

    int getNextIdFromDb();

    void saveTaskToDb(const std::shared_ptr<Task> &task);

public:

    std::vector<std::shared_ptr<Task>> tasks;

    explicit TaskManager(const std::string& connectionString)
                        : connectionString(connectionString),conn(connectionString){}

    void createTask(int userId, int companyId, const std::string &aim,
                    const std::chrono::system_clock::time_point &deadline) {
        int newTaskId = getNextIdFromDb();
        if (newTaskId != -1) {
            auto taskObject = std::make_shared<Task>(newTaskId, userId, companyId, aim, deadline);
            saveTaskToDb(taskObject);
        }
    }

    [[nodiscard]] std::shared_ptr<Task> getTask(int id) const {
        for (const auto& task : tasks) {
            if (task->getId() == id) {
                return task;
            }
        }
        return nullptr;
    }
    int getOverdueTaskCount(int userId);
};


#endif //TASK_MANAGEMENT_TASKMANAGER_H
