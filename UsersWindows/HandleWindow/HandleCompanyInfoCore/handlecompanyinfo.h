//
// Created by mzitr on 24.11.2024.
//

#ifndef TASK_MANAGEMENT_HANDLECOMPANYINFO_H
#define TASK_MANAGEMENT_HANDLECOMPANYINFO_H

#include <QDialog>
#include "../Companies/header/CompanyManager.h"
#include "UserManager.h"
#include "QScrollBar"
#include "Windows.h"
#include "../Companies/header/CompanyManager.h"
#include "../ItemContainer/header/ItemContainer.h"
#include "headers/mainwindow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class HandleCompanyInfo; }
QT_END_NAMESPACE

class HandleCompanyInfo : public QDialog {
Q_OBJECT

public:
    explicit HandleCompanyInfo(QWidget *parent = nullptr, std::shared_ptr<Company> company = nullptr,
                               User *user = nullptr, UserManager *userManager = nullptr);

    ~HandleCompanyInfo() override;

    void addEmployeeToList(const QString &employeeName, int employeeId);

    void onScroll(int value);

    bool isValidEmail(const QString &email);


private slots:

    void on_returnButton_clicked();

    void on_saveChangesButton_clicked();

    void on_deleteCompanyButton_clicked();

private:
    Ui::HandleCompanyInfo *ui;
    std::shared_ptr<Company> company;
    User *user;
    UserManager *userManager;

    void loadMoreItems();

    int currentOffset;
    int limit;

    void fireEmployee(int employeeId);

    void setupLazyLoading();

    void loadNextEmployees();

    QList<QPair<QString, int>> loadEmployeesFromDatabase(int limit, int offset);
};


#endif //TASK_MANAGEMENT_HANDLECOMPANYINFO_H
