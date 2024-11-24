//
// Created by mzitr on 13.11.2024.
//

#include "../headers/InviteManager.h"

int InviteManager::getNextIdFromDb() {
    try {
        pqxx::work txn(conn);
        pqxx::result R = txn.exec("SELECT MAX(id) FROM invites;");
        txn.commit();
        if (R.empty() || R[0][0].is_null()) {
            return 1;
        }
        return R.empty() ? 1 : R[0][0].as<int>() + 1;
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
}

void InviteManager::saveInviteToDb(const std::shared_ptr<Invite<int>> &invite) {
    try {
        pqxx::work txn(conn);
        txn.exec_params("INSERT INTO invites (id, message, sender_id, user_id) VALUES ($1, $2, $3, $4)",
                        invite->getInviteId(), invite->getMessage(), invite->getSenderId(), invite->getReceiverId());
        txn.commit();
    } catch (const std::exception &e) {
        std::cerr << "Error saving invite to database: " << e.what() << std::endl;
    }
}