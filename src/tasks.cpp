#include "tasks.h"
#include <sqlite3.h>
#include <string>
#include <iostream>
#include <limits>

void Tasks::taskMenu(sqlite3* db, std::string username) {
    int userInput;
    int id = -1;
    
    std::cout << "\nTasks\n";
    std::cout << "-----\n\n";

    while (true) {
        std::cout << "Options (Press corresponding numbers):\n";
        std::cout << "--------------------------------------\n"; 
        std::cout << "1(View Tasks)\n" << "2(Add Tasks)\n" << "3(Delete Tasks)\n" << "-1(Back)\n\n";
        std::cin >> userInput;
        if (userInput == -1) {
            std::cout << "Returning...\n";
            return;
        }
        id = getID(db, username);

        if (userInput == 1) {
            viewTask(db, id);
        } else if (userInput == 2) {
            addTask(db, id);
        } else if (userInput == 3) {
            deleteTask(db, id);
        }
    }
}

void Tasks::viewTask(sqlite3* db, int id) {
    int rc;
    sqlite3_stmt* stmt = nullptr;
    const char* sql = 
    "SELECT task_name, due_date, usertask_id "
    "FROM tasks "
    "JOIN usertask ut "
    "ON tasks.task_id = ut.task_id "
    "JOIN users u "
    "ON ut.user_id = u.user_id "
    "WHERE u.user_id = ?;";

    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cout << "CRITICAL: Error reading from database\n\n";
        sqlite3_finalize(stmt);
        return;
    }

    sqlite3_bind_int(stmt, 1, id);

    rc = sqlite3_step(stmt);

    if (rc == SQLITE_DONE) {
        std::cout << "\n---------------\n";
        std::cout << "No Tasks Detected\n";
        std::cout << "---------------\n";
        sqlite3_finalize(stmt);
        return;
    }
    
    std::cout << "\nYour tasks:\n";
    std::cout << "-----------\n";

    while (rc == SQLITE_ROW) {
        std::string currTask = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        std::string currdate =  reinterpret_cast<const char*>(sqlite3_column_text(stmt,1));
        int usertask_id = sqlite3_column_int(stmt, 2);
        std::cout << "Task ID: " << usertask_id << ", Taskname: "<< currTask << ", Due on: " << currdate << "\n";
        rc = sqlite3_step(stmt);
    }
    std::cout << "-----------\n\n";
    sqlite3_finalize(stmt);
    return;
}

void Tasks::addTask(sqlite3* db, int id) {
    std::cout << "Please enter the name of the task -1(Return):\n";
    std::cout <<"-----------------------------------\n";
    if (std::cin.peek() == '\n') {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    std::getline(std::cin, name);

    if (name == "-1") {
        std::cout << "Returning...\n";
        return;
    }

    std::cout << "Please enter the due date of the task in this format 'YYYY-MM-DD' -1(Return):\n\n";
    std::cin >> date;

    if (date == "-1") {
        std::cout << "Returning...\n";
        return;
    }

    bool verify = createTask(db, id, name, date);
    if (verify == true) {
        std::cout <<"\nTask successfully added\n\n";
        return;
    } else {
        std::cout <<"\nTask creation unsuccessful\n\n";
        return;
    }
}

bool Tasks::createTask(sqlite3* db, int id, std::string name, std::string date) {

    sqlite3_stmt* stmt = nullptr;
    int rc;
    const char* newTaskSql = 
    "INSERT into tasks(task_name) VALUES(?);";

    rc = sqlite3_prepare_v2(db, newTaskSql, -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_TRANSIENT);
    if (rc != SQLITE_OK) {
        sqlite3_finalize(stmt);
        return false;
    }

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        sqlite3_finalize(stmt);
        return false;
    }

    int taskid = getTaskID(db, name);
    std::cout << "aasasasasasas: " << taskid;
    const char* addTaskSql = 
    "INSERT INTO usertask (due_date, user_id, task_id) VALUES(?, ?, ?);";

    rc = sqlite3_prepare_v2(db, addTaskSql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cout << "CRITICAL: sqlite3_prepare_v2 has failed\n";
        sqlite3_finalize(stmt);
        return false;
    }

    sqlite3_bind_text(stmt, 1, date.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 2, id);
    sqlite3_bind_int(stmt, 3, taskid);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        std::cout << "CRITICAL: sqlite3_step has failed\n";
        sqlite3_finalize(stmt);
        return false;
    }

    sqlite3_finalize(stmt);
    return true;
}

void Tasks::deleteTask(sqlite3* db, int id) {
    int taskID;

    std::cout << "\nEnter the ID of the task you want to delete -1(Return):\n\n";
    std::cin >> taskID;

    if (taskID == -1) {
        std::cout << "Returning...\n";
        return;
    }

    sqlite3_stmt* stmt = nullptr;
    int rc;

    const char* sql = 
    "DELETE FROM usertask WHERE usertask_id = ?;";

    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK){
        std::cout << "CRITICAL: Error reading database\n";
        sqlite3_finalize(stmt);
        return;
    }

    sqlite3_bind_int(stmt, 1, taskID);
    rc = sqlite3_step(stmt);

    if (rc != SQLITE_DONE) {
        std::cout << "CRITICAL: Error reading database\n";
        sqlite3_finalize(stmt);
        return;
    } else {
        std::cout << "\nTask (ID " << taskID  << ") has been deleted\n\n";
        sqlite3_finalize(stmt);
        return;
    }
}

int Tasks::getID(sqlite3* db, std::string username) {
    sqlite3_stmt* stmt = nullptr;
    const char* sql = "SELECT user_id FROM users WHERE display_name = ?;";

    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) return -1;

    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_TRANSIENT);

    int id = -1;
    rc = sqlite3_step(stmt);
    
    if (rc == SQLITE_ROW) {
        id = sqlite3_column_int(stmt, 0);
    } else {
        sqlite3_finalize(stmt);
        return -1;
    }

    sqlite3_finalize(stmt);
    return id;
}

int Tasks::getTaskID(sqlite3* db, std::string taskname) {
    int id;
    sqlite3_stmt* stmt = nullptr;
    int rc;
    const char* sql =
    "SELECT task_id FROM tasks where task_name = ? LIMIT 1;";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cout << "CRITICAL: Error reading database\n";
        sqlite3_finalize(stmt);
        return -1;
    }

    sqlite3_bind_text(stmt, 1, taskname.c_str(), -1, SQLITE_TRANSIENT);
    rc = sqlite3_step(stmt);

    if (rc != SQLITE_ROW) {
        std::cout << "CRITICAL: Error reading database\n";
        sqlite3_finalize(stmt);
        return -1;
    } else {
        id = sqlite3_column_int(stmt, 0);
    }
    sqlite3_finalize(stmt);
    return id;
}