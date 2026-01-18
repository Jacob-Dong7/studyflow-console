#include <iostream>
#include <sqlite3.h>
#include "login.h"


int main() {
    sqlite3* db = nullptr;
    const char* dbPath = "studyflow.db";

    int rc = sqlite3_open(dbPath, &db);

    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    int userInput;
    std::cout << "Welcome to StudyFlow\n";
    std::cout << "Type 1 to login, 2 to signup and 0 to quit\n";
    std::cin >> userInput;

    if (userInput == 0) {
        std::cout << "Quitting...\n";
        return 0;
    }

    //login & signup
    if (userInput == 1) {
        Login userLogin;
        userLogin.login(db);
    } else if (userInput == 2) {
        
    }
    return 0;
}
