#include <iostream>
#include <sqlite3.h>

#include "login.h"
#include "dashboard.h"

void Login::login(sqlite3* db) {
    int rc;
    sqlite3_stmt* stmt = nullptr;
    std::cout << "\n-1(Return)\n\n";

    while (true) {
        std::cout << "Please enter your username:\n\n";
        std::cin >> username;
        if (username == "-1") return;

        if (username.empty()) {
            std::cout << "\nUsername is empty\n\n";
            continue;
        }

        std::cout << "\nPlease enter your password:\n\n";
        std::cin >> password;

        if (password.empty()) {
            std::cout << "\nPassword is empty\n\n";
            continue;
        }
        
        const char* sql = "SELECT 1 FROM users WHERE display_name = ? AND passwords = ? LIMIT 1;";
        rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
        if (rc != SQLITE_OK) return;

        sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_TRANSIENT);

        rc = sqlite3_step(stmt);

        if (rc == SQLITE_ROW) {
            std::cout << "Login success\n\n";
            sqlite3_finalize(stmt);
            break;
        } else if (rc = SQLITE_DONE){
            std::cout << "\nLogin failed\n\n";
            sqlite3_finalize(stmt);
            return;
        }
    }

    Dashboard userDash;
    userDash.dashboard(db, username, password);

    return;
}