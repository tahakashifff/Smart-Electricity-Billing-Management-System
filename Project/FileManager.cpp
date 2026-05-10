#include <iostream>
#include <fstream>
#include "FileManager.h"
#include "Admin.h"

using namespace std;

void FileManager::saveRates()
{
    ofstream rateValues("rates.txt", ios::out);

    if (!rateValues)
    {
        cout << "Error: Could not open rates.txt for writing!\n";
        return;
    }

    rateValues << Admin::residentialPeakRate << " ";
    rateValues << Admin::residentialOffPeakRate << " ";
    rateValues << Admin::commercialRate << " ";
    rateValues << Admin::solarRate << "\n";

    rateValues.close();
}

void FileManager::loadRates()
{
    ifstream rateFile("rates.txt");

    if (!rateFile)
    {
        return;
    }

    rateFile >> Admin::residentialPeakRate;
    rateFile >> Admin::residentialOffPeakRate;
    rateFile >> Admin::commercialRate;
    rateFile >> Admin::solarRate;

    rateFile.close();
}

void FileManager::saveConsumers(Consumer* consumer[], int count)
{
    ofstream consumerData("Consumers.txt");

    if (!consumerData)
    {
        cout << "Error: Could not open Consumers.txt for writing!\n";
        return;
    }

    for (int i = 0; i < count; i++)
    {
        consumerData << consumer[i]->saveData() << "\n";
    }

    consumerData.close();
}

void FileManager::loadConsumers(Consumer* consumer[], int& count)
{
    ifstream file("Consumers.txt");

    if (!file)
    {
        return;
    }

    string type;

    while (file >> type && count < 100)
    {
        if (type == "Residential")
        {
            int id;
            string name;
            float units, peak, offPeak;

            if (!(file >> id >> name >> units >> peak >> offPeak))
                break;

            consumer[count] = new ResidentialConsumer(id, name, units, peak, offPeak);
            count++;
        }
        else if (type == "Commercial")
        {
            int id;
            string name;
            float units;

            if (!(file >> id >> name >> units))
                break;

            consumer[count] = new CommercialConsumer(id, name, units);
            count++;
        }
        else if (type == "Solar")
        {
            int id;
            string name;
            float units, peak, offPeak, exported;

            if (!(file >> id >> name >> units >> peak >> offPeak >> exported))
                break;

            consumer[count] = new SolarConsumer(id, name, units, peak, offPeak, exported);
            count++;
        }
        else
        {
            string rest;
            getline(file, rest);
        }
    }

    file.close();
}
