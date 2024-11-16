//
// Created by mzitr on 30.10.2024.
//

#ifndef TASK_MANAGEMENT_USERWINDOW_H
#define TASK_MANAGEMENT_USERWINDOW_H
#include "BasicClassCore/basicclass.h"
#include <QMainWindow>
#include "UserManager.h"
#include "ui_userwindow.h"
#include "memory"
#include "../Companies/header/CompanyManager.h"
#include "../AdminWindowCore/adminclass.h"

QT_BEGIN_NAMESPACE
namespace Ui { class UserWindow; }
QT_END_NAMESPACE

class UserWindow : public BasicClass {
Q_OBJECT

public:
    explicit UserWindow(UserManager* userManager, QWidget *parent = nullptr, User *user=nullptr);
    ~UserWindow() override;
    void displayUserInfo() override;

private slots:
    void on_LogOutButton_clicked();
    void onCompanyNameEntered(const QString &companyName);
    void on_createCompanyButton_clicked();
    void on_checkInvitesButton_clicked();
private:
    std::unique_ptr<Ui::UserWindow> ui;
    int currentOffset;
    int limit;
    QList<QPair<QString, QString>> loadInvitesFromDatabase(int limit, int offset);
    void loadNextInvites();
    void addInviteToList(const QString& senderName,const QString& message);
    void acceptInvite();
};



#endif //TASK_MANAGEMENT_USERWINDOW_H
