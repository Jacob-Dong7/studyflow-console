#ifndef SUBJECT_H
#define SUBJECT_H
#include <string>
#include <sqlite3.h>

class Subject {
    public:
    void loadSubject(sqlite3*, std::string, std::string, int);
    void showSubject(sqlite3*, std::string, const char*);
    void createSubject(sqlite3*, int);
    void addSubject(sqlite3*, int, int);

    void deleteSubject(sqlite3*, std::string);
    bool checkSubject(sqlite3*, std::string);

    int getSubjectID(sqlite3*, std::string);
    int getID(sqlite3*, std::string);

    private:
    std::string subjectName;

};

#endif