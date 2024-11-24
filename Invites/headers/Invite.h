#ifndef TASK_MANAGEMENT_INVITE_H
#define TASK_MANAGEMENT_INVITE_H

#include <iostream>
#include <string>

template<typename T>
class Invite {
private:
    T id;
    T senderId;
    std::string message;
    T receiverId;

public:
    Invite(T inviteId, T senderId, const std::string &inviteMessage, T receiverId)
            : id(inviteId), senderId(senderId), message(inviteMessage), receiverId(receiverId) {}

    T getInviteId() const {
        return id;
    }

    T getSenderId() const {
        return senderId;
    }

    std::string getMessage() const {
        return message;
    }

    T getReceiverId() const {
        return receiverId;
    }
};

#endif //TASK_MANAGEMENT_INVITE_H
