//
// Created by mzitr on 30.10.2024.
//

#ifndef TASK_MANAGEMENT_USERWINDOW_H
#define TASK_MANAGEMENT_USERWINDOW_H

#include <QMainWindow>
#include "UserManager.h"

QT_BEGIN_NAMESPACE
namespace Ui { class UserWindow; }
QT_END_NAMESPACE

class UserWindow : public QMainWindow {
Q_OBJECT

public:
    explicit UserWindow(UserManager* userManager, QWidget *parent = nullptr, const std::string& username = "");;

    ~UserWindow() override;

private slots:
    void on_LogOutButton_clicked();

private:
    Ui::UserWindow *ui;
    UserManager *userManager;
    std::string username;
    std::string getEmail(const std::string& username);
    std::string getName(const std::string& name);
    std::string getSurname(const std::string& surname);
};


#endif //TASK_MANAGEMENT_USERWINDOW_H
