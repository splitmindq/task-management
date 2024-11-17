#include "headers/mainwindow.h"
#include "ui_MainWindow.h"
#include "UserManager.h"
#include "QMessageBox"
#include "userWindowCore/userwindow.h"
#include "headers/registrationwindow.h"
#include "tinyxml2.h"
#include "filesystem"

MainWindow::MainWindow(UserManager *userManager, QWidget *parent) :

        QMainWindow(parent), ui(new Ui::MainWindow), userManager(userManager) {
    ui->setupUi(this);

}

MainWindow::~MainWindow() = default;

void MainWindow::on_loginButton_clicked() {
    ui->loginButton->setEnabled(false);
    QString login = ui->loginInput->text();
    QString password = ui->passwordInput->text();

    std::string username = login.toStdString();
    std::string pwd = password.toStdString();

    try {
        if (userManager->login(username, pwd)) {
            std::string role = userManager->getRole(username);
            int companyId = userManager->getCompanyId(username);
            userManager->loadUser(username);
            int id = userManager->getId(username);
            User *user = userManager->findUserById(id);

            if (companyId == -1) {
                auto *userWindow = new UserWindow(userManager, nullptr, user);
                this->close();
                userWindow->show();
            } else {
                if (user->role == "admin") {
                    CompanyManager companyManager(userManager, connectionString);
                    std::shared_ptr<Company> company = companyManager.findCompanyByAdminId(user->id);

                    if (company) {
                        auto *adminWindow = new AdminClass(userManager, nullptr, user, company);
                        adminWindow->show();
                        this->close();
                    } else {
                        throw LoginException("Компания не найдена для данного администратора.");
                    }
                }
            }
        } else {
            ui->loginButton->setEnabled(true);
            throw LoginException("Неверный логин или пароль!");
        }
    }
    catch (const LoginException& e) {
        QMessageBox::warning(this, "Login", QString::fromStdString(e.what()));
    }

    catch (const std::exception& e) {
        QMessageBox::critical(this, "Error", QString("Unexpected error: %1").arg(e.what()));
    }
}

void MainWindow::on_registrationButton_clicked() {
    ui->registrationButton->setEnabled(false);
   auto *registrationWindow = new RegistrationWindow(userManager, nullptr);
    this->close();
    registrationWindow->show();

}