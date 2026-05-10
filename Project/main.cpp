#include <iostream>
#include <iomanip>
#include "Input.h"
#include "Admin.h"
#include "FileManager.h"
#include "LoginManager.h"

using namespace std;

int main()
{
    cout << fixed << setprecision(2);

    Admin admin;
    LoginManager users[100];
    int userCount = 0;

    FileManager::loadRates();
    FileManager::loadConsumers(admin.getConsumer(),admin.getCount());
    FileManager::loadUsers(users, userCount);

    int mainChoice;

    cout << "===== SMART ELECTRICITY SYSTEM ====="<<endl;
    cout<<endl;

    do
    {
        cout << "\n===== LOGIN MENU =====" << endl;

        cout << "1. Admin Login" << endl;
        cout << "2. User Login" << endl;
        cout << "3. Exit Program" << endl;

        mainChoice = inputIntRange("Enter Choice: ", 1, 3);

        if (mainChoice == 1)
        {
            string username;
            string password;

            cout << "Enter Username: ";
            cin >> username;

            cout << "Enter Password: ";
            cin >> password;

            if (username == "admin" && password == "admin123")
            {
                cout << "\nAdmin Login Successful!" << endl;

                int choice;

                do
                {
                    cout << "\n===== ADMIN MENU =====" << endl;

                    cout << "1. Add Consumer" << endl;
                    cout << "2. Display All Consumers" << endl;
                    cout << "3. Search Consumer By ID" << endl;
                    cout << "4. Update Consumer Units" << endl;
                    cout << "5. Set Rates" << endl;
                    cout << "6. Register User" << endl;
                    cout << "7. Show Statistics" << endl;
                    cout << "8. Exit Admin Menu" << endl;

                    choice = inputIntRange("Enter Choice: ",1, 8);

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
                                cout << "Consumer not found!"<< endl;
                            }
                            else
                            {
                                float unpaid = FileManager::getUnpaidTotal(id);
                                float paid   = FileManager::getPaidTotal(id);
                                float credit = FileManager::getCreditTotal(id);

                                cout << c->Display();
                                cout << "Total Paid:   " << paid   << " Rs" << endl;
                                cout << "Total Unpaid: " << unpaid << " Rs" << endl;
                                if (credit > 0)
                                    cout << "Credit Due:   " << credit << " Rs (company owes consumer)" << endl;
                            }
                            break;
                        }

                        case 4:
                        {
                            admin.updateConsumer();

                            FileManager::saveConsumers(admin.getConsumer(), admin.getCount());

                            break;
                        }

                        case 5:
                        {
                            float peak;
                            float offpeak;
                            float commercial;
                            float solar;

                            peak = inputFloat("Enter Residential Peak Rate: ");
                            offpeak = inputFloat("Enter Residential Off-Peak Rate: ");
                            commercial = inputFloat("Enter Commercial Rate: ");
                            solar = inputFloat("Enter Solar Rate: ");

                            admin.setRates(peak,offpeak,commercial,solar);

                            FileManager::saveRates();

                            cout << "Rates updated successfully!"<< endl;
                            break;
                        }

                        case 6:
                        {
                            admin.registerUser(users, userCount);

                            FileManager::saveUsers(users,userCount);

                            break;
                        }

                        case 7:
                        {
                            admin.showStatistics();

                            break;
                        }

                        case 8:
                        {
                            FileManager::saveConsumers(admin.getConsumer(),admin.getCount());

                            FileManager::saveRates();
                            FileManager::saveUsers(users,userCount);

                            cout << "Admin Menu Closed!"<< endl;
                            break;
                        }

                        default:
                        {
                            cout << "Invalid Choice!"
                                 << endl;
                        }
                    }

                } while (choice != 8);
            }
            else
            {
                cout << "Invalid Admin Credentials!"<< endl;
            }
        }

        else if (mainChoice == 2)
        {
            string username;
            string password;

            cout << "Enter Username: ";
            cin >> username;

            cout << "Enter Password: ";
            cin >> password;

            bool found = false;

            for (int i = 0; i < userCount; i++)
            {
                if (users[i].login(username, password))
                {
                    found = true;

                    cout << "\nUser Login Successful!"<< endl;

                    int consumerID = users[i].getConsumerID();

                    Consumer* c = admin.searchConsumerbyID(consumerID);

                    int userChoice;

                    do
                    {
                        cout << "\n===== USER MENU ====="<< endl;

                        cout << "1. View Bill" << endl;
                        cout << "2. View Bill History"<< endl;
                        cout << "3. Pay Bill"<<endl;
                        cout << "4. Exit User Menu"<< endl;

                        userChoice = inputIntRange("Enter Choice: ",1, 4);

                        switch (userChoice)
                        {
                            case 1:
                            {
                                if (c != NULL)
                                {
                                    cout << c->Display();

                                    cout << "Bill: "<< c->calculateBill()<< " Rs" << endl;
                                }
                                break;
                            }

                            case 2:
                            {
                                FileManager::viewBillHistory(consumerID);
                                break;
                            }

                            case 3:
                            {
                                string month;

                                month = inputMonth("Enter Month to Pay:");

                                FileManager::payConsumerBill(consumerID,month);
                                break;
                            }
                            

                            case 4:
                            {
                                cout << "User Menu Closed!"<< endl;
                                break;
                            }

                            default:
                            {
                                cout << "Invalid Choice!"<< endl;
                            }
                        }

                    } while (userChoice != 4);
                }
            }

            if (!found)
            {
                cout << "Invalid User Credentials!"<< endl;
            }
        }

        else if (mainChoice == 3)
        {
            FileManager::saveConsumers(admin.getConsumer(),admin.getCount());

            FileManager::saveRates();

            FileManager::saveUsers(users,userCount);

            cout << "Data saved successfully!"<< endl;

            cout << "Program Closed!"<< endl;
        }

    } while (mainChoice != 3);

    return 0;
}