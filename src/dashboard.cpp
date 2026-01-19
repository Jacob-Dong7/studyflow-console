#include "dashboard.h"
#include "subject.h"
#include <sqlite3.h>
#include <string>
#include <iostream>

void Dashboard::dashboard(sqlite3* db, std::string username, std::string password) {

    std::cout << "Dashboard\n\n";
    std::cout << "Options (Press corresponding numbers):\n" << "1(View Subjects)\n" << "2(Add Subjects)\n" << "3(Delete Subjects)\n" << "-1(Quit)\n";

    while (true) {
        std::cin >> userInput;

        if (userInput == -1) {
            std::cout << "Quitting...\n";
            return;
        } else if (userInput == 1 || userInput == 2 || userInput == 3){
            Subject subject;
            subject.loadSubject(db, username, password, userInput);
            return;
        }

    }
    return;
}
