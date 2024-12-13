#ifndef TASK_MANAGEMENT_TASKMANAGER_H
#define TASK_MANAGEMENT_TASKMANAGER_H

#include "Task.h"
#include "UserManager.h"
#include "User.h"
#include "../ItemContainer/header/ItemContainer.h"
#include <pqxx/pqxx> // Убедитесь, что вы подключили библиотеку pqxx для работы с PostgreSQL

class TaskManager {
private:
    std::string connectionString;
    pqxx::connection conn;

    int getNextIdFromDb();
    void saveTaskToDb(const std::shared_ptr<Task> &task);

public:
    std::vector<std::shared_ptr<Task>> tasks;

    explicit TaskManager(const std::string& connectionString)
            : connectionString(connectionString), conn(connectionString) {}

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


    [[nodiscard]] std::shared_ptr<Task> getTaskByAim(const std::string &aim) {
        try {
            pqxx::work transaction(conn);

            std::string query = "SELECT id, user_id, company_id, aim, deadline, status FROM tasks WHERE aim = $1";
            pqxx::result result = transaction.exec_params(query, aim);

            if (!result.empty()) {

                int id = result[0][0].as<int>();
                int userId = result[0][1].as<int>();
                int companyId = result[0][2].as<int>();
                auto fetchedAim = result[0][3].as<std::string>();

                auto deadlineStr = result[0][4].as<std::string>();
                std::chrono::system_clock::time_point deadline;

                try {

                    std::tm tm = {};
                    std::istringstream ss(deadlineStr);
                    ss >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S");
                    if (ss.fail()) {
                        throw std::runtime_error("Не удалось разобрать строку времени: " + deadlineStr);
                    }

                    time_t time = std::mktime(&tm);
                    deadline = std::chrono::system_clock::from_time_t(time);
                } catch (const std::exception &e) {
                    std::cerr << "Ошибка при преобразовании времени: " << e.what() << '\n';
                    throw;
                }

                int status = result[0][5].as<int>();

                auto task = std::make_shared<Task>(id, userId, companyId, fetchedAim, deadline);
                task->setStatus(status);
                return task;
            }
            return nullptr;
        } catch (const std::exception &e) {
            std::cerr << "Ошибка выполнения запроса: " << e.what() << '\n';
            return nullptr;
        }
    }

    int getOverdueTaskCount(int userId);
    void updateTaskStatus(const Task& task);
};

#endif //TASK_MANAGEMENT_TASKMANAGER_H