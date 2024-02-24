#ifndef TASK_H
#define TASK_H

#include <QString>
#include <QDateTime>

class Task
{
public:
    Task(const QString& name, const QDateTime& dueDate);
    Task(const QString& name, const QString& desc, const QDateTime& dueDate, bool state);

    bool setName(const QString& name);
    const QString& name()const;

    bool setDescription(const QString& desc);
    const QString& description()const;

    const QDateTime& dueDate()const;
    bool setDueDate(const QDateTime& time);

    bool setCompleted(bool state);
    bool isCompleted()const;

private:
    QString _name;
    QString _description;
    QDateTime _dueDate;
    bool _isCompleted;
};

#endif // TASK_H
