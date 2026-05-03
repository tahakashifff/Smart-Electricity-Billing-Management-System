#include <iostream>
#include "Consumer.h"
using namespace std;

Consumer::Consumer()
{
    ConsumerID = 0;
    name = "";
    unitConsumed = 0;
}

Consumer::Consumer(int id, string n , float unitC)
{
    ConsumerID = id;
    name = n;
    unitConsumed = unitC;
}

Consumer::~Consumer(){}

int Consumer::getID()
{
    return ConsumerID;
}

string Consumer::getName()
{
    return name;
}

float Consumer::getunitConsumed()
{
    return unitConsumed;
}

string Consumer::Display()
{

    string result = "";

    result += "ID: " + to_string(ConsumerID) + "\n";
    result += "Name: " + name + "\n";
    result += "Units Consumed: " + to_string(unitConsumed) + "\n";

    return result;
}

ResidentialConsumer::ResidentialConsumer(int id, string n, float unitC, float peak, float offPeak)
: Consumer(id, n, unitC)
{
    peakUnits = peak;
    offPeakUnits = offPeak;
}

float ResidentialConsumer::calculateBill()
{
    float bill;
    return bill = (peakUnits * 20) + (offPeakUnits * 15);
}

CommercialConsumer::CommercialConsumer(int id, string n, float unitC) : Consumer(id, n, unitC)
{
    commercialUnits = unitC;
}

float CommercialConsumer::calculateBill()
{
    float bill;
    return bill = commercialUnits * 25;
}

SolarConsumer::SolarConsumer(int id, string n, float unitC,float peak, float offPeak, float unitEx) : ResidentialConsumer(id, n, unitC, peak, offPeak)
{
    unitExported = unitEx;
}

float SolarConsumer::calculateBill()
{
    float netUnits;
    netUnits = unitConsumed - unitExported;

    float rate = 10;

    if (netUnits <= 0)
        return 0;

    return netUnits * rate;
}





