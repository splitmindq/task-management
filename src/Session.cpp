//
// Created by mzitr on 16.10.2024.
//

#include "Session.h"

void Session::displayInfo() const {
    std::cout << "Пользователь: " << username << "\n";
    std::cout << "ID: " << userId << "\n";
    std::cout << "Администратор: " << (isAdmin ? "Да" : "Нет") << "\n";
}
