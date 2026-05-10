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
        for (int i = 0; i < name.length(); i++)
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

int inputIntRange(string prompt, int min, int max)
{
    while (true)
    {
        int value = inputInt(prompt);

        if (value >= min && value <= max)
            return value;

        cout << "Invalid choice! Please enter a number between " << min << " and " << max << "." << endl;
    }
}
