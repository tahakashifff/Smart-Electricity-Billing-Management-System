#pragma once
#include "Consumer.h"
#include "LoginManager.h"
using namespace std;

class Admin{
    protected:
       Consumer* consumer[100];
       int count;
       
    public:
       static float residentialPeakRate;
       static float residentialOffPeakRate;
       static float commercialRate;
       static float solarRate;
       Admin();
       void addConsumer();
       Consumer* searchConsumerbyID(int id);
       void setRates(float peak, float offpeak , float commercial , float solar);
       void displayAll();
       Consumer** getConsumer();
       int& getCount();
       void registerUser(LoginManager[], int& userCount);
       void showStatistics();
       void updateConsumer();
};