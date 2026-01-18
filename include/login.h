#ifndef LOGIN_H
#define LOGIN_H
#include <sqlite3.h>
#include <string>
class Login {
    public:
    void login(sqlite3 *db);

    private:
    std::string username;
    std::string password;
    bool done = 0;
};
#endif