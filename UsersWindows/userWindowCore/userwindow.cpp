#include "userwindow.h"
#include "ui_userwindow.h"
#include "UserManager.h"
#include "headers/mainwindow.h"
#include <QString>
#include "../menu/headers/companyregwindow.h"

UserWindow::UserWindow(UserManager* userManager, QWidget *parent, User *user)
        : BasicClass(userManager, parent,user), ui(new Ui::UserWindow){
    ui->setupUi(this);
    displayUserInfo();
}

UserWindow::~UserWindow() = default;

void UserWindow::acceptInvite() {
    // Логика для принятия приглашения
}

void UserWindow::addInviteToList(const QString& senderName, const QString& message) {
    QWidget* inviteWidget = new QWidget();
    QHBoxLayout* layout = new QHBoxLayout();

    QLabel* nameLabel = new QLabel(senderName, inviteWidget);
    QPushButton* acceptButton = new QPushButton("Accept", inviteWidget);

    connect(acceptButton, &QPushButton::clicked, this, [this, senderName]() {
        acceptInvite();
    });

    layout->addWidget(nameLabel);
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
}

QList<QPair<QString, QString>> UserWindow::loadInvitesFromDatabase(int limit, int offset) {
    QList<QPair<QString, QString>> invites;

    try {
        pqxx::connection conn(connectionString);
        pqxx::work txn(conn);

        pqxx::result res = txn.exec_params(
                R"(
            SELECT u.username AS sender, i.message
            FROM invites i
            JOIN users u ON i.sender_id = u.id
            WHERE i.recipient_id = )" + std::to_string(user->id) +
                R"( LIMIT )" + std::to_string(limit) +
                R"( OFFSET )" + std::to_string(offset));

        for (const auto& row : res) {
            QString sender = QString::fromStdString(row["sender"].c_str());
            QString message = QString::fromStdString(row["message"].c_str());
            invites.append(qMakePair(sender, message));
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
    user->companyId = company->id;
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