//
// Created by mzitr on 16.11.2024.
//

// You may need to build the project (run Qt uic code generator) to get "ui_EmployeeWindow.h" resolved

#include "employeewindow.h"

#include <utility>
#include "ui_EmployeeWindow.h"

EmployeeWindow::EmployeeWindow(UserManager *userManager, QWidget *parent, User *user, std::shared_ptr<Company> company)
        :
        BasicClass(userManager, parent, user), ui(new Ui::EmployeeWindow), company(std::move(company)) {
    ui->setupUi(this);
    ui->label_3->setVisible(false);
    displayUserInfo();
}

EmployeeWindow::~EmployeeWindow() = default;

void EmployeeWindow::on_LogOutButton_clicked() {
    ui->LogOutButton->setEnabled(false);
    this->close();
    auto *mainWindow = new MainWindow(userManager, nullptr);
    mainWindow->show();
}

void EmployeeWindow::on_resignButton_clicked() {
    ui->resignButton->setEnabled(false);
    user->companyId = -1;
    user->role = "user";
    userManager->saveUser(*user);
    auto userWindow = new UserWindow(userManager, nullptr, user);
    this->close();
    userWindow->show();
}

void EmployeeWindow::displayUserInfo() {
    QString emailText = QString("Email: %1").arg(user->email.c_str());
    ui->emailLineEdit_4->setText(emailText);
    QString nameText = QString("Name: %1").arg(user->name.c_str());
    ui->nameLineEdit_4->setText(nameText);
    QString surnameText = QString("Surname: %1").arg(user->surname.c_str());
    ui->surnameLineEdit_4->setText(surnameText);
}

void EmployeeWindow::on_checkTaskButton_clicked() {
    ui->checkTaskButton->setEnabled(false);
    auto taskWindow = new ToDoWindow(this, user);
    taskWindow->show();
    this->hide();
    ui->checkTaskButton->setEnabled(true);
};

void EmployeeWindow::on_changeInfoButton_clicked() {
    ui->changeInfoButton->setEnabled(false);
    auto changeInfoWindow = new HandleInfoClass(this, user, userManager);
    changeInfoWindow->show();
    ui->changeInfoButton->setEnabled(true);

}
void EmployeeWindow::on_employeeDirectoryBttn_clicked() {
    UserContainer userContainer;
    userContainer.addFilter(UserContainer::filterByCompanyId(user->companyId));
    userContainer.addFilter(UserContainer::filterByRole("admin"));
    userContainer.loadUsersFromDatabase();
    auto user = userContainer.begin();

    ui->label_3->setText(" Name: " + QString::fromStdString(user->name) + ", Email: " + QString::fromStdString(user->email));
    ui->label_3->setVisible(true);
}