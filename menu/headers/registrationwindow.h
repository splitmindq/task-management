//
// Created by mzitr on 03.11.2024.
//

#ifndef TASK_MANAGEMENT_REGISTRATIONWINDOW_H
#define TASK_MANAGEMENT_REGISTRATIONWINDOW_H

#include <QMainWindow>
#include "headers/mainwindow.h"
#include "UserManager.h"
#include "QMessageBox"
QT_BEGIN_NAMESPACE
namespace Ui { class RegistrationWindow; }
QT_END_NAMESPACE

class RegistrationWindow : public QMainWindow {
Q_OBJECT

public:
    explicit RegistrationWindow(UserManager *userManager,QWidget *parent = nullptr);

    ~RegistrationWindow() override;

private slots:
    void on_backToMenuButton_clicked();
    void on_createAccountButton_clicked();

private:
    std::unique_ptr<Ui::RegistrationWindow> ui;
    UserManager *userManager;
};


#endif //TASK_MANAGEMENT_REGISTRATIONWINDOW_H
