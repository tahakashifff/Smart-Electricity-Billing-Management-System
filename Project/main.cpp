#include <iostream>
#include "Admin.h"

using namespace std;

int main() {
    Admin admin;
    int choice;

        cout << "-----SMART ELECTRICITY PORTAL-----" << endl;
        cout << endl;

    do {
        cout << "1. Add Consumer" << endl;
        cout << "2. Display All" << endl;
        cout << "3. Search Consumer" <<endl;
        cout << "4. Generate Bill" << endl;
        cout << "5. Set Rates" << endl;
        cout << "6. Exit" << endl;

        cin >> choice;

        switch(choice) {
            case 1:
                admin.addConsumer();
                break;
            case 2:
                admin.displayAll();
                break;
            case 3:
            {
                int id;
                cout << "Enter ID to search: ";
                cin >> id;

                Consumer* c = admin.searchConsumerbyID(id);

                if (c != NULL)
                {
                  cout << c->Display();
                }
                else
                {
                  cout << "Consumer not found!\n";
                }
                break;
            }
            case 4:
                admin.generateBill();
                break;
            case 5:
            {
                float peak,offpeak,commercial,solar;

                cout << "Enter Residential Peak Rate: ";
                cin >> peak;

                cout << "Enter Residential Off-Peak Rate: ";
                cin >> offpeak;

                cout << "Enter Commercial Rate: ";
                cin >> commercial;

                cout << "Enter Solar Rate: ";
                cin >> solar;

                admin.setRates(peak,offpeak,commercial,solar);
                break;
            }
            default:
                break;
        }

    } while(choice != 6);

    return 0;
}