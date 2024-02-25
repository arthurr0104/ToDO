#include "taskeditwidget.h"

#include <QLineEdit>
#include <QPushButton>
#include <QTextEdit>
#include <QDateTimeEdit>
#include <QCheckBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QModelIndex>
#include <QLabel>

TaskEditWidget::TaskEditWidget(TaskManager* const pTaskManager,  QWidget* parent /* = nullptr*/)
                : QWidget(parent), _editingTask(nullptr),
                _taskManager(pTaskManager)
{
    _taskDueDateEdit = new QDateTimeEdit(this);
    _taskDescriptionEdit = new QTextEdit(this);
    _taskNameEdit = new QLineEdit(this);
    _taskState = new QCheckBox(this);
    _confirmChangesBtn = new QPushButton("Confirm", this);
    _deleteTaskBtn = new QPushButton("Delete", this);
    QVBoxLayout *mainLayout = new QVBoxLayout;
    QHBoxLayout *nameAndStateLayout = new QHBoxLayout;
    QHBoxLayout *confirmAndDeleteLayout = new QHBoxLayout;

    _taskDescriptionEdit->setPlaceholderText("Add note");
    _taskDueDateEdit->setMaximumWidth(130);
    // _taskDescriptionEdit->setMaximumHeight(400);
    nameAndStateLayout->addWidget(_taskState);
    nameAndStateLayout->addWidget(_taskNameEdit);

    confirmAndDeleteLayout->addWidget(_confirmChangesBtn);
    confirmAndDeleteLayout->addWidget(_deleteTaskBtn);

    mainLayout->addLayout(nameAndStateLayout);
    mainLayout->addWidget(_taskDescriptionEdit);
    mainLayout->addWidget(_taskDueDateEdit);
    mainLayout->addLayout(confirmAndDeleteLayout);
    setLayout(mainLayout);

    connect(_confirmChangesBtn, &QPushButton::clicked, this,  &TaskEditWidget::confirmChanges);
    connect(_deleteTaskBtn, &QPushButton::clicked, [=](){
                                                            emit deleteClicked(_editingTaskIndex);
                                                            _editingTask = nullptr;
                                                            _editingTaskIndex = QModelIndex();
                                                            hide();
                                                        });
    connect(this, &TaskEditWidget::changesConfirmed, _taskManager, &TaskManager::taskChanged);
    setWindowTitle("Edit the task");
    hide();
}

void TaskEditWidget::openTaskEditingToolBar(const QModelIndex& taskToEdit){
    _editingTask = _taskManager->task(taskToEdit);

    if (!_editingTask) return;

    _taskNameEdit->setText(_editingTask->name());
    _taskDescriptionEdit->setText(_editingTask->description());
    _taskDueDateEdit->setDateTime(_editingTask->dueDate());
    _taskState->setChecked(_editingTask->isCompleted());
    _editingTaskIndex = taskToEdit;
    show();
}

void TaskEditWidget::confirmChanges(){
    if (!_editingTask) return;


    _editingTask->setCompleted(_taskState->isChecked());
    _editingTask->setName(_taskNameEdit->text());
    _editingTask->setDescription(_taskDescriptionEdit->toPlainText());
    _editingTask->setDueDate(_taskDueDateEdit->dateTime());
   
    emit changesConfirmed(_editingTaskIndex);
    hide();
    _editingTask = nullptr;
    _editingTaskIndex = QModelIndex();
}