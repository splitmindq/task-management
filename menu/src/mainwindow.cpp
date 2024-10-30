#include "../headers/mainwindow.h"
#include "ui_MainWindow.h"
#include "UserManager.h"
#include "QMessageBox"
#include "../../UsersWindows/headers/userwindow.h"

MainWindow::MainWindow(UserManager *userManager, QWidget *parent) :
        QMainWindow(parent), ui(new Ui::MainWindow), userManager(userManager) {
    ui->setupUi(this);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_loginButton_clicked() {
    QString login = ui->loginInput->text();
    QString password = ui->passwordInput->text();

    std::string username = login.toStdString();
    std::string pwd = password.toStdString();

    if (userManager->login(username, pwd)) {
        QMessageBox::information(this, "Login", "Login successful!");

        std::string role = userManager->getRole(username);
        int companyId = userManager->getCompanyId(username);

        if (companyId == -1) {
            auto *userWindow = new UserWindow(userManager, nullptr,username);  // Указываем `this` как родителяWindow->show();
            this->close();
            userWindow->show();
        }
    } else {
        QMessageBox::warning(this, "Login", "Invalid username or password!");
    }
}
