#ifndef DASHBOARD_H
#define DASHBOARD_H
#include <sqlite3.h>
#include <string>

class Dashboard {
    public:
    void dashboard(sqlite3* db, std::string, std::string);

    private:
    int userInput;
};


#endif