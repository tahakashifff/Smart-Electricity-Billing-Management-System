#include <iostream>
#include "Consumer.cpp"

using namespace std;

int main()
{
    Consumer* c;

    c = new ResidentialConsumer(1, "Ayan", 100, 40, 60);
    cout << "---- Residential Consumer ----" << endl;
    cout << c->Display();
    cout << "Bill: " << c->calculateBill() << endl << endl;

    delete c;

    c = new CommercialConsumer(2, "Ali", 200);
    cout << "---- Commercial Consumer ----" << endl;
    cout << c->Display();
    cout << "Bill: " << c->calculateBill() << endl << endl;

    delete c;

    c = new SolarConsumer(3, "Hamza", 100, 50, 50, 30);
    cout << "---- Solar Consumer ----" << endl;
    cout << c->Display();
    cout << "Bill: " << c->calculateBill() << endl << endl;

    delete c;

    return 0;
}