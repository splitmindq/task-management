//
// Created by mzitr on 16.11.2024.
//

// You may need to build the project (run Qt uic code generator) to get "ui_EmployeeWindow.h" resolved

#include "employeewindow.h"
#include "ui_EmployeeWindow.h"


EmployeeWindow::EmployeeWindow(QWidget *parent) :
        BasicClass(nullptr,parent, nullptr), ui(new Ui::EmployeeWindow) {
    ui->setupUi(this);
    displayUserInfo();
}

EmployeeWindow::~EmployeeWindow() {
    delete ui;
}
