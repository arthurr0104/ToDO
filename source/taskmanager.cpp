#include "taskmanager.h"

#include <QVariant>
#include <QFile>
#include <QDataStream>

TaskManager::TaskManager(QObject *parent)
    : QAbstractListModel(parent)
{
    loadModel();
}

TaskManager::~TaskManager()
{
    saveModel();

    for (Task* pTask: _tasks)
        delete pTask;
}

int TaskManager::rowCount(const QModelIndex &parent /*= QModelIndex()*/) const{
    return _tasks.size();
}

QVariant TaskManager::data(const QModelIndex &index, int role /*= Qt::DisplayRole*/) const{
    if (!index.isValid()) return QVariant();

    const Task* task = _tasks[index.row()];

    switch(role){
        case Qt::DisplayRole:
            return task->name();
        case Qt::ToolTipRole:
            return task->description();
        case Qt::CheckStateRole:
            return task->isCompleted(); 
        default:
            return QVariant();
    };
}

bool TaskManager::setData(const QModelIndex &index, const QVariant &value, int role /* = Qt::EditRole*/){
    if (!index.isValid()) return false;

    Task* task = _tasks[index.row()];
    bool result = false;

    switch(role){
        case Qt::CheckStateRole:
            result = task->setCompleted(value.toBool());
        default:
            return false;
    };
    if (result) dataChanged(index, index);
}

void TaskManager::addTask(Task *const pNewTask){
    if (pNewTask){

        _tasks.push_back(pNewTask);
        auto newRow = _tasks.size() - 1;
        emit dataChanged(index(newRow), index(newRow));
    }
}


Qt::ItemFlags TaskManager::flags(const QModelIndex& index) const{
    Qt::ItemFlags defaultFlags = QAbstractListModel::flags(index);
    
    if (index.isValid() && index.row() < rowCount())
        return  Qt::ItemIsEnabled | Qt::ItemIsUserCheckable;
    return defaultFlags;
}


Task* TaskManager::task(const QModelIndex& index){
    return (index.isValid() && index.row() < rowCount()) ? _tasks[index.row()] : nullptr;
}

void TaskManager::taskChanged(const QModelIndex& changedIndex){
    if (changedIndex.isValid()  && changedIndex.row() < rowCount()){
        emit dataChanged(changedIndex, changedIndex);
    }
}

void TaskManager::saveModel(){
    QFile file("savedData");

    file.open(QIODevice::WriteOnly);
    QDataStream stream(&file);

    for(auto task : _tasks){
        stream << task->name() << task->description()
        << task->dueDate() << task->isCompleted();
    }
    file.close();
}

void TaskManager::loadModel(){
    QFile file("savedData");
    file.open(QIODevice::ReadOnly);
    QDataStream stream(&file);

    beginResetModel();
    _tasks.clear();

    Task* task;
    while(!stream.atEnd()){
        QString name, desc;
        QDateTime dueDate;
        bool state;

        stream >> name >> desc >> dueDate >> state;
        task = new Task(name, desc, dueDate, state);
        _tasks.push_back(task);
    }

    endResetModel();
    file.close();
}