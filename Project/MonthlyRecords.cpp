#include "MonthlyRecords.h"

MonthlyRecords::MonthlyRecords()
{
    consumerID = 0;
    month = "";
    amount = 0;
    paid = false;
}

MonthlyRecords::MonthlyRecords(int id, string m, float a)
{
    consumerID = id;
    month = m;
    amount = a;
    paid = false;
}

MonthlyRecords::~MonthlyRecords(){}

string MonthlyRecords::saveRecord()
{
    string data = "";

    data += to_string(consumerID) + " ";
    data += month + " ";
    data += to_string(amount) + " ";
    data += (paid ? "1" : "0");

    return data;
}

int MonthlyRecords::getConsumerID()
{
    return consumerID;
}

string MonthlyRecords::getMonth()
{
    return month;
}

float MonthlyRecords::getAmount()
{
    return amount;
}

bool MonthlyRecords::isPaid()
{
    return paid;
}

void MonthlyRecords::payBill()
{
    paid = true;
}
