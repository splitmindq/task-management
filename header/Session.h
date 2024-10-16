//
// Created by mzitr on 16.10.2024.
//

#ifndef TASK_MANAGEMENT_SESSION_H
#define TASK_MANAGEMENT_SESSION_H
#include <iostream>

class Session{

public:
    int userId;
    std::string username;
    bool isAdmin;


    Session(int id, const std::string& user, bool admin)
            : userId(id), username(user), isAdmin(admin) {}

    void displayInfo() const;
};



#endif //TASK_MANAGEMENT_SESSION_H
