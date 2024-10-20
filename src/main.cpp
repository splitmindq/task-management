#include "sendingEmail.h"
#include "Session.h"

int main() {

    SetConsoleOutputCP(CP_UTF8);

    std::string connectionString = "host=localhost dbname=database user=mzitr password=yourpassword";
    UserManager userManager(connectionString);

    EmailSender emailSender("<emailsender11@mail.ru>", "smtps://smtp.mail.ru:465",
                            "emailsender11@mail.ru", "2bcQFBxtjmwUWdP7jxpT", "Kge6zBXU5SFsMnNsSkfk");

    while (true) {
        displayMenu();
        int choice;
        std::cout << "Enter your choice: ";
        std::cin >> choice;
        handleMenuChoice(choice, userManager, emailSender);
    }

    return 0;
}