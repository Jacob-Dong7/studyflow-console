//.\build\studyflow.exe
#include <iostream>
#include <sqlite3.h>
#include "signup.h"
#include "login.h"


int main() {
    bool verify;
    sqlite3* db = nullptr;
    const char* dbPath = "studyflow.db";

    int rc = sqlite3_open(dbPath, &db);

    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    std::cout << "\nWelcome to StudyFlow\n\n";

    while (true) {
    int userInput;
    std::cout << "---------------------------\n";
    std::cout << "1(Login) 2(Signup) -1(Quit)\n";
    std::cout << "---------------------------\n";
    std::cin >> userInput;

    if (userInput == -1) {
        std::cout << "\nQuitting...\n";
        sqlite3_close(db);
        return 0;
    }

    //login & signup
    if (userInput == 1) {
        Login userLogin;
        userLogin.login(db);
    } else if (userInput == 2) {
        Signup userSignup;
        verify = userSignup.signup(db);

        if (verify == false) {
            std::cout << "Signup failed\n\n";
        } else {
            std::cout << "Signup complete\n\n";
        }
        continue;
    }


    }

    sqlite3_close(db);
    return 0;
}
