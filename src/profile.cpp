#include "profile.h"
#include <iostream>
void Profile::profile(sqlite3* db, std::string username) {
    int id = getID(db, username);
    getProfile(db, id);
}

void Profile::getProfile(sqlite3* db, int user_id) {
    sqlite3_stmt* stmt = nullptr;
    int rc;
    const char* sql = 
    "SELECT display_name, email, uni_name, state "
    "FROM users "
    "JOIN usersubject u "
    "ON users.user_id = u.user_id "
    "JOIN subjects s ON s.subject_id = u.subject_id "
    "JOIN university a ON users.uni_id = a.uni_id "
    "WHERE users.user_id = ?;";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, user_id);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_ROW) {
        std::cout << "CRITICAL: Error reading database\n";
        sqlite3_finalize(stmt);
        return;
    } else {
        username = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        email = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        uni = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        state = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));

        std::cout << "\nUser Profile\n------------------------------------\n";
        std::cout << "User ID: " << user_id << "\n";
        std::cout << "Username: " << username << "\n";
        std::cout << "Email: " << email << "\n";
        std::cout << "University: " << uni << "\n";
        std::cout << "State: " << state << "\n";
    }

    const char* subjectSQL = 
    "SELECT subject_name "
    "FROM usersubject "
    "JOIN subjects "
    "ON subjects.subject_id = usersubject.subject_id " 
    "WHERE usersubject.user_id = ?;";

    rc = sqlite3_prepare_v2(db, subjectSQL, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cout << "CRITICAL: Error reading database\n";
        sqlite3_finalize(stmt);
        return;
    }

    sqlite3_bind_int(stmt, 1, user_id);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_ROW) {
        std::cout << "CRITICAL: Error reading database\n";
        sqlite3_finalize(stmt);
        return;
    } else {
        std::string subjectname;
        std::cout << "\n----------------------\n";
        std::cout << "You Are Enrolled In:\n";
        std::cout <<"----------------------\n";
        while (rc == SQLITE_ROW) {
            subjectname = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
            std::cout << subjectname << "\n";
            rc = sqlite3_step(stmt);
        }
    }

    const char* taskSQL = 
    "SELECT task_name, due_date "
    "FROM usertask "
    "JOIN tasks "
    "ON tasks.task_id = usertask.task_id " 
    "WHERE usertask.user_id = ?;";

    rc = sqlite3_prepare_v2(db, taskSQL, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cout << "CRITICAL: Error reading database\n";
        sqlite3_finalize(stmt);
        return;
    }

    sqlite3_bind_int(stmt, 1, user_id);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_ROW) {
        std::cout << "CRITICAL: Error reading database\n";
        sqlite3_finalize(stmt);
        return;
    } else {
        std::string taskname;
        std::string due;
        std::cout << "\n----------------------\n";
        std::cout << "Your current tasks:\n";
        std::cout <<"----------------------\n";
        while (rc == SQLITE_ROW) {
            taskname = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
            due = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            std::cout << taskname << ", due on: " <<  due << "\n";
            rc = sqlite3_step(stmt);
        }
    }

    std::cout << "------------------------------------\n";
    sqlite3_finalize(stmt);
    return;

}

int Profile::getID(sqlite3* db, std::string username) {
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