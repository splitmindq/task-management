#include "handleinfoclass.h"
#include "ui_HandleInfoClass.h"
#include <QMessageBox>
#include "EmailSender.h"
#include <random>

HandleInfoClass::HandleInfoClass(QWidget *parent, User* user, UserManager* userManager) :
        QDialog(parent), ui(new Ui::HandleInfoClass), user(user), userManager(userManager) {  // Используем QDialog
    ui->setupUi(this);
    ui->codeLineEdit->setVisible(false);
    ui->checkCodeButton->setVisible(false);
    ui->codeLabel->setVisible(false);
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
}

HandleInfoClass::~HandleInfoClass() {
    delete ui;
}

std::string HandleInfoClass::generateVerificationCode() {
    std::string code = std::to_string(rand() % 1000000);
    while (code.length() < 6) {
        code = "0" + code;
    }
    return code;
}

void HandleInfoClass::on_confirmButton_clicked() {
    EmailSender emailSender("<emailsender11@mail.ru>", "smtps://smtp.mail.ru:465",
                            "emailsender11@mail.ru", "2bcQFBxtjmwUWdP7jxpT", "Kge6zBXU5SFsMnNsSkfk");

    ui->confirmButton->setEnabled(false);

    if (!ui->passwordEdit->text().isEmpty()) {
        std::string oldPassword = user->password;
        if (ui->passwordEdit->text().toStdString() == oldPassword) {
            QMessageBox::warning(this, "Error", "Fields cannot be the same.");
            ui->confirmButton->setEnabled(true);
            return;
        } else {
            ui->codeLineEdit->setVisible(true);
            ui->checkCodeButton->setVisible(true);
            ui->codeLabel->setVisible(true);

            verificationCode = generateVerificationCode();
            std::string message = "Your code: " + verificationCode;
            emailSender.sendEmail(user->email, "Password Changing", message);
            ui->confirmButton->setVisible(false);
        }
    }
}

void HandleInfoClass::on_checkCodeButton_clicked() {
    QString enteredCode = ui->codeLineEdit->text();
    if (enteredCode.toStdString() == verificationCode) {
        if(!ui->nameEdit->text().isEmpty()){
            user->name = ui->nameEdit->text().toStdString();
        }
        if(!ui->surnameEdit->text().isEmpty()){
            user->surname = ui->surnameEdit->text().toStdString();
        }
        if(!ui->passwordEdit->text().isEmpty()){
            user->password = ui->passwordEdit->text().toStdString();
        }
        userManager->saveUser(*user);
        QMessageBox::information(this, "Success", "Information updated!");

        QWidget* parentWindow = parentWidget();
        if (parentWindow) {
            parentWindow->close();
        } else {
            qDebug() << "Parent window is nullptr!";
        }

        auto mainWindow = new MainWindow(userManager, nullptr);
        mainWindow->show();
        this->close();
    } else {
        attempts++;
        if (attempts >= 3) {
            QMessageBox::warning(this, "Error", "Too many failed attempts. Exiting.");
            close();
        } else {
            QMessageBox::warning(this, "Error", "Incorrect verification code. Try again.");
        }
    }
}

void HandleInfoClass::on_backToMenuButton_clicked() {
    this->close();
}
