#ifndef TASK_MANAGEMENT_INVITEMANAGER_H
#define TASK_MANAGEMENT_1INVITEMANAGER_H
#include "iostream"
#include "pqxx/pqxx"
#include "Invite.h"

class InviteManager {
private:
    std::string connectionString;
    pqxx::connection conn;
    int getNextIdFromDb();
    void saveInviteToDb(const std::shared_ptr<Invite<int>>& invite);

public:
    std::shared_ptr<Invite<int>> invite;

    explicit InviteManager(const std::string &connectionString)
            : connectionString(connectionString), conn(connectionString) {}

    void createInvite(const std::string &message, int senderId, int receiverId) {

        int newInviteId = getNextIdFromDb();
        if (newInviteId != -1) {
            auto invite = std::make_shared<Invite<int>>(newInviteId, senderId, message, receiverId);
            saveInviteToDb(invite);
        }
    }

    [[nodiscard]] std::shared_ptr<Invite<int>> getInvite() const {
        return invite;
    }
};

#endif //TASK_MANAGEMENT_INVITEMANAGER_H