#ifndef TASK_MANAGEMENT_INVITE_H
#define TASK_MANAGEMENT_INVITE_H
#include "iostream"

class Invite {
private:
    int id;
    int senderId;

public:
    std::string message;
    int receiverId;

    explicit Invite(int inviteId, int senderId, const std::string &inviteMessage, int receiverId)
            : id(inviteId), senderId(senderId), message(inviteMessage), receiverId(receiverId) {}

    int getInviteId() const {
        return id;
    }

    int getSenderId() const {
        return senderId;
    }

    const std::string getMessage() const {
        return message;
    }

    int getReceiverId() const {
        return receiverId;
    }
};

#endif //TASK_MANAGEMENT_INVITE_H