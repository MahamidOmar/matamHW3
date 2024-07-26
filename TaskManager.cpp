
#include "TaskManager.h"

TaskType TaskManager::type_to_check = TaskType::General;
int TaskManager::priority_increment = 0;

void TaskManager::assignTask(const string &personName, const Task &task) {
    ////    first check if the worker already exists in the system
    for(Person& worker : this->workers){
        if(worker.getName() == personName){
            ////    allocate a new copy of the provided task
            ////    this is done so we can change the task_id
            Task copied_task = task;
            ////    change the id, and increase next_job_id
            copied_task.setId((this->next_job_id)++);
            ////    assign the task to the provided worker
            worker.assignTask(copied_task);
            ////    add the task to the all_tasks sorted_list
            this->all_tasks.insert(copied_task);
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
    ////    allocate a new copy of the provided task
    ////    this is done so we can change the task_id
    Task copied_task = task;
    ////    change the id, and increase next_job_id
    copied_task.setId((this->next_job_id)++);
    ////    assign the task to the added worker
    this->workers[current_workers - 1].assignTask(copied_task);
    ////    add the task to the all_tasks sorted_list
    this->all_tasks.insert(copied_task);
}

void TaskManager::completeTask(const string &personName) {
    ////    search for the worker with personName
    for(Person& worker : this->workers){
        if(worker.getName() == personName){
            ////    when found, use the completeTask method defined in user
            int finished_id = worker.completeTask();
            ////    iterate over the all_tasks sorted_list and find the correct node for the task inside it
            for(SortedList<Task>::ConstIterator it = this->all_tasks.begin() ; it != this->all_tasks.end() ; ++it){
                ////    search for the node by getting the id of the task
                if((*it).getId() == finished_id){
                    ////    use the sorted_list remove method which takes the iterator and then removes the node
                    this->all_tasks.remove(it);
                    return;
                }
            }
        }
    }
}

void TaskManager::bumpPriorityByType(TaskType type, int priority) {
    if(priority <= 0){
        return;
    }
    ////    update the static fields according to the received value
    TaskManager::type_to_check = type;
    TaskManager::priority_increment = priority;
    ////    get all the tasks that are of correct type
    SortedList<Task> filtered = this->all_tasks.filter(check_type);
    ////    apply the increase_priority on all of the filtered tasks
    SortedList<Task> changed = filtered.apply(increase_priority);
    ////    remove all of the old filtered tasks from the original list
    for(SortedList<Task>::ConstIterator it = filtered.begin() ; it != filtered.end() ; ++it){
        for(SortedList<Task>::ConstIterator it2 = this->all_tasks.begin() ; it2 != this->all_tasks.end() ; ++it2){
            if((*it).getId() == (*it2).getId()){
                this->all_tasks.remove(it2);
            }
            ////    check if the list is empty, need to exit because operator++ will not work
            if(this->all_tasks.length() == 0 || !(it2 != this->all_tasks.end())){
                break;
            }
        }
        ////    check if the list is empty, need to exit because operator++ will not work
        if(this->all_tasks.length() == 0){
            break;
        }
    }
    ////    add the filtered tasks after applying the increase_priority on them
    for(SortedList<Task>::ConstIterator it = changed.begin() ; it != changed.end() ; ++it){
        this->all_tasks.insert(*it);
    }

    ////    now filter the list of each worker in the system
    for(int i = 0 ; i < current_workers ; ++i){
        Person person = workers[i];
        SortedList<Task> worker_tasks = person.getTasks();
        filtered = worker_tasks.filter(check_type);
        ////    apply the increase_priority on all of the filtered tasks
        changed = filtered.apply(increase_priority);
        ////    remove all of the old filtered tasks from the original list
        for(SortedList<Task>::ConstIterator it = filtered.begin() ; it != filtered.end() ; ++it){
            for(SortedList<Task>::ConstIterator it2 = worker_tasks.begin() ; it2 != worker_tasks.end() ; ++it2){
                if((*it).getId() == (*it2).getId()){
                    worker_tasks.remove(it2);
                }
                ////    check if the list is empty, need to exit because operator++ will not work
                if(worker_tasks.length() == 0 || !(it2 != worker_tasks.end())){
                    break;
                }
            }
            ////    check if the list is empty, need to exit because operator++ will not work
            if(worker_tasks.length() == 0){
                break;
            }
        }
        ////    add the filtered tasks after applying the increase_priority on them
        for(SortedList<Task>::ConstIterator it = changed.begin() ; it != changed.end() ; ++it){
            worker_tasks.insert(*it);
        }
        ////    update the worker's task
        person.setTasks(worker_tasks);
    }

}

void TaskManager::printAllEmployees() const {
    for(int i = 0 ; i < current_workers ; ++i){
        std::cout << workers[i] << std::endl;
    }
}

void TaskManager::printTasksByType(TaskType type) const {
    type_to_check = type;
    SortedList<Task> filtered = this->all_tasks.filter(check_type);
    for(SortedList<Task>::ConstIterator it = filtered.begin() ; it != filtered.end() ; ++it){
        std::cout << (*it) << std::endl;
    }
}

void TaskManager::printAllTasks() const {
    for(SortedList<Task>::ConstIterator it = this->all_tasks.begin() ; it != this->all_tasks.end() ; ++it){
        std::cout << (*it) << std::endl;
    }
}

bool TaskManager::check_type(Task task) {
    return task.getType() == TaskManager::type_to_check;
}

Task TaskManager::increase_priority(Task task) {
    ////    create a new task that has the new priority
    Task new_task = Task(task.getPriority() + TaskManager::priority_increment, task.getType(), task.getDescription());
    new_task.setId(task.getId());
    return new_task;
}

