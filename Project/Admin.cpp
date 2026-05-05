#include <iostream>
#include "Admin.h"
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
    if (count>100)
    {
        cout << "Storage full"<<endl;
        return;
    }

    int type;
    cout << "Select Consumer Type:" << endl;;
    cout << "1. Residential" << endl;
    cout << "2. Commercial" << endl;
    cout << "3. Solar" << endl;
    cin >> type;

    int id;
    string name;
    float units;

    cout << "Enter ID: ";
    cin >> id;
    
    cin.ignore();
    cout << "Enter Name: ";
    getline(cin,name);
   

    cout << "Enter Units Consumed: ";
    cin >> units;

    if (type == 1)
    {
        float peak, offPeak;

        cout << "Enter Peak Units: ";
        cin >> peak;

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

        cout << "Enter Peak Units: ";
        cin >> peak;

        offPeak = units - peak;

        cout << "Enter Units Exported: ";
        cin >> exported;

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
    cout << "Enter Consumer ID: ";
    cin >> id;

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