#include "task.h"

Task::Task(const QString& name, const QDateTime& dueDate):
    _name{name},
    _dueDate{dueDate},
    _isCompleted(false)
{
    
}

Task::Task(const QString& name, const QString& desc, const QDateTime& dueDate, bool state):
    _name{name},
    _description{desc},
    _dueDate{dueDate},
    _isCompleted(state)
{

}

bool Task::setName(const QString& name){
    if (name.isEmpty() || name.size() > 500 || _name == name)
        return false;
    
    _name = name;
    return true;
}

bool Task::setDescription(const QString& desc){
    if (desc.size() > 1000 || _description == desc)
        return false;
    
    _description = desc;
    return true;
}

const QString& Task::name()const{
    return _name;
}

const QString& Task::description()const{
    return _description;
}

bool Task::setCompleted(bool state){
    if (_isCompleted != state){
        _isCompleted = state;
        
        return true;
    }
    return false;
}

bool Task::isCompleted()const{
    return _isCompleted;
}

const QDateTime& Task::dueDate() const{
    return _dueDate;
}

bool Task::setDueDate(const QDateTime& time){
    // Check the validation of the time
    if (!time.isValid() || _dueDate == time) return false;

    _dueDate = time;
    return true;
}
