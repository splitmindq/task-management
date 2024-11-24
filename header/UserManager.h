//
// Created by mzitr on 16.09.2024.
//

#ifndef LABS_USER_MANAGER_H
#define LABS_USER_MANAGER_H
#include <vector>
#include <memory>
#include "User.h"
#include <pqxx/pqxx>

class UserManager {
private:
    std::vector<std::unique_ptr<User>> users;
    int nextId = 1;
    std::string connectionString;


    void loadUsers();


public:
    void loadUser(const std::string& username);
    void loadUserById(int userId);
    void saveUser(const User& user);
    explicit UserManager(const std::string& connStr);
    UserManager() = default;
    ~UserManager() = default;
    bool isUsernameTaken(const std::string& username){

        try {
            pqxx::connection C(connectionString);
            pqxx::work W(C);
            pqxx::result R = W.exec("SELECT COUNT(*) FROM users WHERE username = " + W.quote(username));
            return R[0][0].as<int>() > 0;
        }
        catch (const std::system_error &e) {
            std::cerr << e.what() << std::endl;
            return false;
        }
    };
    std::string getRole(const std::string& username);
    int getCompanyId(const std::string &username);
    void createUser(const std::string &name,const std::string &surname,const std::string &email,const std::string &login, const std::string &password);
    User* findUserById(int id);


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

  int  getId(const std::string &username);
  bool isAdmin(int id);
  void updateUserInDb(const User& user);


};


#endif // LABS_USER_MANAGER_H
