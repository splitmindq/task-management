//
// Created by mzitr on 11.11.2024.
//

// You may need to build the project (run Qt uic code generator) to get "ui_companyRegWindow.h" resolved

#include "headers/companyregwindow.h"
#include "ui_companyRegWindow.h"


companyRegWindow::companyRegWindow(QWidget *parent) :
        QDialog(parent), ui(new Ui::companyRegWindow) {
    ui->setupUi(this);


}

companyRegWindow::~companyRegWindow() {
    delete ui;
}

void companyRegWindow::on_createButton_clicked() {
    ui->createButton->setEnabled(false);

    QString companyName = ui->nameEdit->text();
    if (companyName.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Название компании не может быть пустым.");
    } else {
        emit companyNameEntered(ui->nameEdit->text());
    }
    QMessageBox::information(this,"Создание компании","Компания создана");
    this->close();
}