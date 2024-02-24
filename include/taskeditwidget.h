#ifndef EDITTOOLBAR_H
#define EDITTOOLBAR_H

#include "taskmanager.h"

#include <QWidget>


class QLineEdit;
class QTextEdit;
class QDateTimeEdit;
class QPushButton;
class QCheckBox;
class QModelIndex;

class TaskEditWidget: public QWidget{
    Q_OBJECT

public:
    explicit TaskEditWidget(TaskManager* const pTaskManager, QWidget* parent = nullptr);

signals:
    void changesConfirmed(const QModelIndex&);

public slots:
    void openTaskEditingToolBar(const QModelIndex& );

private slots:
    void confirmChanges();

private:
    QLineEdit* _taskNameEdit;
    QCheckBox *_taskState;
    QTextEdit* _taskDescriptionEdit;
    QDateTimeEdit* _taskDueDateEdit;
    QPushButton* _confirmChangesBtn;
    TaskManager *_taskManager;
    Task* _editingTask;
    QModelIndex _editingTaskIndex;
};

#endif // EDITTOOLBAR_H