//
// Created by mzitr on 24.11.2024.
//

// You may need to build the project (run Qt uic code generator) to get "ui_HandleCompanyInfo.h" resolved

#include "handlecompanyinfo.h"
#include "ui_HandleCompanyInfo.h"

HandleCompanyInfo::HandleCompanyInfo(QWidget *parent, std::shared_ptr<Company> company, User *user,
                                     UserManager *userManager) :
        QDialog(parent), ui(new Ui::HandleCompanyInfo), company(company), user(user), userManager(userManager),
        currentOffset(0), limit(20) {
    ui->setupUi(this);
    QString placeholder = "Company Name: " + QString::fromStdString(company->getName());
    ui->lineEdit->setPlaceholderText(placeholder);
    ui->listWidget->clear();
    currentOffset = 0;

    ui->listWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    setupLazyLoading();
    loadMoreItems();

}

void HandleCompanyInfo::loadMoreItems() {
    QList<QPair<QString, int>> moreEmployees = loadEmployeesFromDatabase(limit, currentOffset);
    for (const auto &employee: moreEmployees) {
        addEmployeeToList(employee.first, employee.second);
    }
    currentOffset += moreEmployees.size();
}

void HandleCompanyInfo::onScroll(int value) {

    int maxValue = ui->listWidget->verticalScrollBar()->maximum();
    if (value >= maxValue - 10) {
        loadMoreItems();
    }
}

void HandleCompanyInfo::setupLazyLoading() {
    connect(ui->listWidget->verticalScrollBar(), &QScrollBar::valueChanged, this, &HandleCompanyInfo::onScroll);
}

HandleCompanyInfo::~HandleCompanyInfo() {
    delete ui;
}

void HandleCompanyInfo::on_returnButton_clicked() {

    this->close();

}

void HandleCompanyInfo::on_saveChangesButton_clicked() {

    if (!ui->lineEdit->text().isEmpty()) {
        company->setName(ui->lineEdit->text().toStdString());
        CompanyManager companyManager(userManager, "host=localhost dbname=database user=mzitr password=yourpassword");
        companyManager.saveCompanyToDb(company);
        QMessageBox::information(this, "Name", "Name was updated successfully");

    } else {
        QMessageBox::warning(this, "Name", "Name field cannot be empty!");
    }

}

void HandleCompanyInfo::fireEmployee(int employeeId) {

    userManager->loadUserById(employeeId);
    auto employee = userManager->findUserById(employeeId);

    employee->companyId = -1;
    employee->role = "user";
    userManager->saveUser(*employee);

    ui->listWidget->clear();
    currentOffset = 0;
    setupLazyLoading();
    loadMoreItems();
}


void HandleCompanyInfo::addEmployeeToList(const QString &employeeName, int employeeId) {
    auto *employeeWidget = new QWidget();
    auto *layout = new QHBoxLayout();

    std::string connStr = "host=localhost dbname=database user=mzitr password=yourpassword";

    TaskManager taskManager(connStr);
    int overdueTasks = taskManager.getOverdueTaskCount(employeeId);
    QString statText = QString("Overdue tasks: %1").arg(overdueTasks);

    auto *nameLabel = new QLabel(employeeName, employeeWidget);
    auto *statisticLabel = new QLabel(statText,employeeWidget);
    auto *fireButton = new QPushButton("fire", employeeWidget);

    connect(fireButton, &QPushButton::clicked, this, [this, employeeId, fireButton]() {
        std::cout << "Button clicked for employee ID: " << employeeId << std::endl;
        fireButton->setEnabled(false);
        fireButton->setText("Fired");
        fireEmployee(employeeId);
    });

    layout->addWidget(nameLabel);
    layout->addWidget(statisticLabel);
    layout->addWidget(fireButton);
    layout->setContentsMargins(0, 0, 0, 0);
    employeeWidget->setLayout(layout);

    auto *item = new QListWidgetItem(ui->listWidget);
    item->setSizeHint(employeeWidget->sizeHint());
    ui->listWidget->setItemWidget(item, employeeWidget);
}

QList<QPair<QString, int>> HandleCompanyInfo::loadEmployeesFromDatabase(int limit, int offset) {
    QList<QPair<QString, int>> employees;
    try {
        pqxx::connection conn("host=localhost dbname=database user=mzitr password=yourpassword");
        if (!conn.is_open()) {
            std::cerr << "Failed to connect to the database!" << std::endl;
            return employees;
        }

        pqxx::work txn(conn);

        txn.conn().prepare("get_employees",
                           "SELECT name, id FROM users WHERE companyId = $1 AND role != $4 LIMIT $2 OFFSET $3");

        pqxx::result res = txn.exec_prepared("get_employees", user->companyId, limit, offset, "admin");
        for (const auto &row: res) {
            QString name = QString::fromStdString(row["name"].c_str());
            int id = row["id"].as<int>();
            employees.append(qMakePair(name, id));

        }

        txn.commit();
    }
    catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return employees;
}

void HandleCompanyInfo::on_deleteCompanyButton_clicked() {

    UserContainer employees;
    employees.addFilter(employees.filterByCompanyId(user->companyId));
    employees.loadUsersFromDatabase();

    for (auto it = employees.begin(); it != employees.end(); ++it) {

        it->companyId = -1;
        it->role = "user";
        userManager->saveUser(*it);

    }

    CompanyManager companyManager(userManager, "host=localhost dbname=database user=mzitr password=yourpassword");
    companyManager.deleteCompany(std::to_string(company->getCompanyId()));

    try {
        pqxx::connection C("host=localhost dbname=database user=mzitr password=yourpassword");
        pqxx::work W(C);
        W.exec("DELETE FROM invites WHERE sender_id = " + W.quote(user->id));
        W.commit();
    } catch (const pqxx::sql_error &e) {
        std::cerr << "SQL error: " << e.what() << std::endl;
        std::cerr << "Query was: " << e.query() << std::endl;
    } catch (const std::system_error &e) {
        std::cerr << "Error while loading invite: " << e.what() << std::endl;
    }

    QWidget *parentWindow = parentWidget();
    if (parentWindow) {
        parentWindow->close();
    } else {
        qDebug() << "Parent window is nullptr!";
    }

    this->close();
    auto *mainWindow = new MainWindow(userManager, nullptr);
    mainWindow->show();

}

