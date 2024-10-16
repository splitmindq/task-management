// Session.cpp
#include "Session.h"

void Session::sessionMenu() const {
    int choice;

    while (true) {
        std::cout << "\n--- Меню сессии ---\n";
        std::cout << "1. Просмотр информации о пользователе\n";
        std::cout << "2. Сохранить сессию\n";
        std::cout << "3. Изменить информацию о пользователе\n";
        std::cout << "4. Создать компанию (стать администратором)\n";
        std::cout << "5. Выход из системы\n";
        std::cout << "6. Завершить работу\n";
        std::cout << "Выберите действие: ";
        std::cin >> choice;

        switch (choice) {
            case 1:
                std::cout << "Информация о пользователе: " << user.username << std::endl; // Например, вывод имени пользователя
                break;

            case 2:
                std::cout << "Сохранение сессии...\nСессия сохранена успешно.\n";
                break;

            case 3:
                std::cout << "Изменение информации о пользователе...\n";
                // Добавьте логику изменения информации о пользователе
                break;

            case 4:
                std::cout << "Создание компании...\n";
                // Логика создания компании
                break;

            case 5:
                std::cout << "Выход из системы...\n";

                return; // Завершение сессии

            case 6:
                std::cout << "Завершение работы...\n";
                exit(0); // Завершение программы

            default:
                std::cout << "Неверный ввод. Попробуйте снова.\n";
                break;
        }
    }
}
