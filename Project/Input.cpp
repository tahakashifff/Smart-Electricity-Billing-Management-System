 #include <iostream>
#include <string>
#include <cctype>
#include "Input.h"
using namespace std;

int inputInt(string prompt)
{
    int value;
    while (true)
    {
        cout << prompt;
        cin >> value;

        if (cin.good())
        {
            cin.ignore(1000, '\n');
            return value;
        }

        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Invalid input! Please enter a whole number." << endl;
    }
}

float inputFloat(string prompt)
{
    float value;
    while (true)
    {
        cout << prompt;
        cin >> value;

        if (cin.good())
        {
            cin.ignore(1000, '\n');
            return value;
        }

        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Invalid input! Please enter a number." << endl;
    }
}

string inputName(string prompt)
{
    string name;
    while (true)
    {
        cout << prompt;
        getline(cin, name);

        if (name.empty())
        {
            cout << "Name cannot be empty!" << endl;
            continue;
        }

        bool valid = true;
        for (size_t i = 0; i < name.length(); i++)
        {
            if (!isalpha(name[i]) && name[i] != ' ')
            {
                valid = false;
                break;
            }
        }

        if (valid)
            return name;

        cout << "Invalid name! Use letters and spaces only." << endl;
    }
}

string inputMonth(string prompt)
{
    string month;
    while (true)
    {
        cout << prompt;
        getline(cin, month);

        if (month.empty())
        {
            cout << "Month cannot be empty!" << endl;
            continue;
        }

        bool valid = true;
        for (size_t i = 0; i < month.length(); i++)
        {
            if (!isalpha(month[i]) && !isdigit(month[i]) && month[i] != '-')
            {
                valid = false;
                break;
            }
        }

        if (valid)
            return month;

        cout << "Invalid month! Use format like May-2026." << endl;
    }
}

int inputIntRange(string prompt, int min, int max){
    while (true)
    {
        int value = inputInt(prompt);

        if (value >= min && value <= max)
            return value;

        cout << "Invalid choice! Please enter a number between " << min << " and " << max << "." << endl;
    }
}

float inputPeakUnits(string prompt, float totalUnits)
{
    while (true)
    {
        float peak = inputFloat(prompt);

        if (peak < 0)
        {
            cout << "Peak units cannot be negative!" << endl;
            continue;
        }

        if (peak > totalUnits)
        {
            cout << "Peak units (" << peak << ") cannot exceed total units consumed (" << totalUnits << ")!" << endl;
            continue;
        }

        return peak;
    }
}
