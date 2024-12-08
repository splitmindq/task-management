#ifndef TASK_MANAGEMENT_ADMINCLASS_H
#define TASK_MANAGEMENT_ADMINCLASS_H

#include <QMainWindow>
#include "BasicClassCore/basicclass.h"
#include "../Companies/header/CompanyManager.h"
#include "../menu/headers/mainwindow.h"
#include "../Invites/headers/InviteManager.h"
#include "../ItemsIterator/header/ItemsIterator.h"
#include "../ItemContainer/header/ItemContainer.h"
#include "HandleWindow/HandleUserInfo/handleinfoclass.h"
#include "HandleWindow/HandleCompanyInfoCore/handlecompanyinfo.h"
#include "../Task/header/TaskManager.h"
#include "../Task/TaskWindow/taskwindow.h"
#include "QTimer"
QT_BEGIN_NAMESPACE
namespace Ui { class AdminClass; }
QT_END_NAMESPACE

class AdminClass : public BasicClass {
Q_OBJECT

public:
    explicit AdminClass(UserManager *userManager, QWidget *parent = nullptr, User *user = nullptr,
                        std::shared_ptr<Company> company = nullptr);

    ~AdminClass() override;

    void displayUserInfo() override;

    void addEmployeeToList(const QString &employeeName, int employeeId);

    void inviteEmployee(int employeeId);

    void giveTask(int employeeId, QPushButton *addTaskButton);

    void addCompanyEmployeeToList(const QString &employeeName, int employeeId);

private slots:

    void on_LogOutButton_clicked();

    void on_findEmployeesButton_clicked();

    static void on_docDownloadButton_clicked();

    void onScroll(int value);

    void loadMoreItems();

    void on_changeInfoButton_clicked();

    void on_modifyCompanyButton_clicked();

    void on_giveTask_clicked();

private:
    int currentOffset;
    int limit;
    std::unique_ptr<Ui::AdminClass> ui;
    std::shared_ptr<Company> company;

    void setupLazyLoading();

    void loadNextEmployees();
    void loadNextCompanyEmployees();
    static QList<QPair<QString, int>> loadEmployeesFromDatabase(int limit, int offset);
    QList<QPair<QString, int>> loadCompanyEmployeesFromDatabase(int limit, int offset);

};

#endif //TASK_MANAGEMENT_ADMINCLASS_H