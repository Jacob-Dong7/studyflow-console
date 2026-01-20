#include "dashboard.h"
#include "profile.h"
#include "subject.h"
#include <sqlite3.h>
#include <string>
#include <iostream>

void Dashboard::dashboard(sqlite3* db, std::string username, std::string password) {
    std::cout << "\nDashboard\n";
    std::cout << "---------\n\n";

    while (true) {

        std::cout << "Options (Press corresponding numbers):\n" << "1(View Subjects)\n" << "2(Add Subjects)\n" << "3(Delete Subjects)\n" << "4(Tasks)\n" << "5(Calculate GPA)\n"  << "6(Profile)\n" << "-1(Back)\n\n";
        std::cin >> userInput;

        if (userInput == -1) {
            std::cout << "\nReturning...\n\n";
            return;
        } else if (userInput == 1 || userInput == 2 || userInput == 3){
            Subject subject;
            subject.loadSubject(db, username, password, userInput);
        } else if (userInput == 6) {
            Profile userProfile;
            userProfile.profile(db, username);
        }

    }
    return;
}
