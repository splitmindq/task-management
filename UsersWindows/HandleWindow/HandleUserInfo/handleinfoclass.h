#ifndef TASK_MANAGEMENT_HANDLEINFOCLASS_H
#define TASK_MANAGEMENT_HANDLEINFOCLASS_H

#include <QDialog>
#include "UserManager.h"
#include "headers/mainwindow.h"
#include "userWindowCore/userwindow.h"
#include "EmployeeWindow/employeewindow.h"
#include "AdminWindowCore/adminclass.h"
#include <QMessageBox>
#include "EmailSender.h"
#include <random>
#include <iomanip>
#include <sstream>
#include <QDebug>
#include <ctime>
QT_BEGIN_NAMESPACE
namespace Ui { class HandleInfoClass; }
QT_END_NAMESPACE

class HandleInfoClass : public QDialog {
Q_OBJECT

public:
    explicit HandleInfoClass(QWidget *parent = nullptr, User *user = nullptr, UserManager *userManager = nullptr);

    ~HandleInfoClass() override;

private slots:

    void on_confirmButton_clicked();

    void on_checkCodeButton_clicked();

    static std::string generateVerificationCode();

    void on_backToMenuButton_clicked();



private:
    Ui::HandleInfoClass *ui;
    User *user;
    UserManager *userManager;
    std::string verificationCode;
    int attempts = 0;

    bool isPasswordValid() const;

    void updateUserInfo();
};

#endif //TASK_MANAGEMENT_HANDLEINFOCLASS_H
