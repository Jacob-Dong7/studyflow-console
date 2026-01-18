#include <iostream>
#include <sqlite3.h>

#include "login.h"

void Login::login(sqlite3* db) {
    int rc;
    sqlite3_stmt* stmt = nullptr;
    std::cout << "Type -1 to quit\n";

    while (true) {
        if (username == "-1") return;

        std::cout << "Please enter your username:\n";
        std::cin >> username;

        if (username.empty()) {
            std::cout << "Username is empty";
            continue;
        }

        std::cout << "Please enter your password:\n";
        std::cin >> password;

        if (password.empty()) {
            std::cout << "Password is empty";
            continue;
        }
        
        const char* sql = "SELECT 1 FROM users WHERE display_name = ? AND passwords = ? LIMIT 1;";
        rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
        if (rc != SQLITE_OK) return;

        sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_TRANSIENT);

        rc = sqlite3_step(stmt);

        if (rc == SQLITE_ROW) {
            std::cout << "Login success\n";
            sqlite3_finalize(stmt);
            return;
        } else if (rc = SQLITE_DONE){
            std::cout << "Login failed\n";
            sqlite3_finalize(stmt);
            return;
        }


    }
    
}