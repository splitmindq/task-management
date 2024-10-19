//
// Created by mzitr on 16.09.2024.
//

#ifndef LABS_USER_MANAGER_H
#define LABS_USER_MANAGER_H

#include <vector>
#include <memory>
#include "User.h"
#include "Session.h"
#include <pqxx/pqxx>
class UserManager {
private:
    std::vector<std::unique_ptr<User>> users;
    int nextId = 1;
    std::string connectionString;
    std::unique_ptr<Session> currentSession;

    void loadUsers();
    void loadUser(const std::string& username);
    void saveUser(const User& user);
    bool isUsernameTaken(const std::string& username);
    std::string getRole(const std::string& username);
    int getId(const std::string& username);

public:
    explicit UserManager(const std::string& connStr);
    UserManager() = default;
    ~UserManager() = default;

    void createUser();
    void readUsers();
    void updateUser();
    void deleteUser();
    User* findUserById(int id);
    void authenticate();

    bool login(const std::string& username, const std::string& password)  {
        try {
            pqxx::connection C(connectionString);
            pqxx::work W(C);

            pqxx::result R = W.exec(
                    "SELECT COUNT(*) FROM users WHERE username = " + W.quote(username) +
                    " AND password = " + W.quote(password));

            return R[0][0].as<int>() > 0;
        }
        catch (const std::system_error& e) {
            std::cerr << e.what() << std::endl;
            return false;
        }
    }



};

#endif // LABS_USER_MANAGER_H
