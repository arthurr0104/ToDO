#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include "taskmanager.h"
#include "taskeditwidget.h"

#include <QWidget>
#include <QListView>
#include <QLineEdit>
#include <QTextEdit>
#include <QCheckBox>
#include <QStandardItemModel>
#include <QDateTimeEdit>
#include <QPushButton>
#include <QToolBar>

class MainWidget: public QWidget{
    Q_OBJECT
public:
    explicit MainWidget(QWidget* parent = nullptr);

public slots:
    void addTask();

private:
    TaskManager* _taskManager;
    QListView* _tasksView;
    QLineEdit* _taskNameEdit;
    QPushButton* _addTaskButton;
    QDateTimeEdit* _taskDueDateEdit;
    TaskEditWidget* _taskEditWgt;
};

#endif // ifndef MAINWIDGET_H