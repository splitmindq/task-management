//
// Created by mzitr on 16.11.2024.
//

// You may need to build the project (run Qt uic code generator) to get "ui_EmployeeWindow.h" resolved

#include "employeewindow.h"
#include "ui_EmployeeWindow.h"


EmployeeWindow::EmployeeWindow(UserManager *userManager,QWidget *parent,User *user, std::shared_ptr<Company> company) :
        BasicClass(userManager,parent, user), ui(new Ui::EmployeeWindow),company(company) {
    ui->setupUi(this);
    displayUserInfo();
}

EmployeeWindow::~EmployeeWindow() = default;

void EmployeeWindow::on_LogOutButton_clicked() {
    ui->LogOutButton->setEnabled(false);
    this->close();
    auto *mainWindow = new MainWindow(userManager, nullptr);
    mainWindow ->show();
}

void EmployeeWindow::on_resignButton_clicked() {
    ui->resignButton->setEnabled(false);
    user->companyId=-1;
    user->role="user";
    userManager->saveUser(*user);
    auto userWindow = new UserWindow(userManager, nullptr,user);
    this->close();
    userWindow->show();
}

void EmployeeWindow::displayUserInfo(){
    std::cout<<"";
}