#include "todowindow.h"
#include "ui_ToDoWindow.h"
#include <QDebug>

ToDoWindow::ToDoWindow(QWidget *parent, User *user) :
        QMainWindow(parent), user(user), ui(new Ui::ToDoWindow), currentOffset(0), limit(20) {
    ui->setupUi(this);
    ui->loadList->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    ui->inProcessList->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    setupLazyLoading();
    setupLazyLoadingForInProcessList();
    loadNextTasks();
    loadNextInProcessTasks();
}

void ToDoWindow::addTasksToList(const QString &taskAim, const QString &taskDeadline) {
    auto *taskWidget = new QWidget();
    auto *layout = new QHBoxLayout(taskWidget);

    auto *aimLabel = new QLabel(taskAim, taskWidget);
    auto *deadLineLabel = new QLabel(taskDeadline, taskWidget);

    QDate deadLineDate = QDate::fromString(taskDeadline, "dd.MM.yyyy");
    if (deadLineDate < QDate::currentDate()) {
        aimLabel->setStyleSheet("color: red; font-weight: bold;");
        QMessageBox::warning(this, "Внимание", "Некоторые задачи имеют истекший дедлайн.");
    }

    layout->addWidget(aimLabel);
    layout->addWidget(deadLineLabel);
    layout->setContentsMargins(0, 0, 0, 0);
    taskWidget->setLayout(layout);

    auto *item = new QListWidgetItem(ui->loadList);
    item->setSizeHint(taskWidget->sizeHint());
    ui->loadList->setItemWidget(item, taskWidget);
}

void ToDoWindow::on_exitButton_clicked() {
    auto employeeWindow = qobject_cast<EmployeeWindow *>(this->parent());
    this->close();
    employeeWindow->show();
}

ToDoWindow::~ToDoWindow() {
    delete ui;
}

void ToDoWindow::onScroll(int value) {
    int maxValue = ui->loadList->verticalScrollBar()->maximum();
    if (value >= maxValue - 10) {
        loadMoreItems();
    }
}

std::string timePointToString(const std::chrono::system_clock::time_point &timePoint) {
    std::time_t timeT = std::chrono::system_clock::to_time_t(timePoint);
    std::tm localTime{};
#ifdef _WIN32
    localtime_s(&localTime, &timeT);
#else
    localtime_r(&timeT, &localTime);
#endif

    std::ostringstream oss;
    oss << std::setfill('0') << std::setw(2) << localTime.tm_mday << "."
        << std::setfill('0') << std::setw(2) << (localTime.tm_mon + 1) << "."
        << (localTime.tm_year + 1900);

    return oss.str();
}

QList<QPair<QString, QString>> ToDoWindow::loadTasksFromDatabase(int limit, int offset) {
    QList<QPair<QString, QString>> tasks;

    TaskContainer taskContainer;
    taskContainer.addFilter(TaskContainer::filterTasksByUserId(user->id));
    taskContainer.addFilter(TaskContainer::filterTasksByStatus(0));
    taskContainer.loadTasksFromDatabase();

    for (const auto &emp: taskContainer) {
        QString aim = QString::fromStdString(emp.getAim());
        QString deadline = QString::fromStdString(timePointToString(emp.getDeadline()));
        tasks.append(qMakePair(aim, deadline));
    }
    return tasks;
}

void ToDoWindow::setupLazyLoading() {
    connect(ui->loadList->verticalScrollBar(), &QScrollBar::valueChanged, this, &ToDoWindow::onScroll);
}

void ToDoWindow::loadMoreItems() {
    QList<QPair<QString, QString>> moreTasks = loadTasksFromDatabase(limit, currentOffset);
    for (const auto &task: moreTasks) {
        addTasksToList(task.first, task.second);
    }
    currentOffset += limit;
}

void ToDoWindow::loadNextTasks() {
    QList<QPair<QString, QString>> tasks = loadTasksFromDatabase(limit, currentOffset);
    for (const auto &task: tasks) {
        addTasksToList(task.first, task.second);
    }
    currentOffset += limit;
}

QListWidgetItem* findSelectedItem(const QList<QListWidget*>& lists) {
    for (QListWidget* list : lists) {
        QList<QListWidgetItem*> selectedItems = list->selectedItems();
        if (!selectedItems.isEmpty()) {
            return selectedItems.first();
        }
    }
    return nullptr;
}

