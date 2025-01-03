#include "adminclass.h"
#include "ui_AdminClass.h"
#include <pqxx/pqxx>
#include <iostream>
#include <QScrollBar>
#include <utility>

AdminClass::AdminClass(UserManager *userManager, QWidget *parent, User *user, std::shared_ptr<Company> company)
        : BasicClass(userManager, parent, user), ui(new Ui::AdminClass), company(std::move(company)), currentOffset(0), limit(20) {
    ui->setupUi(this);
    ui->listWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    ui->statusLabel->setVisible(false);
    displayUserInfo();
    setupLazyLoading();
    setWindowTitle("Task Management");
    showMaximized();

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
    ui->LogOutButton->setEnabled(false);
    this->close();
    auto *mainWindow = new MainWindow(userManager, nullptr);
    mainWindow->show();
}


void AdminClass::updateInviteButtonState(QPushButton* inviteButton, int employeeId) {
    try {
        pqxx::connection C(connectionString);
        pqxx::work W(C);
        pqxx::result R = W.exec_params("SELECT id FROM invites WHERE user_id = $1 AND sender_id = $2 LIMIT 1",employeeId,user->id);
        if (R.empty()) {
            inviteButton->setText("Invite");
            inviteButton->setEnabled(true);
        } else {
            inviteButton->setText("Invited");
            inviteButton->setStyleSheet("background-color: #4444444f");
            inviteButton->setEnabled(false);
        }
    } catch (const std::system_error &e) {
        std::cerr << "Failed to check invite status: " << e.what() << std::endl;
        inviteButton->setText("Error");
        inviteButton->setEnabled(false);
    }
}

void AdminClass::addEmployeeToList(const QString &employeeName, int employeeId) {
    auto *employeeWidget = new QWidget();
    auto *layout = new QHBoxLayout(employeeWidget);
    auto *nameLabel = new QLabel(employeeName, employeeWidget);
    auto *inviteButton = new QPushButton(employeeWidget);

    updateInviteButtonState(inviteButton, employeeId);

    connect(inviteButton, &QPushButton::clicked, this, [this, employeeId, inviteButton]() {
        inviteEmployee(employeeId);
        inviteButton->setEnabled(false);
        inviteButton->setText("Invited");
    });

    layout->addWidget(nameLabel);
    layout->addWidget(inviteButton);
    layout->setContentsMargins(0, 0, 0, 0);
    employeeWidget->setLayout(layout);

    auto *item = new QListWidgetItem(ui->listWidget);
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
    QList<QPair<QString, int>> moreEmployees = loadEmployeesFromDatabase(limit, currentOffset);
    for (const auto &employee: moreEmployees) {
        addEmployeeToList(employee.first, employee.second);
    }
}

void AdminClass::setupLazyLoading() {
    connect(ui->listWidget->verticalScrollBar(), &QScrollBar::valueChanged, this, &AdminClass::onScroll);
}

void AdminClass::loadNextEmployees() {
    QList<QPair<QString, int>> employees = loadEmployeesFromDatabase(limit, currentOffset);

    for (const auto &employee: employees) {
        addEmployeeToList(employee.first, employee.second);
    }

    currentOffset += limit;
}

QList<QPair<QString, int>> AdminClass::loadEmployeesFromDatabase(int limit, int offset) {
    QList<QPair<QString, int>> employees;

    UserContainer unemployedUsers;
    unemployedUsers.addFilter(UserContainer::filterByRole("user"));
    unemployedUsers.addFilter(UserContainer::filterByLimit(limit));
    unemployedUsers.addFilter(UserContainer::filterByOffset(offset));
    unemployedUsers.loadUsersFromDatabase();
    for (const auto& emp: unemployedUsers) {
        QString name = QString::fromStdString(emp.name);
        employees.append(qMakePair(name, emp.id));
    }
    return employees;
}


void AdminClass::inviteEmployee(int employeeId) {

    InviteManager inviteManager(connectionString);

    std::string message = "You were invited to " + company->getName();
    inviteManager.createInvite(message, user->id, employeeId);

}

void AdminClass::on_docDownloadButton_clicked() {

    UserContainer container;
    container.addFilter(UserContainer::filterByRole("user"));
    container.loadUsersFromDatabase();
    XmlHandler xmlHandler;
    std::string filename = "users.xml";
    xmlHandler.clearRootContent(filename);
    xmlHandler.createXmlFile(filename, "UsersList");
    for (auto it = container.begin(); it != container.end(); ++it) {
        xmlHandler.addUserToXml(filename, &(*it));

    }
}

