#ifndef PROFILE_H
#define PROFILE_H
#include <string>
#include <sqlite3.h>
class Profile {
    public:
    void profile(sqlite3*, std::string);
    int getID(sqlite3*, std::string);

    void getProfile(sqlite3* db, int);
    private:
    std::string username, email, uni, state;

};
#endif