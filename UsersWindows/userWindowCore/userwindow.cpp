#include "userwindow.h"
#include "ui_userwindow.h"
#include "UserManager.h"
#include "headers/mainwindow.h"
#include <QString>
#include "../menu/headers/companyregwindow.h"

UserWindow::UserWindow(UserManager* userManager, QWidget *parent, User *user)
        : BasicClass(userManager, parent,user), ui(new Ui::UserWindow) {

    ui->setupUi(this);
    displayUserInfo();
}

void UserWindow::displayUserInfo() {
    QString emailText = QString("Email: %1").arg(getEmail(user->username).c_str());
    ui->emailLineEdit->setText(emailText);
    QString nameText = QString("Name: %1").arg(getName(user->username).c_str());
    ui->nameLineEdit->setText(nameText);
    QString surnameText = QString("Surname: %1").arg(getSurname(user->username).c_str());
    ui->surnameLineEdit->setText(surnameText);
}

UserWindow::~UserWindow() = default;

void UserWindow::on_LogOutButton_clicked() {
    this->close();
    MainWindow *mainWindow = new MainWindow(userManager, nullptr);
    mainWindow->show();
}

void UserWindow::onCompanyNameEntered(const QString &companyName) {

    qDebug() << "Company name " << companyName;
    CompanyManager companyManager(userManager,companyName.toStdString(),userManager->getId(user->username),connectionString);
    auto company = companyManager.getCompany();
    user->companyId = company->id;
    userManager->saveUser(*user);
    auto *adminWindow = new AdminClass(userManager, nullptr,user,company);
    adminWindow ->show();

}

void UserWindow::on_createCompanyButton_clicked() {
    auto *regWindow = new companyRegWindow(this);
    connect(regWindow, &companyRegWindow::companyNameEntered, this, &UserWindow::onCompanyNameEntered);
    regWindow->exec();


}