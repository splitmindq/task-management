#include "headers/mainwindow.h"
#include "ui_MainWindow.h"
#include "UserManager.h"
#include "QMessageBox"
#include "userWindowCore/userwindow.h"
#include "headers/registrationwindow.h"

MainWindow::MainWindow(UserManager *userManager, QWidget *parent) :

        QMainWindow(parent), ui(new Ui::MainWindow), userManager(userManager) {
    ui->setupUi(this);

}

MainWindow::~MainWindow() = default;

void MainWindow::on_loginButton_clicked()  {

    QString login = ui->loginInput->text();
    QString password = ui->passwordInput->text();

    std::string username = login.toStdString();
    std::string pwd = password.toStdString();


    if (userManager->login(username, pwd)) {

        std::string role = userManager->getRole(username);
        int companyId = userManager->getCompanyId(username);

        if (companyId == -1) {

            userManager->loadUser(username);
            int id = userManager->getId(username);
            User *user = userManager->findUserById(id);
            auto *userWindow = new UserWindow(userManager, nullptr,user);

            this->close();
            userWindow->show();
        }
        else{




        }

    } else {
        QMessageBox::warning(this, "Login", "Invalid username or password!");
    }

}

void MainWindow::on_registrationButton_clicked() {

   auto *registrationWindow = new RegistrationWindow(userManager, nullptr);
    this->close();
    registrationWindow->show();

}