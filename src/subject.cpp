#include "subject.h"
#include <sqlite3.h>
#include <string>
#include <limits>
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
        std::cout << "\n";
        const char* sql = "select subject_name from subjects s JOIN usersubject u ON s.subject_id = u.subject_id JOIN users b ON b.user_id = u.user_id WHERE b.user_id = ?;";
        rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
        sqlite3_bind_int(stmt, 1, id);

        rc = sqlite3_step(stmt);

        if (rc != SQLITE_ROW) {
            std::cout << "User has no subjects enrolled\n\n";
            sqlite3_finalize(stmt);
            return;
        }

        while (rc == SQLITE_ROW) {
            std::cout << sqlite3_column_text(stmt, 0) << "\n";
            rc = sqlite3_step(stmt);
        }
        std::cout << "\n";
    } else if (userInput == 2) {
        createSubject(db, id);
    } else if (userInput == 3) {
        deleteSubject(db, username);
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

void Subject::createSubject(sqlite3* db, int user_id) {
    int rc;
    sqlite3_stmt* stmt = nullptr;
    while (true) {
        std::cout << "\nPlease enter the name of the subject -1(Return)\n\n";
        if (std::cin.peek() == '\n') {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        std::getline(std::cin, subjectName);
        
        if (subjectName == "-1") {
            std::cout << "Returning...\n\n";
            return;
        }

        const char* sqlCreate = "INSERT INTO subjects (subject_name) VALUES (?)";
        rc = sqlite3_prepare_v2(db, sqlCreate, -1, &stmt, nullptr);

        if (rc != SQLITE_OK) return;

        sqlite3_bind_text(stmt, 1, subjectName.c_str(), -1, SQLITE_TRANSIENT);

        rc = sqlite3_step(stmt);
        if (rc != SQLITE_DONE) {
            std::cout << "\nError writing to database\n\n";
            sqlite3_finalize(stmt);
            return;
        }
        int subjectID = getSubjectID(db, subjectName);

        addSubject(db, user_id, subjectID);
        sqlite3_finalize(stmt);

        return;
    }
}

int Subject::getSubjectID(sqlite3* db, std::string subject) {
    int subject_id;
    sqlite3_stmt* stmt = nullptr;
    int rc;
    const char* sql = "SELECT subject_id FROM subjects WHERE subject_name = ?;";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) return -1;

    sqlite3_bind_text(stmt, 1, subject.c_str(), -1, SQLITE_TRANSIENT);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_ROW) {
        std::cout << "CRITICAL: Error reading from database\n\n";
        sqlite3_finalize(stmt);
        return -1;
    } else {
        subject_id = sqlite3_column_int(stmt, 0);
        sqlite3_finalize(stmt);
        return subject_id;
    }

}

void Subject::addSubject(sqlite3* db,int user_id, int subject) {
    sqlite3_stmt* stmt = nullptr;
    int rc;
    const char* sql = "INSERT INTO usersubject (user_id, subject_id) VALUES(?, ?);";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, user_id);
    sqlite3_bind_int(stmt, 2, subject);

    rc = sqlite3_step(stmt);

    if (rc != SQLITE_DONE) {
        std::cout << "CRTICIAL: Error writing to database\n\n";
        sqlite3_finalize(stmt);
        return;
    } else {
        std::cout << "\nSubject added\n\n";
        sqlite3_finalize(stmt);
        return;
    }
    
}

void Subject::deleteSubject(sqlite3* db, std::string username) {
    sqlite3_stmt* stmt = nullptr;
    std::string name;
    int rc;
    bool verify;
    int id, subjectID;
    while (true) {
        std::cout << "\nPlease enter the name of the subject you want to remove -1(Return)\n\n"; 

        if (std::cin.peek() == '\n') {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }

        std::getline(std::cin, name);

        if (name == "-1") {
            std::cout << "Returning...\n\n";
            return;
        }

        id = getID(db, username);
        subjectID = getSubjectID(db, name);

        

        verify = checkSubject(db, name);

        if (verify != true) {
            std::cout << "\nThe subject you entered does not exist in your record\n\n";
            return;
        } else {
            const char* sql = "DELETE FROM usersubject WHERE user_id = ? AND subject_id = ?;";
            rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
            if (rc != SQLITE_OK) {
                std::cout << "CRTICIAL: Error writing to database\n\n";
                return;
            }

            sqlite3_bind_int(stmt, 1, id);
            sqlite3_bind_int(stmt, 2, subjectID);

            rc = sqlite3_step(stmt);
            
            if (rc != SQLITE_DONE) {
                std::cout << "CRTICIAL: Error writing to database\n\n";
                sqlite3_finalize(stmt);
                return;
            } 
            std::cout << "Subject removed\n\n";
            sqlite3_finalize(stmt);
            return;
        }
    }
}

bool Subject::checkSubject(sqlite3* db, std::string name) {
    sqlite3_stmt* stmt = nullptr;
    int rc;

    const char* sql = "SELECT subject_id FROM subjects WHERE subject_name = ? LIMIT 1;";

    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_TRANSIENT);
    if (rc != SQLITE_OK) {
        std::cout << "CRITICAL: Error reading from database\n\n";
        sqlite3_finalize(stmt);
        return false;
    } else {
        rc = sqlite3_step(stmt);
        if (rc = SQLITE_ROW) {
            sqlite3_finalize(stmt);
            return true;
        }
    }

    return false;
}