//
// Created by mzitr on 11.11.2024.
//

#ifndef TASK_MANAGEMENT_COMPANYREGWINDOW_H
#define TASK_MANAGEMENT_COMPANYREGWINDOW_H

#include <QDialog>
#include "QMessageBox"
#include "../UsersWindows/userWindowCore/userwindow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class companyRegWindow; }
QT_END_NAMESPACE

class companyRegWindow : public QDialog {
Q_OBJECT

public:
    explicit companyRegWindow(QWidget *parent = nullptr);

    ~companyRegWindow() override;

signals:
    void companyNameEntered(const QString &companyName);

private slots:
    void on_createButton_clicked();


private:
    Ui::companyRegWindow *ui;
};


#endif //TASK_MANAGEMENT_COMPANYREGWINDOW_H
