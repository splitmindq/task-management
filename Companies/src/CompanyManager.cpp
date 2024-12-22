#include "../header/CompanyManager.h"

int CompanyManager::getNextIdFromDb() {
    try {
        pqxx::work txn(conn);
        std::string query = "SELECT MAX(id) FROM companies;";
        int maxId = executeScalarQuery<int>(txn, query);
        return maxId + 1;
    } catch (const std::system_error &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
}

void CompanyManager::saveCompanyToDb(const std::shared_ptr<Company> &companyObject) {
    try {
        pqxx::connection C(connectionString);
        pqxx::work W(C);

        W.exec(std::format(
                "INSERT INTO companies (id, name, adminId) "
                "VALUES ({}, '{}', {}) "
                "ON CONFLICT (id) DO UPDATE "
                "SET name = EXCLUDED.name, "
                "adminId = EXCLUDED.adminId",
                companyObject->getCompanyId(), companyObject->getName(), companyObject->getAdminId()
        ));

        W.commit();
    } catch (const std::system_error &e) {
        std::cerr << "Error while saving the companyObject to the database: " << e.what() << std::endl;
    }
}

std::shared_ptr<Company> CompanyManager::findCompanyByAdminId(int adminId) {
    try {
        pqxx::work txn(conn);

        std::string query = "SELECT id, name FROM companies WHERE adminId = " + txn.quote(adminId) + " LIMIT 1;";
        pqxx::result result = txn.exec(query);

        if (!result.empty()) {
            int companyId = result[0][0].as<int>();
            auto companyName = result[0][1].as<std::string>();
            return std::make_shared<Company>(companyId, companyName, adminId);
        }

        txn.commit();
    } catch (const std::system_error &e) {
        std::cerr << "Error finding company: " << e.what() << std::endl;
    }

    return nullptr;
}

std::shared_ptr<Company> CompanyManager::findCompanyById(int id) {
    try {
        pqxx::work txn(conn);

        std::string query = "SELECT id, name, adminId FROM companies WHERE id = " + txn.quote(id) + " LIMIT 1;";
        pqxx::result result = txn.exec(query);

        if (!result.empty()) {
            int companyId = result[0][0].as<int>();
            auto companyName = result[0][1].as<std::string>();
            int adminId = result[0][2].as<int>();
            return std::make_shared<Company>(companyId, companyName, adminId);
        }

        txn.commit();
    } catch (const std::system_error &e) {
        std::cerr << "Error finding company: " << e.what() << std::endl;
    }

    return nullptr;
}
