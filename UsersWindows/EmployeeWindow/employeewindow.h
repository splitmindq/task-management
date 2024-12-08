//
// Created by mzitr on 16.11.2024.
//

#ifndef TASK_MANAGEMENT_EMPLOYEEWINDOW_H
#define TASK_MANAGEMENT_EMPLOYEEWINDOW_H

#include <QMainWindow>
#include "BasicClassCore/basicclass.h"
#include "../Companies/header/CompanyManager.h"
#include "../menu/headers/mainwindow.h"
#include "../userWindowCore/userwindow.h"
#include "HandleWindow/HandleUserInfo/handleinfoclass.h"
#include "../EmployeeWindow/ToDoListWindow/todowindow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class EmployeeWindow; }
QT_END_NAMESPACE

class EmployeeWindow : public BasicClass {
Q_OBJECT

public:
    explicit EmployeeWindow(UserManager *userManager, QWidget *parent = nullptr, User *user = nullptr,
                            std::shared_ptr<Company> company = nullptr);

    void displayUserInfo() override;

    ~EmployeeWindow() override;

private slots:

    void on_LogOutButton_clicked();

    void on_resignButton_clicked();

    void on_changeInfoButton_clicked();

    void on_checkTaskButton_clicked();

    void on_employeeDirectoryBttn_clicked();

private:
    std::unique_ptr<Ui::EmployeeWindow> ui;
    std::shared_ptr<Company> company;

};

#endif //TASK_MANAGEMENT_EMPLOYEEWINDOW_H
