//
// Created by mzitr on 27.11.2024.
//

#include "../header/TaskManager.h"

int TaskManager::getNextIdFromDb() {

    try {
        pqxx::work txn(conn);
        pqxx::result R = txn.exec("SELECT MAX(id) FROM tasks;");
        txn.commit();
        if (R.empty() || R[0][0].is_null()) {
            return 1;
        }
        return R.empty() ? 1 : R[0][0].as<int>() + 1;
    }
    catch (const std::system_error &e) {

        std::cerr << "Error: " << e.what() << std::endl;
        return -1;

    }
}

void TaskManager::saveTaskToDb(const std::shared_ptr<Task> &task) {
    try {
        pqxx::work txn(conn);

        auto deadline_time = task->getDeadline();
        std::time_t deadline_t = std::chrono::system_clock::to_time_t(deadline_time);
        std::tm *tm_ptr = std::localtime(&deadline_t);

        std::ostringstream oss;
        oss << std::put_time(tm_ptr, "%Y-%m-%d %H:%M:%S");
        std::string deadline_str = oss.str();

        txn.exec_params(
                "INSERT INTO tasks (id, user_id, company_id, aim, deadline, status) VALUES ($1, $2, $3, $4, $5, $6)",
                task->getId(), task->getUserId(), task->getCompanyId(),
                task->getAim(), deadline_str, task->getStatus());
        txn.commit();
    } catch (const std::exception &e) {
        std::cerr << "Error saving task to database: " << e.what() << std::endl;

    }
}

int TaskManager::getOverdueTaskCount(int userId) {

    TaskContainer taskContainer;
    taskContainer.addFilter(TaskContainer::filterTasksByUserId(userId));
    taskContainer.loadTasksFromDatabase();
    int overdueCount = 0;
    for (auto &task: taskContainer) {
        if (task.isDeadlinePassed()) {
            ++overdueCount;
        }
    }

    return overdueCount;

}

void TaskManager::updateTaskStatus(const Task &task) {
    try {
        pqxx::work transaction(conn);

        std::string updateQuery = R"(
            UPDATE tasks
            SET status = $1
            WHERE user_id = $2 AND id = $3
        )";

        transaction.exec_params(updateQuery, task.getStatus(), task.getUserId(), task.getId());
        transaction.commit();

    } catch (const std::exception &e) {
        std::cerr << "Ошибка при обновлении статуса задачи: " << e.what() << '\n';
    }
}

