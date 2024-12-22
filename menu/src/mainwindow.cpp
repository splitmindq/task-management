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
    setWindowTitle("Task Management");
    showMaximized();

}


MainWindow::~MainWindow() = default;


void MainWindow::on_loginButton_clicked() {
    ui->loginButton->setEnabled(false);
    QString login = ui->loginInput->text();
    QString password = ui->passwordInput->text();

        std::string username = login.toStdString();
    std::string pwd = password.toStdString();

    try {
        if (!userManager->login(username, pwd)) {
            ui->loginButton->setEnabled(true);
            throw LoginException("Invalid username or password!");
        }

        handleUserRole(username);
    } catch (const LoginException &e) {
        QMessageBox::warning(this, "Login", QString::fromStdString(e.what()));
        ui->loginButton->setEnabled(true);
    } catch (const std::exception &e) {
        QMessageBox::critical(this, "Error", QString("Unexpected error: %1").arg(e.what()));
        ui->loginButton->setEnabled(true);
    }
}

void MainWindow::handleUserRole(const std::string &username) {
    std::string role = userManager->getRole(username);
    int companyId = userManager->getCompanyId(username);
    userManager->loadUser(username);
    int id = userManager->getId(username);
    User *user = userManager->findUserById(id);

    if (companyId == -1) {
        openUserWindow(user);
    } else {
        handleAdminOrEmployee(user);
    }
}

void MainWindow::openUserWindow(User *user) {
    auto *userWindow = new UserWindow(userManager, nullptr, user);
    userWindow->show();
    QTimer::singleShot(100, this, &QWidget::close);
}

void MainWindow::handleAdminOrEmployee(User *user) {
    if (user->role == "admin") {
        handleAdmin(user);
    } else {
        openEmployeeWindow(user);
    }
}

void MainWindow::handleAdmin(User *user) {
    CompanyManager companyManager(userManager, connectionString);
    std::shared_ptr<Company> company = companyManager.findCompanyByAdminId(user->id);

    if (company) {
        auto adminWindow = new AdminClass(userManager, nullptr, user, company);
        adminWindow->show();
        QTimer::singleShot(100, this, &QWidget::close);
    } else {
        throw LoginException("Company not found for this administrator.");
    }
}

void MainWindow::openEmployeeWindow(User *user) {
    CompanyManager companyManager(userManager, connectionString);
    std::shared_ptr<Company> company = companyManager.findCompanyById(user->companyId);

    if (company) {
        auto employeeWindow = new EmployeeWindow(userManager, nullptr, user, company);
        employeeWindow->show();
        QTimer::singleShot(100, this, &QWidget::close);
    } else {
        throw LoginException("Company not found for this user.");
    }
}

void MainWindow::on_registrationButton_clicked() {
    ui->registrationButton->setEnabled(false);
    auto registrationWindow = new RegistrationWindow(userManager, nullptr);
    registrationWindow->showMaximized();
    ui->registrationButton->setEnabled(true);

}