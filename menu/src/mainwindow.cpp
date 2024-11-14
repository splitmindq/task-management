#include "headers/mainwindow.h"
#include "ui_MainWindow.h"
#include "UserManager.h"
#include "QMessageBox"
#include "userWindowCore/userwindow.h"
#include "headers/registrationwindow.h"
MainWindow::MainWindow(UserManager *userManager, QWidget *parent)
                        : BasicClass(nullptr,parent, nullptr),ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    displayUserInfo();

}
MainWindow::~MainWindow() = default;

void MainWindow::on_registrationButton_clicked() {

    auto *registrationWindow = new RegistrationWindow(userManager, nullptr);
    this->close();
    registrationWindow->show();
}
void MainWindow::on_loginButton_clicked() {

    UserManager userMng(connectionString);
    QString login = ui->loginInput->text();
    QString password = ui->passwordInput->text();

    std::string username = login.toStdString();
    std::string pwd = password.toStdString();

    if (userMng.login(username, pwd)) {

        std::string role = userMng.getRole(username);
        int companyId = userMng.getCompanyId(username);
        userMng.loadUser(username);
        int id = userMng.getId(username);
        User *user = userMng.findUserById(id);

        if (!user) {
            QMessageBox::warning(this, "Login", "User not found.");
            return;
        }

        if (companyId == -1) {
            auto *userWindow = new UserWindow(&userMng, nullptr, user);
            this->close();
            userWindow->show();
        } else {
            if (userMng.isAdmin(user->id)) {
                CompanyManager companyManager(&userMng, connectionString);
                std::shared_ptr<Company> company = companyManager.findCompanyByAdminId(user->id);

                if (company) {
                    this->close();
                    auto *adminWindow = new AdminClass(&userMng, nullptr, user, company);
                    adminWindow->show();
                } else {
                    QMessageBox::warning(this, "Login", "Компания не найдена для данного администратора.");
                }
            }
        }
    }

}

