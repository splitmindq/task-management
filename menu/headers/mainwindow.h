#ifndef TASK_MANAGEMENT_MAINWINDOW_H
#define TASK_MANAGEMENT_MAINWINDOW_H

#include <QMainWindow>
#include "UserManager.h"


QT_BEGIN_NAMESPACE
namespace Ui {
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
Q_OBJECT

public:
    explicit MainWindow(UserManager* userManager, QWidget *parent = nullptr);
    ~MainWindow() ;

private slots:
    void on_loginButton_clicked();
    void on_registrationButton_clicked();


private:
    std::unique_ptr<Ui::MainWindow> ui;
    UserManager *userManager;
};

#endif // TASK_MANAGEMENT_MAINWINDOW_H
