#ifndef SIGNUP_H
#define SIGNUP_H
#include <string>
#include <sqlite3.h>
class Signup {
    public:
    signup(sqlite3* db);

    private:
    std::string username, password;

}
#endif