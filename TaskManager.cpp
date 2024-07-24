
#include "TaskManager.h"


void TaskManager::assignTask(const string &personName, const Task &task) {
    ////    first check if the worker already exists in the system
    for(Person& worker : this->workers){
        if(worker.getName() == personName){
            ////    create a new copy of the provided task
            ////    this is done so we can change the task_id
            Task copied_task = task;
            copied_task.setId(this->next_job_id);
            ////    assign the task to the provided worker
            worker.assignTask(copied_task);
            return;
        }
    }
    ////    check if there are MAX_PERSONS currently in the system
    if(this->current_workers == MAX_PERSONS){
        throw std::runtime_error("Maximum number of workers reached");
    }
    ////    here we can the worker with personName doesn't exist in the system
    ////    and we can add a new one because we still didn't reach the MAX_PERSONS
    ////    create a new worker
    Person new_worker(personName);
    ////    add the new_worker to the workers array, and increase the number of current workers
    this->workers[current_workers++] = new_worker;
    ////    create a new copy of the provided task
    ////    this is done so we can change the task_id
    Task copied_task = task;
    copied_task.setId(this->next_job_id);
    this->workers[current_workers - 1].assignTask(copied_task);
}

void TaskManager::completeTask(const string &personName) {

}

void TaskManager::bumpPriorityByType(TaskType type, int priority) {

}

void TaskManager::printAllEmployees() const {

}

void TaskManager::printTasksByType(TaskType type) const {

}

void TaskManager::printAllTasks() const {

}
