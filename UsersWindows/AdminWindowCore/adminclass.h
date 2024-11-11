//
// Created by mzitr on 04.11.2024.
//

#ifndef TASK_MANAGEMENT_ADMINCLASS_H
#define TASK_MANAGEMENT_ADMINCLASS_H

#include <QMainWindow>
#include "BasicClassCore/basicclass.h"

QT_BEGIN_NAMESPACE
namespace Ui { class AdminClass; }
QT_END_NAMESPACE

class AdminClass : public BasicClass {
    Q_OBJECT

public:
    explicit AdminClass(UserManager *userManager,QWidget *parent = nullptr,const std::string& username = "");
    ~AdminClass() override;
//    void displayUserInfo() override;

private:
    std::unique_ptr<Ui::AdminClass> ui;
};


#endif //TASK_MANAGEMENT_ADMINCLASS_H
