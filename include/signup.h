#ifndef SIGNUP_H
#define SIGNUP_H
#include <string>
#include <sqlite3.h>
class Signup {
    public:
    bool signup(sqlite3* db);
    bool verifyName(std::string, sqlite3* db);
    bool verifyPassword(std::string);

    bool save(std::string, std::string, std::string, int, sqlite3*);

    private:
    std::string username, password, email;
    int uni;
    int rc;
    bool verify = 0;

};
#endif