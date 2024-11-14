//
// Created by mzitr on 04.11.2024.
//

#ifndef TASK_MANAGEMENT_MAINWINDOW_H
#define TASK_MANAGEMENT_MAINWINDOW_H

#include "../UsersWindows/BasicClassCore/basicclass.h"

#include <QMainWindow>
#include "UserManager.h"
#include "../Companies/header/CompanyManager.h"

QT_BEGIN_NAMESPACE
namespace Ui {
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public BasicClass { // Наследуем от BasicClass
Q_OBJECT

public:
    explicit MainWindow(UserManager* userManager, QWidget *parent = nullptr);
    ~MainWindow() override;
    void MainWindow::displayUserInfo() {

    }

private slots:
    void on_loginButton_clicked();
    void on_registrationButton_clicked();

private:
    std::unique_ptr<Ui::MainWindow> ui;
};

#endif // TASK_MANAGEMENT_MAINWINDOW_H