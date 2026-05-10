#include <iostream>
#include "Input.h"
#include "Admin.h"
#include "Consumer.h"
using namespace std;

float Admin::residentialPeakRate = 20;
float Admin::residentialOffPeakRate = 15;
float Admin::commercialRate = 25;
float Admin::solarRate = 10;

Admin::Admin()
{
    count = 0;
}

void Admin::addConsumer()
{
    if (count >= 100)
    {
        cout << "Storage full"<<endl;
        return;
    }

    cout << "Select Consumer Type:"<< endl;
    cout << "1. Residential"<< endl;
    cout << "2. Commercial"<< endl;
    cout << "3. Solar"<< endl;

    int type = inputIntRange("Enter Choice: ", 1, 3);

    int id = inputInt("Enter ID: ");
    string name  = inputName("Enter Name: ");
    float  units = inputFloat("Enter Units Consumed: ");

    name = formatName(name);

    if (type == 1)
    {
        float peak, offPeak;

        peak= inputFloat("Enter Peak Units: ");
        offPeak = units - peak;

        consumer[count] = new ResidentialConsumer(id, name, units, peak, offPeak);
    }

    else if (type == 2)
    {
        consumer[count] = new CommercialConsumer(id, name, units);
    }

    else if (type == 3)
    {
        float peak, offPeak, exported;

        peak = inputFloat("Enter Peak Units: ");
        offPeak  = units - peak;
        exported = inputFloat("Enter Units Exported: ");

        consumer[count] = new SolarConsumer(id, name, units, peak, offPeak, exported);
    }

    else
    {
        cout << "Invalid type!" << endl;
        return;
    }

    count++;

    cout << "Consumer added successfully!" << endl;
}

void Admin::displayAll()
{
    if (count == 0)
    {
        cout << "No consumers found!\n";
        return;
    }

    for (int i = 0; i < count; i++)
    {
        cout << "------------------------\n";
        cout << consumer[i]->Display();
        cout << "Bill: " << consumer[i]->calculateBill() << " Rs" << endl;
    }
}

Consumer* Admin::searchConsumerbyID(int id)
{
    for (int i = 0; i < count; i++)
    {
        if (consumer[i]->getID() == id)
        {
            return consumer[i];
        }
    }

    return NULL;
}

void Admin::generateBill()
{
    int id;
    id = inputInt("Enter Consumer ID: ");

    Consumer* c = searchConsumerbyID(id);

    if (c == NULL)
    {
        cout << "Consumer not found!" << endl;
        return;
    }

    cout << "------ Consumer Details ------" << endl;
    cout << c->Display();
    cout << "Bill: " << c->calculateBill() << " Rs" << endl;
}

void Admin::setRates(float peak, float offpeak , float commercial , float solar)
{
    residentialPeakRate = peak;
    residentialOffPeakRate = offpeak;
    commercialRate = commercial;
    solarRate = solar;
}

Consumer** Admin::getConsumer()
{
    return consumer;
}

int& Admin::getCount()
{
    return count;
}