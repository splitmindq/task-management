//
// Created by mzitr on 30.11.2024.
//

// You may need to build the project (run Qt uic code generator) to get "ui_TaskWindow.h" resolved

#include "taskwindow.h"
#include "ui_TaskWindow.h"

TaskWindow::TaskWindow(QWidget *parent) :
        QDialog(parent), ui(new Ui::TaskWindow) {
    ui->setupUi(this);

    ui->dateEdit->setCalendarPopup(true);
    ui->dateEdit->setMinimumDate(QDate::currentDate());
    connect(ui->pushButton, &QPushButton::clicked, this, &TaskWindow::onSubmit);
    ui->label->setVisible(false);
    setWindowTitle("Enter Task and Deadline");
}

TaskWindow::~TaskWindow() {
    delete ui;
}

QString TaskWindow::getTaskDescription() const {
    return ui->lineEdit->text();
}

QDateTime TaskWindow::getDeadline() const {
    return ui->dateEdit->dateTime();
}

void TaskWindow::onSubmit() {
    if (ui->lineEdit->text().isEmpty()) {
        ui->label->setText("The task description cannot be empty.");
        ui->label->setVisible(true);
    } else {
        ui->label->setVisible(false);
        accept();
    }
}
