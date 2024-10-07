#include "sendingEmail.h"

int main() {

    SetConsoleOutputCP(CP_UTF8);
    std::string connectionString = "host=localhost dbname=database user=mzitr password=yourpassword";
    UserManager userManager(connectionString);

    EmailSender emailSender("<emailsender11@mail.ru>", "smtp://smtp.mail.ru:587",
                            "emailsender11@mail.ru", "XPm2jZ2YS0ECx5q5PKzW", "Kge6zBXU5SFsMnNsSkfk");

    while (true) {
        displayMenu();
        int choice;
        std::cout << "Enter your choice: ";
        std::cin >> choice;
        handleMenuChoice(choice, userManager, emailSender);
    }

    return 0;
}