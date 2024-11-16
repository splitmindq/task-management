//
// Created by mzitr on 16.11.2024.
//

#ifndef TASK_MANAGEMENT_EMPLOYEEWINDOW_H
#define TASK_MANAGEMENT_EMPLOYEEWINDOW_H

#include <QMainWindow>
#include "BasicClassCore/basicclass.h"

QT_BEGIN_NAMESPACE
namespace Ui { class EmployeeWindow; }
QT_END_NAMESPACE

class EmployeeWindow : public BasicClass{
Q_OBJECT

public:
    explicit EmployeeWindow(QWidget *parent = nullptr);

    ~EmployeeWindow() override;

private:
    Ui::EmployeeWindow *ui;
};


#endif //TASK_MANAGEMENT_EMPLOYEEWINDOW_H