void ToDoWindow::moveItem(QListWidget *sourceList, QListWidget *targetList) {
    QListWidgetItem *item = sourceList->currentItem();
    if (!item) {
        qDebug() << "Ни один элемент не выбран. Операция переноса отменена.";
        return;
    }

    QWidget *taskWidget = sourceList->itemWidget(item);
    if (!taskWidget) return;

    QList<QLabel *> labels;
    QLayout *layout = taskWidget->layout();
    if (layout) {
        for (int i = 0; i < layout->count(); ++i) {
            QWidget *widget = layout->itemAt(i)->widget();
            if (QLabel *label = qobject_cast<QLabel *>(widget)) {
                labels.append(label);
                qDebug() << "Найден QLabel с текстом: " << label->text();
            }
        }
    }

    if (labels.size() < 2) {
        qDebug() << "Недостаточное количество QLabel для операции.";
        return;
    }

    QString taskAim = labels[0]->text();
    QString taskDeadline = labels[1]->text();

    auto *newTaskWidget = new QWidget();
    auto *newLayout = new QHBoxLayout(newTaskWidget);

    auto *aimLabel = new QLabel(taskAim, newTaskWidget);
    auto *deadLineLabel = new QLabel(taskDeadline, newTaskWidget);

    QDate deadLineDate = QDate::fromString(taskDeadline, "dd.MM.yyyy");
    if (deadLineDate < QDate::currentDate()) {
        aimLabel->setStyleSheet("color: red; font-weight: bold;");
    }

    newLayout->addWidget(aimLabel);
    newLayout->addWidget(deadLineLabel);
    newLayout->setContentsMargins(0, 0, 0, 0);
    newTaskWidget->setLayout(newLayout);

    auto *newItem = new QListWidgetItem(targetList);
    newItem->setSizeHint(newTaskWidget->sizeHint());
    targetList->setItemWidget(newItem, newTaskWidget);

    sourceList->removeItemWidget(item);
    delete item;

    ui->loadList->clearSelection();
    ui->inProcessList->clearSelection();
    ui->doneList->clearSelection();

    targetList->setCurrentItem(newItem);
    sourceList->setCurrentItem(nullptr);
}

void ToDoWindow::on_leftButton_clicked() {
    if (ui->loadList->currentItem()) {
        moveItem(ui->loadList, ui->inProcessList);
    } else if (ui->inProcessList->currentItem()) {
        moveItem(ui->inProcessList, ui->doneList);
    } else if (ui->doneList->currentItem()) {
        qDebug() << "Элементы из списка 'Выполнено' не перемещаются.";
    } else {
        qDebug() << "Ни один элемент не выбран. Операция переноса отменена.";
    }

    ui->loadList->setCurrentItem(nullptr);
    ui->inProcessList->setCurrentItem(nullptr);
    ui->doneList->setCurrentItem(nullptr);
}

void ToDoWindow::on_rightButton_clicked() {
    ui->loadList->clearSelection();
    ui->inProcessList->clearSelection();
    ui->doneList->clearSelection();

    if (ui->inProcessList->currentItem()) {
        moveItem(ui->inProcessList, ui->loadList);
    } else if (ui->doneList->currentItem()) {
        moveItem(ui->doneList, ui->inProcessList);
    } else {
        qDebug() << "Ни один элемент не выбран. Операция переноса отменена.";
    }

    ui->loadList->setCurrentItem(nullptr);
    ui->inProcessList->setCurrentItem(nullptr);
    ui->doneList->setCurrentItem(nullptr);
}

