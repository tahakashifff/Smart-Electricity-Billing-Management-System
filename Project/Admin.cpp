#include <iostream>
#include "Input.h"
#include "Admin.h"
#include "Consumer.h"
#include "MonthlyRecords.h"
#include "FileManager.h"
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

    if (searchConsumerbyID(id) != NULL)
    {
        cout << "Consumer with ID " << id << " already exists!" << endl;
        return;
    }

    string name  = inputName("Enter Name: ");
    string month = inputMonth("Enter Month for This Reading (Example: May-2026): ");
    float  units = inputFloat("Enter Units Consumed: ");

    name = formatName(name);

    if (type == 1)
    {
        float peak, offPeak;

        peak = inputPeakUnits("Enter Peak Units: ", units);
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

        peak = inputPeakUnits("Enter Peak Units: ", units);
        offPeak  = units - peak;
        exported = inputFloat("Enter Units Exported: ");

        consumer[count] = new SolarConsumer(id, name, units, peak, offPeak, exported);
    }

    else
    {
        cout << "Invalid type!" << endl;
        return;
    }

    float bill = consumer[count]->calculateBill();

    if (FileManager::billExists(id, month))
    {
        cout << "A bill for " << month << " already exists for this consumer! Bill record not saved." << endl;
    }
    else
    {
        MonthlyRecords record(id, month, bill);
        FileManager::saveBillRecord(record);
        if (bill < 0)
            cout << "Credit for " << month << ": " << -bill << " Rs (company owes consumer)" << endl;
        else
            cout << "Bill for " << month << ": " << bill << " Rs (saved to bills.txt)" << endl;
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
        float unpaid = FileManager::getUnpaidTotal(consumer[i]->getID());
        float paid   = FileManager::getPaidTotal(consumer[i]->getID());
        float credit = FileManager::getCreditTotal(consumer[i]->getID());

        cout << "------------------------\n";
        cout << consumer[i]->Display();
        cout << "Total Paid:   " << paid   << " Rs" << endl;
        cout << "Total Unpaid: " << unpaid << " Rs" << endl;
        if (credit > 0)
            cout << "Credit Due:   " << credit << " Rs (company owes consumer)" << endl;
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

void Admin::registerUser(LoginManager users[], int& userCount)
{
    string username;
    string password;

    int consumerID;

    cout << "Enter Username: ";
    cin >> username;

    cout << "Enter Password: ";
    cin >> password;

    consumerID = inputInt("Enter Consumer ID: ");

    Consumer* c = searchConsumerbyID(consumerID);

    if (c == NULL)
    {
        cout << "Consumer does not exist!" << endl;
        return;
    }

    users[userCount] = LoginManager(username, password, consumerID);

    userCount++;

    cout << "User registered successfully!"
         << endl;
}

void Admin::updateConsumer()
{
    int id = inputInt("Enter Consumer ID to Update: ");

    Consumer* c = searchConsumerbyID(id);

    if (c == NULL)
    {
        cout << "Consumer not found!" << endl;
        return;
    }

    string month = inputMonth("Enter Month for This Reading (Example: May-2026): ");

    if (FileManager::billExists(id, month))
    {
        cout << "A bill for " << month << " already exists for this consumer!" << endl;
        return;
    }

    cout << "------ Current Details ------" << endl;
    cout << c->Display();
    cout << "Current Bill: " << c->calculateBill() << " Rs" << endl;

    float units = inputFloat("Enter New Units Consumed: ");

    SolarConsumer* sc = dynamic_cast<SolarConsumer*>(c);
    ResidentialConsumer* rc = dynamic_cast<ResidentialConsumer*>(c);

    float peak = 0, offPeak = 0, extra = 0;

    if (sc != NULL)
    {
        peak = inputPeakUnits("Enter Peak Units: ", units);
        offPeak = units - peak;
        extra = inputFloat("Enter Units Exported: ");
    }
    else if (rc != NULL)
    {
        peak = inputPeakUnits("Enter Peak Units: ", units);
        offPeak = units - peak;
    }

    c->updateUnits(units, peak, offPeak, extra);

    float newBill = c->calculateBill();

    MonthlyRecords record(c->getID(), month, newBill);
    FileManager::saveBillRecord(record);

    cout << "Consumer updated successfully!" << endl;

    if (newBill < 0)
        cout << "Credit for " << month << ": " << -newBill << " Rs (company owes consumer)" << endl;
    else
        cout << "Bill for " << month << ": " << newBill << " Rs (saved to bills.txt)" << endl;
}

void Admin::showStatistics(){
    if (count == 0)
    {
        cout << "No consumers available!" << endl;
        return;
    }

    float totalCollected = 0;
    float totalOutstanding = 0;

    float highestUnpaid = FileManager::getUnpaidTotal(consumer[0]->getID());
    float lowestUnpaid  = highestUnpaid;

    int highestID = consumer[0]->getID();
    int lowestID  = consumer[0]->getID();

    for (int i = 0; i < count; i++)
    {
        float unpaid = FileManager::getUnpaidTotal(consumer[i]->getID());
        float paid   = FileManager::getPaidTotal(consumer[i]->getID());

        totalCollected   += paid;
        totalOutstanding += unpaid;

        if (unpaid > highestUnpaid)
        {
            highestUnpaid = unpaid;
            highestID = consumer[i]->getID();
        }

        if (unpaid < lowestUnpaid)
        {
            lowestUnpaid = unpaid;
            lowestID = consumer[i]->getID();
        }
    }

    cout << "\n===== SYSTEM STATISTICS =====" << endl;
    cout << "Total Consumers:     " << count            << endl;
    cout << "Total Collected:     " << totalCollected   << " Rs" << endl;
    cout << "Total Outstanding:   " << totalOutstanding << " Rs" << endl;
    cout << "Highest Unpaid Bill: " << highestUnpaid    << " Rs (ID: " << highestID << ")" << endl;
    cout << "Lowest Unpaid Bill:  " << lowestUnpaid     << " Rs (ID: " << lowestID  << ")" << endl;
}