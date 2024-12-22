#ifndef TASK_MANAGEMENT_COMPANYMANAGER_H
#define TASK_MANAGEMENT_COMPANYMANAGER_H

#include <iostream>
#include <memory>
#include <string>
#include <pqxx/pqxx>
#include "Company.h"
#include "UserManager.h"
#include "QMessageBox"


class CompanyManager {
private:
    UserManager *userManager;
    std::string connectionString;
    pqxx::connection conn;

    int getNextIdFromDb();

public:
    void saveCompanyToDb(const std::shared_ptr<Company> &companyObject);

    std::shared_ptr<Company> company;

    explicit CompanyManager(UserManager *userManager, const std::string &connectionString)
            : userManager(userManager), connectionString(connectionString), conn(connectionString) {}

    void createCompany(const std::string &companyName, int adminId) {
        int newCompanyId = getNextIdFromDb();

        if (newCompanyId == -1) {
            newCompanyId = 1;
        }

        company = std::make_shared<Company>(newCompanyId, companyName, adminId);


        std::cout << "Company successfully created:" << companyName << std::endl;
        saveCompanyToDb(company);
    }


    [[nodiscard]] std::shared_ptr<Company> getCompany() const {
        return company;
    }

    std::shared_ptr<Company> findCompanyByAdminId(int adminId);

    std::shared_ptr<Company> findCompanyById(int id);


    template<typename T>
    T executeScalarQuery(pqxx::work &txn, const std::string &query) {
        pqxx::result R = txn.exec(query);
        if (R.empty() || R[0][0].is_null()) {
            throw std::invalid_argument("Query result is empty or NULL");
        }
        return R[0][0].as<T>();
    }

    void deleteCompany(const std::string &companyId) {

        try {
            pqxx::work txn(conn);
            txn.exec_params("DELETE FROM companies WHERE id = $1", companyId);
            txn.commit();
        } catch (const std::system_error &e) {
            std::cerr << "Error while deleting company: " << e.what() << std::endl;

        }


    }
};


#endif //TASK_MANAGEMENT_COMPANYMANAGER_H
