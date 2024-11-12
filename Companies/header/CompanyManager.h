#ifndef TASK_MANAGEMENT_COMPANYMANAGER_H
#define TASK_MANAGEMENT_COMPANYMANAGER_H

#include <iostream>
#include <memory>
#include <string>
#include <pqxx/pqxx>
#include "Company.h"
#include "UserManager.h"

class CompanyManager {
private:

    UserManager* userManager;
    std::string connectionString;
    pqxx::connection conn;

    int getNextIdFromDb();
    void saveCompanyToDb(const std::shared_ptr<Company>& company);

public:
    std::shared_ptr<Company> company;

    explicit CompanyManager(UserManager* userManager, const std::string& companyName, int adminId, const std::string& connectionString)
            : userManager(userManager), connectionString(connectionString), conn(connectionString) {
        int newCompanyId = getNextIdFromDb();
        if (newCompanyId != -1) {
            company = std::make_shared<Company>(newCompanyId, companyName, adminId);
            saveCompanyToDb(company);
        }
    }

//    void inviteUser(const std::shared_ptr<User>& user);
//    void setAdmin(int userId);
    [[nodiscard]] std::shared_ptr<Company> getCompany() const;

};

#endif //TASK_MANAGEMENT_COMPANYMANAGER_H
