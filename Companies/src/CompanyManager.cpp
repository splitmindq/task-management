#include "../header/CompanyManager.h"

int CompanyManager::getNextIdFromDb() {
    try {
        pqxx::work txn(conn);
        pqxx::result R = txn.exec("SELECT MAX(id) FROM companies;");
        txn.commit();
        if (R.empty() || R[0][0].is_null()){
            return 1;
        }
        return R.empty() ? 1 : R[0][0].as<int>() + 1;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
}

void CompanyManager::saveCompanyToDb(const std::shared_ptr<Company>& company) {
    try {
        pqxx::work txn(conn);
        txn.exec_params("INSERT INTO companies (name, adminId) VALUES ($1, $2)",
                        company->getName(), company->getAdminId());
        txn.commit();
    } catch (const std::exception& e) {
        std::cerr << "Ошибка при сохранении компании в базе данных: " << e.what() << std::endl;
    }
}

std::shared_ptr<Company> CompanyManager::findCompanyByAdminId(int adminId) {
    try {
        pqxx::work txn(conn);

        std::string query = "SELECT id, name FROM companies WHERE adminId = " + txn.quote(adminId) + " LIMIT 1;";
        pqxx::result result = txn.exec(query);

        if (!result.empty()) {
            int companyId = result[0][0].as<int>();
            std::string companyName = result[0][1].as<std::string>();
            return std::make_shared<Company>(companyId, companyName, adminId);
        }

        txn.commit();
    } catch (const std::exception& e) {
        std::cerr << "Error finding company: " << e.what() << std::endl;
    }

    return nullptr;
}
