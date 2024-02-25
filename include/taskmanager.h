#ifndef TASKMANAGER_H
#define TASKMANAGER_H

#include "task.h"

#include <QAbstractListModel>
#include <QList>
#include <QSortFilterProxyModel>


class ProxyModel : public QSortFilterProxyModel {
    Q_OBJECT

public:
    ProxyModel(QObject *parent = nullptr);

    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const;

    bool lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const;

};

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
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

signals:
    void taskDeleted();
    
public slots:
    void taskChanged(const QModelIndex& changedIndex);

    void deleteTask(const QModelIndex& removingIndex);

private:
    QList<Task*> _tasks;

    void saveModel();
    void loadModel();
};
#endif // TASKMANAGER_H
