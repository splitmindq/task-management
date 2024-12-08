//
// Created by mzitr on 27.11.2024.
//

#ifndef TASK_MANAGEMENT_TASK_H
#define TASK_MANAGEMENT_TASK_H

#include "iostream"
#include "chrono"
#include "ctime"

class Task {

private:
    int id;
    int userId;
    int companyId;
    std::string aim;
    std::chrono::system_clock::time_point deadline;
    bool status = false;


public:
    explicit Task(int id, int userId, int companyId, const std::string &aim,
                  const std::chrono::system_clock::time_point &deadline)
            : id(id), userId(userId), companyId(companyId), aim(aim), deadline(deadline) {}


    [[nodiscard]] int getId() const { return id; }

    [[nodiscard]] int getUserId() const { return userId; }

    [[nodiscard]] int getCompanyId() const { return companyId; }

    [[nodiscard]] std::string getAim() const { return aim; }

    [[nodiscard]] std::chrono::system_clock::time_point getDeadline() const { return deadline; }

    [[nodiscard]] bool getStatus() const { return status; }

    void setStatus(bool newStatus) { status = newStatus; }

    [[nodiscard]] bool isDeadlinePassed() const {
        return std::chrono::system_clock::now() > deadline;
    }


};


#endif //TASK_MANAGEMENT_TASK_H
