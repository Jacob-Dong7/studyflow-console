#include "tasks.h"
#include <sqlite3.h>
#include <string>
#include <iostream>

void Tasks::taskMenu(sqlite3* db, std::string username) {
    int userInput;
    int id = -1;
    
    std::cout << "\nTasks\n";
    std::cout << "-----\n\n";

    while (true) {
        std::cout << "Options (Press corresponding numbers):\n" << "1(View Tasks)\n" << "2(Add Tasks)\n" << "3(Delete Tasks)\n" << "-1(Back)\n\n";
        std::cin >> userInput;
        if (userInput == -1) {
            std::cout << "Returning...\n";
            return;
        }
        id = getID(db, username);

        if (userInput == 1) {
            viewTask(db, id);
        } else if (userInput == 2) {

        } else if (userInput == 3) {

        }
    }
}

void Tasks::viewTask(sqlite3* db, int id) {
    int rc;
    sqlite3_stmt* stmt = nullptr;
    const char* sql = 
    "SELECT task_name "
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
        std::cout << "No Tasks Detected\n\n";
        sqlite3_finalize(stmt);
        return;
    }
    
    std::cout << "\nYour tasks:\n";
    std::cout << "-----------\n";

    while (rc == SQLITE_ROW) {
        std::string currTask = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        std::cout << currTask << "\n";
        rc = sqlite3_step(stmt);
    }
    std::cout << "-----------\n\n";
    sqlite3_finalize(stmt);
    return;
}

void Tasks::addTask(sqlite3* db, int id) {

}

bool Tasks::createTask(sqlite3* db, int id) {
    
}

void Tasks::deleteTask(sqlite3* db, int id) {

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