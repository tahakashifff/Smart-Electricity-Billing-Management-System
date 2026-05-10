#pragma once
#include <string>
using namespace std;

class MonthlyRecords
{
private:
    int consumerID;
    string month;
    float amount;
    bool paid;

public:
    MonthlyRecords();
    MonthlyRecords(int id, string m, float a);
    ~MonthlyRecords();
    string saveRecord();
    int getConsumerID();
    string getMonth();
    float getAmount();
    bool isPaid();
    void payBill();
};
