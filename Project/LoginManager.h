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
       bool login(string u, string p);
       int getConsumerID();
       string getUsername();
       string saveData();
};