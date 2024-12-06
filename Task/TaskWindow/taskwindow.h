//
// Created by mzitr on 30.11.2024.
//

#ifndef TASK_MANAGEMENT_TASKWINDOW_H
#define TASK_MANAGEMENT_TASKWINDOW_H


#include <QDialog>
#include <QDateTimeEdit>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QFormLayout>
#include <QSpinBox>
#include <QMessageBox>



QT_BEGIN_NAMESPACE
namespace Ui { class TaskWindow; }
QT_END_NAMESPACE

class TaskWindow : public QDialog {
Q_OBJECT

public:
    explicit TaskWindow(QWidget *parent = nullptr);

    QString getTaskDescription() const;
    QDateTime getDeadline() const;

    ~TaskWindow() override;

private:
    Ui::TaskWindow *ui;


private slots:
    void onSubmit();
};


#endif //TASK_MANAGEMENT_TASKWINDOW_H
