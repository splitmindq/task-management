#include "sendingEmail.h"

#include "headers/mainwindow.h"
#include "QApplication"


int main(int argc, char*argv[]) {
    std::string connectionString = "host=localhost dbname=database user=mzitr password=yourpassword";
    UserManager userManager(connectionString);

    SetConsoleOutputCP(CP_UTF8);


    EmailSender emailSender("<emailsender11@mail.ru>", "smtps://smtp.mail.ru:465",
                            "emailsender11@mail.ru", "2bcQFBxtjmwUWdP7jxpT", "Kge6zBXU5SFsMnNsSkfk");

    QApplication MainMenu(argc,argv);
    MainWindow window(&userManager);
    window.show();

//
//    while (true) {
//        displayMenu();
//        int choice;
//        std::cout << "Enter your choice: ";
//        std::cin >> choice;
//        handleMenuChoice(choice, userManager, emailSender);
//    }

    return MainMenu.exec();

}