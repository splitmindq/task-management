#include "userwindow.h"
#include "ui_userwindow.h"
#include "UserManager.h"
#include "headers/mainwindow.h"
#include <QString>
#include "../menu/headers/companyregwindow.h"

UserWindow::UserWindow(UserManager* userManager, QWidget *parent, User *user)
        : BasicClass(userManager, parent,user), ui(new Ui::UserWindow),currentOffset(0), limit(20){
    ui->setupUi(this);
    displayUserInfo();
}

UserWindow::~UserWindow() = default;

void UserWindow::clearInviteForUser(int inviterId) {
    try {
        pqxx::connection C(connectionString);
        pqxx::work W(C);
        W.exec("DELETE FROM invites WHERE user_id = " + W.quote(user->id) +
            "AND sender_id = " + W.quote(inviterId)
        );
        W.commit();
        std::cout << "Invites deleted for user ID: " << user->id << std::endl;
    } catch (const pqxx::sql_error &e) {
        std::cerr << "SQL error: " << e.what() << std::endl;
        std::cerr << "Query was: " << e.query() << std::endl;
    } catch (const std::system_error &e) {
        std::cerr << "Error while loading invite: " << e.what() << std::endl;
    }

}

void UserWindow::acceptInvite(int inviterId) {
    std::cout<<inviterId;
    auto inviter = userManager->findUserById(inviterId);
    int companyId = inviter->companyId;
    user->role="employee";
    user->companyId = companyId;
    userManager->saveUser(*user);
    CompanyManager companyManager(userManager, connectionString);
    auto company = companyManager.findCompanyByAdminId(inviterId);
    auto employeeWindow = new EmployeeWindow(userManager, nullptr,user,company);
    employeeWindow->show();
    clearInviteForUser(inviterId);
    this -> close();

}

void UserWindow::addInviteToList(const QString& senderName, const QString& message) {
    QWidget* inviteWidget = new QWidget();
    QHBoxLayout* layout = new QHBoxLayout();

    QLabel* nameLabel = new QLabel(senderName, inviteWidget);
    QPushButton* acceptButton = new QPushButton("Accept", inviteWidget);
    QLabel* messageLabel = new QLabel(message,inviteWidget);
    connect(acceptButton, &QPushButton::clicked, this, [this, senderName]() {

        int inviterId = userManager->getId(senderName.toStdString());
        userManager->loadUser(senderName.toStdString());
        acceptInvite(inviterId);

    });

    layout->addWidget(nameLabel);
    layout->addWidget(messageLabel);
    layout->addWidget(acceptButton);
    layout->setContentsMargins(0, 0, 0, 0);
    inviteWidget->setLayout(layout);

    QListWidgetItem* item = new QListWidgetItem(ui->listWidget);
    item->setSizeHint(inviteWidget->sizeHint());
    ui->listWidget->setItemWidget(item, inviteWidget);
}

void UserWindow::displayUserInfo() {
    QString emailText = QString("Email: %1").arg(user->email.c_str());
    ui->emailLineEdit->setText(emailText);
    QString nameText = QString("Name: %1").arg(user->name.c_str());
    ui->nameLineEdit->setText(nameText);
    QString surnameText = QString("Surname: %1").arg(user->surname.c_str());
    ui->surnameLineEdit->setText(surnameText);
}

void UserWindow::on_LogOutButton_clicked() {
    this->close();
    MainWindow *mainWindow = new MainWindow(userManager, nullptr);
    mainWindow->show();
}

void UserWindow::on_checkInvitesButton_clicked() {
    ui->listWidget->clear();
    currentOffset = 0;
    loadNextInvites();
}QList<QPair<QString, QString>> UserWindow::loadInvitesFromDatabase(int limit, int offset) {
    QList<QPair<QString, QString>> invites;
    QSet<QString> seenSenders;

    try {
        pqxx::connection conn(connectionString);
        pqxx::work txn(conn);

        pqxx::result res = txn.exec_params(
                R"(
            SELECT u.username AS sender, i.message
            FROM invites i
            JOIN users u ON i.sender_id = u.id
            WHERE i.user_id = $1
            LIMIT $2 OFFSET $3
            )",
                user->id, limit, offset);

        for (const auto& row : res) {
            QString sender = QString::fromStdString(row["sender"].c_str());
            QString message = QString::fromStdString(row["message"].c_str());

            if (!seenSenders.contains(sender)) {
                invites.append(qMakePair(sender, message));
                seenSenders.insert(sender);
            }
        }

        txn.commit();
    } catch (const std::exception& e) {
        std::cerr << "Error loading invites: " << e.what() << std::endl;
    }

    return invites;
}

void UserWindow::loadNextInvites() {
    QList<QPair<QString, QString>> invites = loadInvitesFromDatabase(limit, currentOffset);

    for (const auto& invite : invites) {
        addInviteToList(invite.first, invite.second);
    }

    currentOffset += limit;
}

void UserWindow::onCompanyNameEntered(const QString &companyName) {
    CompanyManager companyManager(userManager, connectionString);
    companyManager.createCompany(companyName.toStdString(), user->id);
    auto company = companyManager.getCompany();
    user->companyId = company->getCompanyId();
    user->role = "admin";
    userManager->updateUserInDb(*user);
    auto *adminWindow = new AdminClass(userManager, nullptr, user, company);
    adminWindow->show();
    this->close();
}

void UserWindow::on_createCompanyButton_clicked() {
    auto *regWindow = new companyRegWindow(this);
    connect(regWindow, &companyRegWindow::companyNameEntered, this, &UserWindow::onCompanyNameEntered);
    regWindow->exec();
}

void UserWindow::on_changeInfoButton_clicked() {
    ui->changeInfoButton->setEnabled(false);
    auto changeInfoWindow = new HandleInfoClass(this, user, userManager);
    changeInfoWindow->show();
    ui->changeInfoButton->setEnabled(true);


}