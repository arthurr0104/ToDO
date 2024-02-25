#include "taskmanager.h"

#include <QVariant>
#include <QFile>
#include <QDataStream>


ProxyModel::ProxyModel(QObject* parent /* = nullptr */) : QSortFilterProxyModel(parent)
{

}

bool ProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const{
    auto sModel = sourceModel();

    
    // return sModel->data(sModel->index(source_row, 0), Qt::CheckStateRole).toBool();
    return true;
}

bool ProxyModel::lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const{
    QVariant leftData = sourceModel()->data(source_left);
    QVariant rightData = sourceModel()->data(source_right);

    switch (leftData.userType())
    {
        case QMetaType::QDateTime:
            return leftData.toDateTime() < rightData.toDateTime();
        case QMetaType::QString:
            return QString::localeAwareCompare(leftData.toString(), rightData.toString()) < 0;
        case QMetaType::Bool:
            return leftData.toBool() < rightData.toBool();
    };
    return true;

}


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

    switch(role){
        case Qt::CheckStateRole:
            return task->isCompleted() ? task->setCompleted(false) : task->setCompleted(true);
        break;
        default:
            return false;
    };
}

void TaskManager::addTask(Task *const pNewTask){
    if (pNewTask){
        size_t newRowIndex =  _tasks.size();
        if (insertRow(newRowIndex))
            _tasks[newRowIndex] = pNewTask;
    }
}

bool TaskManager::insertRows(int row, int count, const QModelIndex &parent)
{   
    if (row < 0 || row > rowCount(parent) || count <= 0)
        return false;

    beginInsertRows(parent, row, row + count - 1);
    for (int i = 0; i < count; ++i)
        _tasks.insert(row++, nullptr);
    endInsertRows();

    return true;
}

bool TaskManager:: removeRows(int row, int count, const QModelIndex &parent /* = QModelIndex()*/)
{   
    beginRemoveRows(parent, row, row + count - 1);
    for ( int i =0; i < count; ++i)
        _tasks.removeAt(row++);
    endRemoveRows();
    
    return true;
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

void TaskManager::deleteTask(const QModelIndex& removingIndex){
    if(removeRow(removingIndex.row()))
        emit taskDeleted();
    
}

void TaskManager::saveModel(){
    QFile file("db");

    file.open(QIODevice::WriteOnly);
    QDataStream stream(&file);

    for(auto task : _tasks){
        stream << task->name() << task->description()
        << task->dueDate() << task->isCompleted();
    }
    file.close();
}

void TaskManager::loadModel(){
    QFile file("db");
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