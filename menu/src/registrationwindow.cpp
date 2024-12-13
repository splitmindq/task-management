//
// Created by mzitr on 03.11.2024.
//

// You may need to build the project (run Qt uic code generator) to get "ui_RegistrationWindow.h" resolved

#include "headers/registrationwindow.h"
#include "ui_RegistrationWindow.h"
#include "UserManager.h"

RegistrationWindow::RegistrationWindow(UserManager *userManager, QWidget *parent) :
        QMainWindow(parent), ui(new Ui::RegistrationWindow), userManager(userManager) {
    ui->setupUi(this);
    setWindowTitle("Task Management");

}

RegistrationWindow::~RegistrationWindow() = default;

void RegistrationWindow::on_backToMenuButton_clicked() {
    ui->backToMenuButton->setEnabled(false);
    this->close();

}

void RegistrationWindow::on_createAccountButton_clicked() {
    ui->createAccountButton->setEnabled(false);
    std::string name = ui->nameEdit->text().toStdString();
    std::string surname = ui->surnameEdit->text().toStdString();
    std::string email = ui->emailEdit->text().toStdString();
    std::string login = ui->loginEdit->text().toStdString();
    std::string password = ui->passwordEdit->text().toStdString();

    if (login.empty() || name.empty() || surname.empty() || email.empty() || password.empty()) {
        QMessageBox::warning(this, "Error", "Please fill out all fields.");
        ui->createAccountButton->setEnabled(true);
        return;
    }


    bool isUsernameTaken = userManager->isUsernameTaken(login);
    if (isUsernameTaken) {
        QMessageBox::warning(this, "Error", "Username is already taken. Please choose another.");
        ui->loginEdit->clear();
        ui->loginEdit->setFocus();
        return;
    }

    userManager->createUser(name, surname, email, login, password);

    QMessageBox::information(this, "Registration", "You have successfully registered!");

    on_backToMenuButton_clicked();

}
