#pragma once
#include <string>
using namespace std;

class LoginManager{
    private:
       string username;
       string password;
       int linkedConsumerID;

    public:
       LoginManager();
       ~LoginManager();
       LoginManager(string u, string p, int id);
       bool login(string u, string p) const;
       int getConsumerID() const;
       string getUsername() const;
       string saveData() const;
};