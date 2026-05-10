#include <iostream>
#include <iomanip>
#include "Input.h"
#include "Admin.h"
#include "FileManager.h"

using namespace std;

int main()
{
    cout << fixed << setprecision(2);

    Admin admin;

    FileManager::loadRates();
    FileManager::loadConsumers(admin.getConsumer(), admin.getCount());

    int choice;

    do
    {
        cout << "===== Smart Electricity Billing System ====="<<endl;

        cout << "1. Add Consumer"<<endl;
        cout << "2. Display All Consumers"<<endl;
        cout << "3. Search Consumer By ID"<<endl;
        cout << "4. Generate Bill"<<endl;
        cout << "5. Set Rates"<<endl;
        cout << "6. Exit"<<endl;

        choice = inputIntRange("Enter Choice: ", 1, 6);

        switch (choice)
        {
            case 1:
            {
                admin.addConsumer();
                break;
            }
            case 2:
            {
                admin.displayAll();
                break;
            }
            case 3:
            {
                int id;
                id = inputInt("Enter ID: ");

                Consumer* c = admin.searchConsumerbyID(id);

                if (c == NULL)
                {
                    cout << "Consumer not found!"<<endl;
                }
                else
                {
                    cout << c->Display();
                    cout << "Bill: "<< c->calculateBill()<< endl;
                }

                break;
            }

            case 4:
            {
                admin.generateBill();
                break;
            }

            case 5:
            {
                float peak,offpeak,commercial,solar;

                peak = inputFloat("Enter Residential Peak Rate: ");
                offpeak = inputFloat("Enter Residential Off-Peak Rate: ");
                commercial = inputFloat("Enter Commercial Rate: ");
                solar = inputFloat("Enter Solar Rate: ");

                admin.setRates(peak , offpeak , commercial , solar);
                FileManager::saveRates();

                cout << "Rates updated successfully!"<<endl;

                break;
            }

            case 6:
            {
                FileManager::saveConsumers(admin.getConsumer() , admin.getCount());

                FileManager::saveRates();

                cout << "Data saved successfully!"<<endl;
                cout << "Exiting program..."<<endl;

                break;
            }

            default:
            {
                cout << "Invalid choice!"<<endl;
            }
        }

    } while (choice != 6);

    return 0;
}