void ToDoWindow::on_submitButton_clicked() {
    QLocale::setDefault(QLocale(QLocale::Russian, QLocale::Russia));

    if (setlocale(LC_ALL, "ru_RU.UTF-8") == nullptr) {
        std::cerr << "Could not set locale to Russian." << std::endl;
        return;
    }

    QStringList aimsDone;
    int itemCountDone = ui->doneList->count();
    for (int i = 0; i < itemCountDone; ++i) {
        QListWidgetItem *itemDone = ui->doneList->item(i);
        QWidget *taskWidgetDone = ui->doneList->itemWidget(itemDone);
        if (taskWidgetDone) {
            QList<QLabel *> labelsDone;
            QLayout *layoutDone = taskWidgetDone->layout();
            if (layoutDone) {
                for (int j = 0; j < layoutDone->count(); ++j) {
                    QWidget *widget = layoutDone->itemAt(j)->widget();
                    if (QLabel *label = qobject_cast<QLabel *>(widget)) {
                        labelsDone.append(label);
                    }
                }
            }
            if (!labelsDone.isEmpty()) {
                QString taskAim = labelsDone[0]->text();
                aimsDone.append(taskAim);
            }
        }
    }

    QStringList aimsInProgress;
    int itemCountInProgress = ui->inProcessList->count();
    for (int i = 0; i < itemCountInProgress; ++i) {
        QListWidgetItem *itemInProgress = ui->inProcessList->item(i);
        QWidget *taskWidgetInProgress = ui->inProcessList->itemWidget(itemInProgress);
        if (taskWidgetInProgress) {
            QList<QLabel *> labelsInProgress;
            QLayout *layoutInProgress = taskWidgetInProgress->layout();
            if (layoutInProgress) {
                for (int j = 0; j < layoutInProgress->count(); ++j) {
                    QWidget *widget = layoutInProgress->itemAt(j)->widget();
                    if (QLabel *label = qobject_cast<QLabel *>(widget)) {
                        labelsInProgress.append(label);
                    }
                }
            }
            if (!labelsInProgress.isEmpty()) {
                QString taskAim = labelsInProgress[0]->text();
                aimsInProgress.append(taskAim);
            }
        }
    }

    QStringList aimsLoading;
    int itemCountLoading = ui->loadList->count();
    for (int i = 0; i < itemCountLoading; ++i) {
        QListWidgetItem *itemLoading = ui->loadList->item(i);
        QWidget *taskWidgetLoading = ui->loadList->itemWidget(itemLoading);
        if (taskWidgetLoading) {
            QList<QLabel *> labelsLoading;
            QLayout *layoutLoading = taskWidgetLoading->layout();
            if (layoutLoading) {
                for (int j = 0; j < layoutLoading->count(); ++j) {
                    QWidget *widget = layoutLoading->itemAt(j)->widget();
                    if (QLabel *label = qobject_cast<QLabel *>(widget)) {
                        labelsLoading.append(label);
                    }
                }
            }
            if (!labelsLoading.isEmpty()) {
                QString taskAim = labelsLoading[0]->text();
                aimsLoading.append(taskAim);
            }
        }
    }

    TaskManager taskManager("host=localhost dbname=database user=mzitr password=yourpassword");

    for (const QString &aim : aimsDone) {
        std::string aimStr = aim.toStdString();
        std::shared_ptr<Task> task = taskManager.getTaskByAim(aimStr);
        if (task) {
            task->setStatus(2);
            taskManager.updateTaskStatus(*task);
        }
    }

    for (const QString &aim : aimsInProgress) {
        std::string aimStr = aim.toStdString();
        std::shared_ptr<Task> task = taskManager.getTaskByAim(aimStr);
        if (task) {
            task->setStatus(1);
            taskManager.updateTaskStatus(*task);
        }
    }

    for (const QString &aim : aimsLoading) {
        std::string aimStr = aim.toStdString();
        std::shared_ptr<Task> task = taskManager.getTaskByAim(aimStr);
        if (task) {
            task->setStatus(0);
            taskManager.updateTaskStatus(*task);
        }
    }
}

QList<QPair<QString, QString>> ToDoWindow::loadTasksFromDatabaseWithStatus(int status, int limit, int offset) {
    QList<QPair<QString, QString>> tasks;
    TaskContainer taskContainer;
    taskContainer.addFilter(TaskContainer::filterTasksByUserId(user->id));
    taskContainer.addFilter(TaskContainer::filterTasksByStatus(status));
    taskContainer.loadTasksFromDatabase();

    for (const auto &emp : taskContainer) {
        QString aim = QString::fromStdString(emp.getAim());
        QString deadline = QString::fromStdString(timePointToString(emp.getDeadline()));
        tasks.append(qMakePair(aim, deadline));
    }
    return tasks;
}

void ToDoWindow::loadNextInProcessTasks() {
    QList<QPair<QString, QString>> tasks = loadTasksFromDatabaseWithStatus(1, limit, currentOffset);
    for (const auto &task : tasks) {
        addTasksToInProcessList(task.first, task.second);
    }
    currentOffset += limit;
}

void ToDoWindow::addTasksToInProcessList(const QString &taskAim, const QString &taskDeadline) {
    auto *taskWidget = new QWidget();
    auto *layout = new QHBoxLayout(taskWidget);

    auto *aimLabel = new QLabel(taskAim, taskWidget);
    auto *deadLineLabel = new QLabel(taskDeadline, taskWidget);

    QDate deadLineDate = QDate::fromString(taskDeadline, "dd.MM.yyyy");
    if (deadLineDate < QDate::currentDate()) {
        aimLabel->setStyleSheet("color: red; font-weight: bold;");
    }

    layout->addWidget(aimLabel);
    layout->addWidget(deadLineLabel);
    layout->setContentsMargins(0, 0, 0, 0);
    taskWidget->setLayout(layout);

    auto *item = new QListWidgetItem(ui->inProcessList);
    item->setSizeHint(taskWidget->sizeHint());
    ui->inProcessList->setItemWidget(item, taskWidget);
}

void ToDoWindow::setupLazyLoadingForInProcessList() {
    connect(ui->inProcessList->verticalScrollBar(), &QScrollBar::valueChanged, this, &ToDoWindow::onInProcessScroll);
}

void ToDoWindow::onInProcessScroll(int value) {
    int maxValue = ui->inProcessList->verticalScrollBar()->maximum();
    if (value >= maxValue - 10) {
        loadMoreInProcessItems();
    }
}

void ToDoWindow::loadMoreInProcessItems() {
    QList<QPair<QString, QString>> moreTasks = loadTasksFromDatabaseWithStatus(1, limit, currentOffset);
    for (const auto &task : moreTasks) {
        addTasksToInProcessList(task.first, task.second);
    }
    currentOffset += limit;
}