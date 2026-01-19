#include <sqlite3.h>
#include <iostream>
#include "signup.h"

bool Signup::signup(sqlite3* db) {
    while (true) {
        std::cout << "\nPlease enter your desired username -1(Return):\n";
        std::cout << "(Username must be: At least 3 characters long)\n\n";
        std::cin >> username;

        if (username == "-1") {
            std::cout << "\nReturning...\n";
            return false;
        }

        verify = verifyName(username, db);

        if (verify == 0) {
            continue;
        }

        std::cout << "\nPlease enter your desired password -1(Return):\n";
        std::cout << "(Password must be: At least 5 characters long)\n\n";
        std::cin >> password;

        if (password == "-1") {
            std::cout << "\nReturning...\n";
            return false;
        }

        verify = verifyPassword(password);

        if (verify == 0) {
            continue;
        } 

        std::cout << "\nPlease select your university -1(Return):\n";
        std::cout << "1(RMIT), 2(Monash), 3(USYD)\n\n";
        std::cin >> uni;

        if (uni == -1) {
            std::cout << "\nReturning...\n";
            return false;
        }

        std::cout << "\nPlease select your email address -1(Return):\n\n";
        std::cin >> email;

        if (email == "-1") {
            std::cout << "\nReturning...\n";
            return false;
        }

        verify = save(username, password, email, uni, db);

        if (verify != 1) {
            std::cout << "CRITICAL: There was an issue when saving to the database\n";
            continue;
        }

        std::cout << "Account successfuly created\n";
        break;
    }

    return true;
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