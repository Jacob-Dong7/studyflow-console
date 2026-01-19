#include "subject.h"
#include <sqlite3.h>
#include <string>
#include <iostream>

void Subject::loadSubject(sqlite3* db, std::string username, std::string password, int userInput) {
    sqlite3_stmt* stmt = nullptr;
    int rc;
    int id;

    id = getID(db, username);

    if (id == -1) {
        std::cout << "Error fetching user data\n";
        sqlite3_finalize(stmt);
        return;
    }

    if (userInput == 1) {
        const char* sql = "select subject_name from subjects s JOIN usersubject u ON s.subject_id = u.subject_id JOIN users b ON b.user_id = u.user_id WHERE b.user_id = ?;";
        rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
        sqlite3_bind_int(stmt, 1, id);

        rc = sqlite3_step(stmt);
        while (rc == SQLITE_ROW) {
            std::cout << sqlite3_column_text(stmt, 0) << "\n";
            rc = sqlite3_step(stmt);
        }
    } else {
        return;
    }

    sqlite3_finalize(stmt);
    return;
}

int Subject::getID(sqlite3* db, std::string username) {
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