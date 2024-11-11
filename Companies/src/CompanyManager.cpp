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
