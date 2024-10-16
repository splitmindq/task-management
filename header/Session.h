// Session.h
#ifndef TASK_MANAGEMENT_SESSION_H
#define TASK_MANAGEMENT_SESSION_H

#include <iostream>
#include "User.h"

class Session {
private:
    User user;

public:
    explicit Session(const User &user) : user(user) {}
    void sessionMenu() const;
};

#endif //TASK_MANAGEMENT_SESSION_H
