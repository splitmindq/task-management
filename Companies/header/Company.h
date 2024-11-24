#ifndef TASK_MANAGEMENT_COMPANY_H
#define TASK_MANAGEMENT_COMPANY_H

#include <iostream>
#include <string>
#include <stdexcept>

class Company {
private:
    int id;
    std::string companyName;
    int adminId;

public:
    Company(int companyId, const std::string &name, int adminId = -1)
            : id(companyId), adminId(adminId) {
        setName(name);
    }

    [[nodiscard]] int getCompanyId() const { return id; }

    [[nodiscard]] const std::string &getName() const { return companyName; }

    [[nodiscard]] int getAdminId() const { return adminId; }

    void setAdminId(int setAdminId) { adminId = setAdminId; }

    void setName(const std::string_view &name) {
        if (name.empty()) {
            throw std::invalid_argument("Company name cannot be empty.");
        }
        companyName = name;
    }

};

#endif // TASK_MANAGEMENT_COMPANY_H