#include "../headers/userwindow.h"
#include "ui_userwindow.h"
#include "UserManager.h"
#include "../../menu/headers/mainwindow.h"
#include <QString>
#include <pqxx/pqxx>

UserWindow::UserWindow(UserManager* userManager, QWidget *parent, const std::string& username)
        : QMainWindow(parent), ui(new Ui::UserWindow), userManager(userManager), username(username) {
    ui->setupUi(this);

    QString emailText = QString("Email: %1").arg(getEmail(username).c_str());
    ui->emailLineEdit->setText(emailText);
    QString nameText = QString("Name: %1").arg(getName(username).c_str());
    ui->nameLineEdit->setText(nameText);
    QString surnameText = QString("surname: %1").arg(getName(username).c_str());
    ui->surnameLineEdit->setText(surnameText);
}

std::string UserWindow::getEmail(const std::string &username) {
    try {
        pqxx::connection C("host=localhost dbname=database user=mzitr password=yourpassword");
        pqxx::work W(C);

        pqxx::result R = W.exec("SELECT email FROM users WHERE username = " + W.quote(username));
        if (!R.empty()) {
            return R[0][0].as<std::string>();
        } else {
            return "";
        }
    } catch (const std::system_error &e) {
        std::cerr << e.what() << std::endl;
        return "";
    }
}

std::string UserWindow::getName(const std::string &username) {
    try {
        pqxx::connection C("host=localhost dbname=database user=mzitr password=yourpassword");
        pqxx::work W(C);

        pqxx::result R = W.exec("SELECT name FROM users WHERE username = " + W.quote(username));
        if (!R.empty()) {
            return R[0][0].as<std::string>();
        } else {
            return "";
        }
    } catch (const std::system_error &e) {
        std::cerr << e.what() << std::endl;
        return "";
    }
}

std::string UserWindow::getSurname(const std::string &username) {
    try {
        pqxx::connection C("host=localhost dbname=database user=mzitr password=yourpassword");
        pqxx::work W(C);

        pqxx::result R = W.exec("SELECT surname FROM users WHERE username = " + W.quote(username));
        if (!R.empty()) {
            return R[0][0].as<std::string>();
        } else {
            return "";
        }
    } catch (const std::system_error &e) {
        std::cerr << e.what() << std::endl;
        return "";
    }
}

UserWindow::~UserWindow() {
    delete ui;
}

void UserWindow::on_LogOutButton_clicked() {
    this->close();

    MainWindow *mainWindow = new MainWindow(userManager, nullptr);
    mainWindow->show();
}