#include <iostream>
#include <sstream>
#include <algorithm>
#include "Admin.h"
using namespace std;

string displayName(string name)
{
    replace(name.begin(), name.end(), '_', ' ');

    return name;
}

string ftos(float v)
{
    ostringstream ss;
    ss << fixed;
    ss.precision(2);
    ss << v;
    return ss.str();
}

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
    result += "Name: " + displayName(name) + "\n";
    result += "Units Consumed: " + ftos(unitConsumed) + "\n";

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
    return bill = (peakUnits * Admin::residentialPeakRate) + (offPeakUnits * Admin::residentialOffPeakRate);
}

CommercialConsumer::CommercialConsumer(int id, string n, float unitC) : Consumer(id, n, unitC)
{
    commercialUnits = unitC;
}

float CommercialConsumer::calculateBill()
{
    float bill;
    return bill = commercialUnits * Admin::commercialRate;
}

SolarConsumer::SolarConsumer(int id, string n, float unitC,float peak, float offPeak, float unitEx) : ResidentialConsumer(id, n, unitC, peak, offPeak)
{
    unitExported = unitEx;
}

float SolarConsumer::calculateBill()
{
    float netUnits = unitConsumed - unitExported;

    return netUnits * Admin::solarRate;
}

string Consumer::saveData()
{
    string data = "";

    data += to_string(ConsumerID) + " ";
    data += name + " ";
    data += ftos(unitConsumed) + " ";

    return data;
}

string ResidentialConsumer::saveData()
{
    string data = "Residential ";

    data += Consumer::saveData();

    data += ftos(peakUnits) + " ";
    data += ftos(offPeakUnits);

    return data;
}

string CommercialConsumer::saveData()
{
    string data = "Commercial ";

    data += Consumer::saveData();

    return data;
}

string SolarConsumer::saveData()
{
    string data = "Solar ";

    data += Consumer::saveData();

    data += ftos(peakUnits) + " ";
    data += ftos(offPeakUnits) + " ";
    data += ftos(unitExported);

    return data;
}

string formatName(string name)
{
    replace(name.begin(), name.end(), ' ', '_');

    return name;
}

void ResidentialConsumer::updateUnits(float units, float peak, float offPeak, float extra)
{
    (void)extra;
    unitConsumed = units;
    peakUnits = peak;
    offPeakUnits = offPeak;
}

void CommercialConsumer::updateUnits(float units, float peak, float offPeak, float extra)
{
    (void)peak;
    (void)offPeak;
    (void)extra;
    unitConsumed = units;
    commercialUnits = units;
}

void SolarConsumer::updateUnits(float units, float peak, float offPeak, float extra)
{
    unitConsumed = units;
    peakUnits = peak;
    offPeakUnits = offPeak;
    unitExported = extra;
}
