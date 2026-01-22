#ifndef TASKS_H
#define TASKS_H
#include <sqlite3.h>
#include <string>
class Tasks {
    public:
    void viewTask(sqlite3*, int);
    void addTask(sqlite3*, int); 
    void deleteTask(sqlite3*, int); 
    int getID(sqlite3*, std::string);

    bool createTask(sqlite3*, int, std::string, std::string);
    void taskMenu(sqlite3*, std::string username);

    int getTaskID(sqlite3* db, std::string taskname);
    private:
    std::string name, date;

};
#endif