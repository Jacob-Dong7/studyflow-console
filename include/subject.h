#ifndef SUBJECT_H
#define SUBJECT_H
#include <string>
#include <sqlite3.h>

class Subject {
    public:
    void loadSubject(sqlite3*, std::string, std::string, int);
    void showSubject(sqlite3*, std::string, const char*);
    int getID(sqlite3*, std::string);
    private:

};

#endif