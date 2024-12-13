#include "sendingEmail.h"
#include "headers/mainwindow.h"
#include "QApplication"
#include "QScreen"
int main(int argc, char *argv[]) {

    std::string connectionString = "host=localhost dbname=database user=mzitr password=yourpassword";
    UserManager userManager(connectionString);

    SetConsoleOutputCP(CP_UTF8);

    QApplication MainMenu(argc, argv);

    MainWindow window(&userManager);
    return QApplication::exec();

}
#include <iostream>
#include <direct.h> // для _mkdir

