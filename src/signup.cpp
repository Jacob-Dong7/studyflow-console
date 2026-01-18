#include <sqlite3.h>
#include <iostream>
#include "signup.h"

void Signup::signup(sqlite3* db) {

    while (true) {
        std::cout << "Type -1 to quit\n";
        std::cout << "Please enter your desired username:\n";
        std::cout << "Username must be: At least 3 characters long\n";
        std::cin >> username;

        if (username == "-1") {
            return;
        }

        verify = verifyName(username, db);

        if (verify == 0) {
            continue;
        }

        std::cout << "Please enter your desired password:\n";
        std::cout << "Password must be: At least 5 characters long\n";
        std::cin >> password;

        verify = verifyPassword(password);

        if (verify == 0) {
            continue;
        } 

        std::cout << "Please select your university:\n";
        std::cout << "1(RMIT), 2(Monash), 3(USYD)\n";
        std::cin >> uni;

        std::cout << "Please select your email address:\n";
        std::cin >> email;

        verify = save(username, password, email, uni, db);

        if (verify != 1) {
            std::cout << "CRITICAL: There was an issue when saving to the database\n";
            continue;
        }

        std::cout << "Account successfuly created\n";
        return;
    }
}

bool Signup::verifyName(std::string username, sqlite3* db) {
    if (username.empty() || username.size() < 3) {
        std::cout << "Warning: Username does not meet requirement\n";
        return false;
    }
    sqlite3_stmt* stmt = nullptr;

    const char* checkUsername = "SELECT 1 FROM users WHERE display_name = ? LIMIT 1;";
    
    rc = sqlite3_prepare_v2(db, checkUsername, -1, &stmt, nullptr);

    if (rc != SQLITE_OK) return false;

    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_TRANSIENT);

    rc = sqlite3_step(stmt);
        
    if (rc == SQLITE_ROW) {
        std::cout << "Warning: username already exists\n";
        sqlite3_finalize(stmt);
        return false;
    }

    sqlite3_finalize(stmt);
    return true;
}

bool Signup::verifyPassword(std::string password) {
     
    if (password.empty() || password.size() < 5) {
        std::cout << "Warning: Password must be at least 5 characters long\n";
        return false;
    }

    return true;
}

bool Signup::save(std::string username, std::string password, std::string email, int uni, sqlite3* db) {
    sqlite3_stmt* stmt = nullptr;
    const char* sql = "INSERT INTO users(display_name, passwords, email, uni_id) VALUES(?, ?, ?, ?);";

    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);

    if (rc != SQLITE_OK) {
        return false;
    }

    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, email.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 4, uni);

    rc = sqlite3_step(stmt);

    if (rc != SQLITE_DONE) {
        sqlite3_finalize(stmt);
        return false;
    }

    sqlite3_finalize(stmt);

    return true;
}