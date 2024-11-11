//
// Created by mzitr on 30.10.2024.
//

#ifndef TASK_MANAGEMENT_USERWINDOW_H
#define TASK_MANAGEMENT_USERWINDOW_H
#include "BasicClassCore/basicclass.h"
#include <QMainWindow>
#include "UserManager.h"
#include "ui_userwindow.h"
#include "memory"
#include "../Companies/header/CompanyManager.h"
QT_BEGIN_NAMESPACE
namespace Ui { class UserWindow; }
QT_END_NAMESPACE

class UserWindow : public BasicClass {
Q_OBJECT

public:
    explicit UserWindow(UserManager* userManager, QWidget *parent = nullptr, const std::string& username = "");
    ~UserWindow() override;
    void displayUserInfo() override;

private slots:
    void on_LogOutButton_clicked();
    void onCompanyNameEntered(const QString &companyName);
    void on_createCompanyButton_clicked();
private:
    std::unique_ptr<Ui::UserWindow> ui;


};


#endif //TASK_MANAGEMENT_USERWINDOW_H
