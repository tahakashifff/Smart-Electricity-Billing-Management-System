#include "LoginManager.h"

LoginManager::LoginManager()
{
    username = "";
    password = "";
    linkedConsumerID = 0;
}

LoginManager::LoginManager(string u, string p, int id)
{
    username = u;
    password = p;
    linkedConsumerID = id;
}

LoginManager::~LoginManager(){}

bool LoginManager::login(string u, string p) const
{
    if (username == u && password == p)
    {
        return true;
    }

    return false;
}

int LoginManager::getConsumerID() const
{
    return linkedConsumerID;
}

string LoginManager::getUsername() const
{
    return username;
}

string LoginManager::saveData() const
{
    string data = "";

    data += username + " ";
    data += password + " ";
    data += to_string(linkedConsumerID);

    return data;
}
