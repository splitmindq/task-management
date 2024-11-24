//
// Created by mzitr on 04.11.2024.
//

#ifndef TASK_MANAGEMENT_BASICCLASS_H
#define TASK_MANAGEMENT_BASICCLASS_H

#include <QMainWindow>
#include <UserManager.h>
#include <pqxx/pqxx>
#include <string>
#include "EmailSender.h"

QT_BEGIN_NAMESPACE
namespace Ui { class BasicClass; }
QT_END_NAMESPACE

class BasicClass : public QMainWindow {
Q_OBJECT

public:
    explicit BasicClass(UserManager *userManager, QWidget *parent = nullptr, User *user = nullptr);

    ~BasicClass() override;

    virtual void displayUserInfo() = 0;


protected:
    UserManager *userManager;
    std::string connectionString = "host=localhost dbname=database user=mzitr password=yourpassword";
    User *user;

    virtual std::string getEmail(const std::string &username);

    virtual std::string getName(const std::string &username);

    virtual std::string getSurname(const std::string &username);
};


#endif //TASK_MANAGEMENT_BASICCLASS_H
