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

companyRegWindow::~companyRegWindow() = default;

void companyRegWindow::on_createButton_clicked() {
    ui->createButton->setEnabled(false);

    QString companyName = ui->nameEdit->text();
    if (companyName.isEmpty()) {
        QMessageBox::warning(this, "Error", "Name field cannot be empty.");
    } else {
        emit companyNameEntered(ui->nameEdit->text());
    }
    QMessageBox::information(this,"Company","Company Created");
    this->close();
}

void companyRegWindow::on_returnButton_clicked(){
    ui->returnButton->setEnabled(false);
    this->close();

}