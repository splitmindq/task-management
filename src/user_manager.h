//
// Created by mzitr on 16.09.2024.
//

#ifndef LABS_USER_MANAGER_H
#define LABS_USER_MANAGER_H


class UserManager {
private:
    std::vector<std::unique_ptr<User>> users;
    int next_id = 1;

    void delete_all_users();

public:
    UserManager();
    ~UserManager() = default;

    void create_user();
    void read_users() const;
    void update_user();
    void delete_user();
    User* find_user_by_id(int id);
};





#endif //LABS_USER_MANAGER_H
