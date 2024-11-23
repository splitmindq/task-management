#ifndef TASK_MANAGEMENT_CHANGEINFOWINDOW_H
#define TASK_MANAGEMENT_CHANGEINFOWINDOW_H

#include <QWidget>
#include "UserManager.h"
#include "../menu/headers/mainwindow.h"
QT_BEGIN_NAMESPACE
namespace Ui { class ChangeInfoWindow; }
QT_END_NAMESPACE

class ChangeInfoWindow : public QWidget {
Q_OBJECT

public:
    explicit ChangeInfoWindow(QWidget *parent = nullptr, User* user = nullptr, UserManager* userManager = nullptr);
    ~ChangeInfoWindow() override;

private slots:
    void on_confirmButton_clicked();
    void on_checkCodeButton_clicked();
    std::string generateVerificationCode();

private:
    Ui::ChangeInfoWindow *ui;
    User *user;
    UserManager* userManager;
    std::string verificationCode;
    int attempts = 0;
};

#endif //TASK_MANAGEMENT_CHANGEINFOWINDOW_H