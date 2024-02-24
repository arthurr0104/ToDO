#ifndef TASKMANAGER_H
#define TASKMANAGER_H

#include "task.h"

#include <QAbstractListModel>
#include <QList>

class TaskManager : public QAbstractListModel
{
    Q_OBJECT
public:

    TaskManager(QObject *parent = nullptr);
    ~TaskManager();

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;
    void addTask(Task *const pNewTask);
    Task* task(const QModelIndex& index);
    
public slots:
    void taskChanged(const QModelIndex& changedIndex);

private:
    QList<Task*> _tasks;

    void saveModel();
    void loadModel();
};
#endif // TASKMANAGER_H
