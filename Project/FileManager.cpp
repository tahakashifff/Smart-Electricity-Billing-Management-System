#include <iostream>
#include <fstream>
#include <filesystem>
#include "FileManager.h"
#include "Admin.h"

using namespace std;
namespace fs = std::filesystem;

static fs::path locateDataRoot()
{
    fs::path dir = fs::current_path();

    while (true)
    {
        if (fs::exists(dir / "users.txt") || fs::exists(dir / "Consumers.txt") || fs::exists(dir / "rates.txt") || fs::exists(dir / "bills.txt"))
            return dir;

        if (fs::exists(dir / "Project"))
            return dir / "Project";

        if (!dir.has_parent_path() || dir.parent_path() == dir)
            break;

        dir = dir.parent_path();
    }

    return fs::current_path();
}

static fs::path dataReadPath(const string& fileName)
{
    fs::path root = locateDataRoot();

    if (fs::exists(root / fileName))
        return root / fileName;

    if (fs::exists(root / "Project" / fileName))
        return root / "Project" / fileName;

    return root / fileName;
}

static fs::path dataWritePath(const string& fileName)
{
    fs::path root = locateDataRoot();

    if (fs::exists(root / fileName))
        return root / fileName;

    if (fs::exists(root / "Project"))
        return root / "Project" / fileName;

    return root / fileName;
}

void FileManager::saveRates()
{
    ofstream rateValues(dataWritePath("rates.txt"), ios::out);

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
    ifstream rateFile(dataReadPath("rates.txt"));

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
    ofstream consumerData(dataWritePath("Consumers.txt"));

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
    ifstream file(dataReadPath("Consumers.txt"));

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

void FileManager::saveUsers(LoginManager users[], int count)
{
    ofstream file(dataWritePath("users.txt"));

    for (int i = 0; i < count; i++)
    {
        file << users[i].saveData() << endl;
    }

    file.close();
}

void FileManager::loadUsers(LoginManager users[], int& count)
{
    ifstream file(dataReadPath("users.txt"));

    if (!file)
    {
        return;
    }

    string username;
    string password;
    int id;

    while (file >> username >> password >> id)
    {
        users[count] = LoginManager(username, password, id);

        count++;
    }

    file.close();
}

void FileManager::saveBillRecord(MonthlyRecords record)
{
    ofstream file(dataWritePath("bills.txt"), ios::app);

    file << record.saveRecord() << endl;

    file.close();
}

void FileManager::viewBillHistory(int consumerID)
{
    ifstream file(dataReadPath("bills.txt"));

    if (!file)
    {
        cout << "No bill records found!" << endl;
        return;
    }

    int id;
    string month;
    float amount;
    bool paid;
    bool found = false;
    float totalUnpaid = 0;
    int unpaidMonths = 0;

    while (file >> id >> month >> amount >> paid)
    {
        if (id == consumerID)
        {
            found = true;
            cout << "Month: " << month << endl;

            if (amount < 0)
                cout << "Credit: " << -amount << " Rs (company owes you)" << endl;
            else
                cout << "Amount: " << amount << " Rs" << endl;

            if (paid)
                cout << "Status: PAID" << endl;
            else
            {
                if (amount < 0)
                    cout << "Status: CREDIT PENDING" << endl;
                else
                {
                    cout << "Status: UNPAID" << endl;
                    totalUnpaid += amount;
                    unpaidMonths++;
                }
            }

            cout << "----------------------" << endl;
        }
    }

    file.close();

    if (!found)
    {
        cout << "No bill records found for this consumer!" << endl;
        return;
    }

    if (unpaidMonths > 0)
        cout << "Remaining Due: " << totalUnpaid << " Rs (" << unpaidMonths << " month(s) unpaid)" << endl;
    else
        cout << "All bills paid!" << endl;
}

void FileManager::payConsumerBill(int consumerID, string month)
{
    ifstream file(dataReadPath("bills.txt"));

    if (!file)
    {
        cout << "No bill records found!" << endl;
        return;
    }

    ofstream temp(dataWritePath("temp.txt"));

    int id;
    string m;
    float amount;
    bool paid;
    bool found = false;
    bool alreadyPaid = false;

    while (file >> id >> m >> amount >> paid)
    {
        if (id == consumerID && m == month)
        {
            found = true;
            if (paid)
                alreadyPaid = true;
            else
                paid = true;
        }

        temp << id << " " << m << " " << amount << " " << paid << endl;
    }

    file.close();
    temp.close();

    remove(dataWritePath("bills.txt").string().c_str());
    rename(dataWritePath("temp.txt").string().c_str(), dataWritePath("bills.txt").string().c_str());

    if (!found)
        cout << "No bill record found for month: " << month << endl;
    else if (alreadyPaid)
        cout << "Bill for " << month << " is already paid!" << endl;
    else
        cout << "Bill for " << month << " paid successfully!" << endl;
}

float FileManager::getUnpaidTotal(int consumerID)
{
    ifstream file(dataReadPath("bills.txt"));

    if (!file)
        return 0;

    int id;
    string month;
    float amount;
    bool paid;
    float total = 0;

    while (file >> id >> month >> amount >> paid)
    {
        if (id == consumerID && !paid && amount > 0)
            total += amount;
    }

    file.close();
    return total;
}

float FileManager::getPaidTotal(int consumerID)
{
    ifstream file(dataReadPath("bills.txt"));

    if (!file)
        return 0;

    int id;
    string month;
    float amount;
    bool paid;
    float total = 0;

    while (file >> id >> month >> amount >> paid)
    {
        if (id == consumerID && paid)
            total += amount;
    }

    file.close();
    return total;
}

bool FileManager::billExists(int consumerID, string month)
{
    ifstream file(dataReadPath("bills.txt"));

    if (!file)
        return false;

    int id;
    string m;
    float amount;
    bool paid;

    while (file >> id >> m >> amount >> paid)
    {
        if (id == consumerID && m == month)
        {
            file.close();
            return true;
        }
    }

    file.close();
    return false;
}

float FileManager::getCreditTotal(int consumerID)
{
    ifstream file(dataReadPath("bills.txt"));

    if (!file)
        return 0;

    int id;
    string month;
    float amount;
    bool paid;
    float total = 0;

    while (file >> id >> month >> amount >> paid)
    {
        if (id == consumerID && amount < 0)
            total += -amount;
    }

    file.close();
    return total;
}
