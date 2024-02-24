#include "mainwidget.h"

#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>

MainWidget::MainWidget(QWidget *parent /* = nullptr*/) : QWidget(parent){
    _taskManager = new TaskManager(this);
    _tasksView = new QListView(this);
    _taskDueDateEdit = new QDateTimeEdit(this);
    _taskNameEdit = new QLineEdit(this);
    _addTaskButton = new QPushButton("Add a task", this);
    _taskEditWgt = new TaskEditWidget(_taskManager, this);
    QVBoxLayout *vLayout = new QVBoxLayout;
    
    _addTaskButton->hide();
    _tasksView->setModel(_taskManager);
    _taskNameEdit->setPlaceholderText("Add a task");
    _taskDueDateEdit->setMinimumDateTime(QDateTime::currentDateTime());
    
    vLayout->addWidget(_taskNameEdit);
    vLayout->addWidget(_taskDueDateEdit);
    vLayout->addWidget(_addTaskButton);
    vLayout->addWidget(_tasksView);

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addLayout(vLayout);
    mainLayout->addWidget(_taskEditWgt);

    setLayout(mainLayout);


    
    connect(_addTaskButton, &QPushButton::clicked, this, &MainWidget::addTask);
    connect(_taskNameEdit, &QLineEdit::textChanged, _addTaskButton, &QPushButton::show);
    connect(_addTaskButton, &QPushButton::clicked, _addTaskButton, &QPushButton::hide);
    connect(_tasksView, &QListView::doubleClicked, _taskEditWgt, &TaskEditWidget::openTaskEditingToolBar);
    
    connect(_tasksView, &QListView::doubleClicked, [=](){
                                                        _tasksView->setEnabled(false);
                                                        _taskNameEdit->setEnabled(false);
                                                        _taskDueDateEdit->setEnabled(false);
                                                        });

    connect(_taskManager, &TaskManager::dataChanged, [=](){
                                                        if (!_tasksView->isEnabled())
                                                        {
                                                            _tasksView->setEnabled(true);
                                                            _taskNameEdit->setEnabled(true);
                                                            _taskDueDateEdit->setEnabled(true);
                                                        }});
    setMinimumSize(800, 800);
    show();
}

    
void MainWidget::addTask(){
    QString name = _taskNameEdit->text();
    QDateTime dueDate = _taskDueDateEdit->dateTime(); 

    if (!name.isEmpty() && name.size() <= 500){
        _taskManager->addTask(new Task(name, dueDate));
        _taskNameEdit->clear();
        _taskDueDateEdit->setDateTime(QDateTime::currentDateTime());
    }
}