#include "sendingEmail.h"

User::User(int id, const std::string& email) : id(id), email(std::make_unique<std::string>(email)) {}

UserManager::UserManager() : next_id(1) {}

UserManager::~UserManager() {
    // Destructor is not needed because smart pointers automatically deallocate memory
}

void UserManager::create_user() {
    std::string email_input;
    std::cout << "Enter user email: ";
    std::cin >> email_input;

    users.push_back(std::make_unique<User>(next_id++, email_input));
    std::cout << "User created with ID: " << users.back()->id << std::endl;
}

void UserManager::read_users() const {
    if (users.empty()) {
        std::cout << "No users found." << std::endl;
        return;
    }

    std::cout << "List of users:\n";
    for (const auto& user : users) {
        std::cout << "ID: " << user->id << ", Email: " << *(user->email) << std::endl;
    }
}

void UserManager::update_user() {
    int id;
    std::cout << "Enter user ID to update: ";
    std::cin >> id;

    User* user = find_user_by_id(id);
    if (user) {
        std::string new_email;
        std::cout << "Enter new email: ";
        std::cin >> new_email;
        user->email = std::make_unique<std::string>(new_email);
        std::cout << "User with ID " << id << " updated." << std::endl;
    } else {
        std::cout << "User with ID " << id << " not found." << std::endl;
    }
}

void UserManager::delete_user() {
    int id;
    std::cout << "Enter user ID to delete: ";
    std::cin >> id;

    auto it = std::find_if(users.begin(), users.end(), [id](const std::unique_ptr<User>& user) {
        return user->id == id;
    });

    if (it != users.end()) {
        std::cout << "User with ID " << id << " and email " << *(*it)->email << " has been deleted." << std::endl;
        users.erase(it);  // Smart pointer automatically frees memory
    } else {
        std::cout << "User with ID " << id << " not found." << std::endl;
    }
}

User* UserManager::find_user_by_id(int id) {
    for (const auto& user : users) {
        if (user->id == id) {
            return user.get();  // Return raw pointer to the user object
        }
    }
    return nullptr;
}
