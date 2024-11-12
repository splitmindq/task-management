//
// Created by mzitr on 04.11.2024.
//

#ifndef TASK_MANAGEMENT_ADMINCLASS_H
#define TASK_MANAGEMENT_ADMINCLASS_H

#include <QMainWindow>
#include "BasicClassCore/basicclass.h"
#include "../Companies/header/CompanyManager.h"
QT_BEGIN_NAMESPACE
namespace Ui { class AdminClass; }
QT_END_NAMESPACE

class AdminClass : public BasicClass {
    Q_OBJECT

public:
    explicit AdminClass(UserManager *userManager,QWidget *parent = nullptr,User *user = nullptr,std::shared_ptr<Company> company = nullptr);
    ~AdminClass() override;
    void displayUserInfo() override;

private:
    std::unique_ptr<Ui::AdminClass> ui;
    UserManager *userManager;
    User *user;
    std::shared_ptr<Company> company;

};


#endif //TASK_MANAGEMENT_ADMINCLASS_H