void AdminClass::on_changeInfoButton_clicked() {
    ui->changeInfoButton->setEnabled(false);
    auto changeInfoWindow = new HandleInfoClass(this, user, userManager);

    QRect parentGeometry = this->geometry();

    changeInfoWindow->resize(parentGeometry.size());
    changeInfoWindow->showMaximized();

    ui->changeInfoButton->setEnabled(true);
}

void AdminClass::on_modifyCompanyButton_clicked() {
    ui->modifyCompanyButton->setEnabled(false);
    auto handleCompanyWindow = new HandleCompanyInfo(this, company, user, userManager);
    QRect parentGeometry = this->geometry();
    connect(handleCompanyWindow, &HandleCompanyInfo::companyInfoUpdated, this, &AdminClass::refreshCompanyInfo);
    handleCompanyWindow->resize(parentGeometry.size());

    handleCompanyWindow->showMaximized();
    ui->modifyCompanyButton->setEnabled(true);


}

void AdminClass::refreshCompanyInfo() {
    ui->listWidget->clear();
    currentOffset = 0;
    loadNextCompanyEmployees();
}

QList<QPair<QString, int>> AdminClass::loadCompanyEmployeesFromDatabase(int limit, int offset) {
    QList<QPair<QString, int>> employees;

    UserContainer employeeContainer;
    employeeContainer.addFilter(UserContainer::filterByCompanyId(user->companyId));
    employeeContainer.addFilter(UserContainer::filterByRole("employee"));
    employeeContainer.addFilter(UserContainer::filterByLimit(limit));
    employeeContainer.addFilter(UserContainer::filterByOffset(offset));
    employeeContainer.loadUsersFromDatabase();

    for (auto it = employeeContainer.begin(); it != employeeContainer.end(); ++it) {

        QString name = QString::fromStdString(it->name);
        employees.append(qMakePair(name, it->id));
    }

    return employees;
}

void AdminClass::addCompanyEmployeeToList(const QString &employeeName, int employeeId) {
    auto *employeeWidget = new QWidget();
    auto *layout = new QHBoxLayout();

    auto *nameLabel = new QLabel(employeeName, employeeWidget);
    auto *addTaskButton = new QPushButton("Add Task", employeeWidget);

    connect(addTaskButton, &QPushButton::clicked, this,[this, employeeId, addTaskButton]() {
        giveTask(employeeId, addTaskButton);
    });

    layout->addWidget(nameLabel);
    layout->addWidget(addTaskButton);
    layout->setContentsMargins(0, 0, 0, 0);
    employeeWidget->setLayout(layout);

    auto *item = new QListWidgetItem(ui->listWidget);
    item->setSizeHint(employeeWidget->sizeHint());
    ui->listWidget->setItemWidget(item, employeeWidget);
}


void AdminClass::loadNextCompanyEmployees() {

    QList<QPair<QString, int>> employees = loadCompanyEmployeesFromDatabase(limit, currentOffset);

    for (const auto &employee: employees) {
        addCompanyEmployeeToList(employee.first, employee.second);
    }

    currentOffset += limit;

}

void AdminClass::giveTask(int employeeId, QPushButton *addTaskButton) {

    TaskWindow dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        QString taskDescription = dialog.getTaskDescription();
        QDateTime deadline = dialog.getDeadline();
        auto deadlineTime = std::chrono::system_clock::from_time_t(deadline.toSecsSinceEpoch());
        TaskManager taskManager(connectionString);
        taskManager.createTask(employeeId, user->companyId, taskDescription.toStdString(), deadlineTime);
        ui->statusLabel->setText("Task successfully created!");
        ui->statusLabel->setStyleSheet("color: green;");
        ui->statusLabel->setVisible(true);
        addTaskButton->setEnabled(false);
        QTimer::singleShot(3000, this, [this, addTaskButton]() {
            ui->statusLabel->setVisible(false);
            addTaskButton->setEnabled(true);
        });


    }
}


void AdminClass::on_giveTask_clicked() {
    ui->listWidget->clear();
    currentOffset = 0;
    loadNextCompanyEmployees();

}