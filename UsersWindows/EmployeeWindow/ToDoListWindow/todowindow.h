#ifndef TASK_MANAGEMENT_TODOWINDOW_H
#define TASK_MANAGEMENT_TODOWINDOW_H

#include <QMainWindow>
#include "QPixmap"
#include "EmployeeWindow/employeewindow.h"
#include "QListWidget"
#include "locale.h"
#include <QLocale>


QT_BEGIN_NAMESPACE
namespace Ui { class ToDoWindow; }
QT_END_NAMESPACE

class ToDoWindow : public QMainWindow {
Q_OBJECT

public:
    explicit ToDoWindow(QWidget *parent = nullptr, User *user = nullptr);

    void addTasksToList(const QString &taskAim, const QString &taskDeadline);

    static std::string timePointToString(const std::chrono::system_clock::time_point &timePoint) ;

    ~ToDoWindow() override;

private slots:

    void on_exitButton_clicked();

    void on_rightButton_clicked();

    void on_leftButton_clicked();


    void on_submitButton_clicked();

private:
    User *user;
    int currentOffset;
    int limit;
    Ui::ToDoWindow *ui;

    void loadMoreItems();

    void setupLazyLoading();

    void onScroll(int value);

    QList<QPair<QString, QString>> loadTasksFromDatabase(int limit, int offset);

    void loadNextTasks();

    void moveItem(QListWidget *sourceList, QListWidget *targetList);

    QList<QPair<QString, QString>> loadTasksFromDatabaseWithStatus(int status, int limit, int offset);

    void loadNextInProcessTasks();

    void addTasksToInProcessList(const QString &taskAim, const QString &taskDeadline);

    void setupLazyLoadingForInProcessList();

    void onInProcessScroll(int value);

    void loadMoreInProcessItems();
    };

#endif //TASK_MANAGEMENT_TODOWINDOW_H
