#include "handleinfoclass.h"
#include "ui_HandleInfoClass.h"


HandleInfoClass::HandleInfoClass(QWidget *parent, User *user, UserManager *userManager) :
        QDialog(parent), ui(new Ui::HandleInfoClass), user(user), userManager(userManager), attempts(0) {
    ui->setupUi(this);
    ui->codeLineEdit->setVisible(false);
    ui->checkCodeButton->setVisible(false);
    ui->codeLabel->setVisible(false);
    setWindowTitle("Task Management");

}

HandleInfoClass::~HandleInfoClass() {
    delete ui;
}

std::string HandleInfoClass::generateVerificationCode() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 999999);
    std::ostringstream oss;
    oss << std::setw(6) << std::setfill('0') << dis(gen);
    return oss.str();
}

bool HandleInfoClass::isPasswordValid() const {
    return ui->passwordEdit->text().toStdString() != user->password;
}

void HandleInfoClass::updateUserInfo() {
    if (!ui->nameEdit->text().isEmpty()) {
        user->name = ui->nameEdit->text().toStdString();
    }
    if (!ui->surnameEdit->text().isEmpty()) {
        user->surname = ui->surnameEdit->text().toStdString();
    }
    if (!ui->passwordEdit->text().isEmpty()) {
        user->password = ui->passwordEdit->text().toStdString();
    }
    userManager->saveUser(*user);
}

void HandleInfoClass::on_confirmButton_clicked() {
    EmailSender emailSender("<emailsender11@mail.ru>", "smtps://smtp.mail.ru:465",
                            "emailsender11@mail.ru", "EiQ6CgicPRbKHnESunqJ", "BvVX14M6PVfV6TcxxFre");

    ui->confirmButton->setEnabled(false);

    bool isAnyFieldChanged = !ui->nameEdit->text().isEmpty() && ui->nameEdit->text().toStdString() != user->name ||
                             !ui->surnameEdit->text().isEmpty() && ui->surnameEdit->text().toStdString() != user->surname ||
                             !ui->passwordEdit->text().isEmpty() && ui->passwordEdit->text().toStdString() != user->password;

    if (!isAnyFieldChanged) {
        QMessageBox::warning(this, "Error", "No changes detected. Please modify at least one field.");
        ui->confirmButton->setEnabled(true);
        return;
    }



    verificationCode = generateVerificationCode();
    std::string message = "Your code: " + verificationCode;

    try {
        if (!emailSender.sendEmail(user->email, "Profile information Changing", message)) {
            QMessageBox::warning(this, "Error", "Failed to send verification email. Please check internet connection.");
            ui->confirmButton->setEnabled(true);
            return;
        }
    } catch (const std::exception &e) {
        QMessageBox::warning(this, "Error", QString("An error occurred: %1").arg(e.what()));
        ui->confirmButton->setEnabled(true);
        return;
    }
    ui->codeLineEdit->setVisible(true);
    ui->checkCodeButton->setVisible(true);
    ui->codeLabel->setVisible(true);
    ui->confirmButton->setVisible(false);
}


void HandleInfoClass::on_checkCodeButton_clicked() {
    QString enteredCode = ui->codeLineEdit->text();
    if (enteredCode.toStdString() == verificationCode) {
        updateUserInfo();
        QMessageBox::information(this, "Success", "Information updated!");

        QWidget *parentWindow = parentWidget();
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
        const int maxAttempts = 3;
        if (attempts >= maxAttempts) {
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
