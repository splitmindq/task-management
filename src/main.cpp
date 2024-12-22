#include "sendingEmail.h"
#include "headers/mainwindow.h"
#include "QApplication"
#include "QScreen"
int main(int argc, char *argv[]) {

    std::string connectionString = "host=localhost dbname=database user=mzitr password=yourpassword";
    UserManager userManager(connectionString);

    QApplication MainMenu(argc, argv);

    MainWindow window(&userManager);
    return QApplication::exec();
}
