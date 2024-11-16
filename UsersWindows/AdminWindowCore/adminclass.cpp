#include "adminclass.h"
#include "ui_AdminClass.h"
#include <pqxx/pqxx>
#include <iostream>
#include <QScrollBar>

AdminClass::AdminClass(UserManager *userManager, QWidget *parent, User *user, std::shared_ptr<Company> company)
        : BasicClass(userManager, parent, user), ui(new Ui::AdminClass), company(company), currentOffset(0), limit(20) {
    ui->setupUi(this);
    ui->listWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    displayUserInfo();
    setupLazyLoading();
}

AdminClass::~AdminClass() = default;

void AdminClass::displayUserInfo() {
    QString emailText = QString("Email: %1").arg(getEmail(user->username).c_str());
    ui->emailLineEdit->setText(emailText);
    QString nameText = QString("Name: %1").arg(getName(user->username).c_str());
    ui->nameLineEdit->setText(nameText);
    QString surnameText = QString("Surname: %1").arg(getSurname(user->username).c_str());
    ui->surnameLineEdit->setText(surnameText);
}

void AdminClass::on_LogOutButton_clicked() {
    this->close();
    MainWindow *mainWindow = new MainWindow(userManager, nullptr);
    mainWindow->show();
}

void AdminClass::addEmployeeToList(const QString& employeeName, int employeeId) {
    QWidget* employeeWidget = new QWidget();
    QHBoxLayout* layout = new QHBoxLayout();

    QLabel* nameLabel = new QLabel(employeeName, employeeWidget);
    QPushButton* inviteButton = new QPushButton("Invite", employeeWidget);

    connect(inviteButton, &QPushButton::clicked, this, [this, employeeId, inviteButton]() {
        inviteEmployee(employeeId);
        inviteButton->setEnabled(false);
        inviteButton->setText("Invited");
    });

    layout->addWidget(nameLabel);
    layout->addWidget(inviteButton);
    layout->setContentsMargins(0, 0, 0, 0);
    employeeWidget->setLayout(layout);

    QListWidgetItem* item = new QListWidgetItem(ui->listWidget);
    item->setSizeHint(employeeWidget->sizeHint());
    ui->listWidget->setItemWidget(item, employeeWidget);
}

void AdminClass::on_findEmployeesButton_clicked() {
    ui->listWidget->clear();
    currentOffset = 0;
    loadNextEmployees();
}

void AdminClass::onScroll(int value) {
    int maxValue = ui->listWidget->verticalScrollBar()->maximum();
    if (value >= maxValue - 10) {
        loadMoreItems();
    }
}
void AdminClass::loadMoreItems() {
    QList<QPair<QString, int>> moreEmployees =  loadEmployeesFromDatabase(limit, currentOffset);
    for (const auto& employee : moreEmployees) {
        addEmployeeToList(employee.first, employee.second);
    }
}
void AdminClass::setupLazyLoading() {
    connect(ui->listWidget->verticalScrollBar(), &QScrollBar::valueChanged, this, &AdminClass::onScroll);
}

void AdminClass::loadNextEmployees() {
    QList<QPair<QString, int>> employees = loadEmployeesFromDatabase(limit, currentOffset);

    for (const auto& employee : employees) {
        addEmployeeToList(employee.first, employee.second);
    }

    currentOffset += limit;
}
QList<QPair<QString, int>> AdminClass::loadEmployeesFromDatabase(int limit, int offset) {
    QList<QPair<QString, int>> employees;

    try {
        pqxx::connection conn(connectionString);
        if (!conn.is_open()) {
            std::cerr << "Failed to connect to the database!" << std::endl;
            return employees;
        }

        pqxx::work txn(conn);

        txn.conn().prepare("get_employees", "SELECT name, id FROM users WHERE role != $1 LIMIT $2 OFFSET $3");

        pqxx::result res = txn.exec_prepared("get_employees", "admin", limit, offset);

        for (const auto& row : res) {
            QString name = QString::fromStdString(row["name"].c_str());
            int id = row["id"].as<int>();
            employees.append(qMakePair(name, id));
        }

        txn.commit();
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return employees;
}


void AdminClass::inviteEmployee(int employeeId) {

    InviteManager inviteManager(connectionString);

    std::string message = "You were invited to " + company->companyName;
    inviteManager.createInvite(message, user->id, employeeId);

}
