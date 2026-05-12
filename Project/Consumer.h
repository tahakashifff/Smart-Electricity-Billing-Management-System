#pragma once
#include <string>
using namespace std;

class Consumer{
    protected:
       int ConsumerID;
       string name;
       float unitConsumed;

    public:
       Consumer();
       Consumer(int id , string n , float unitC);
       virtual ~Consumer();
       virtual float calculateBill()=0;
       int getID();
       string getName();
       float getunitConsumed();
       virtual string Display();
       virtual string saveData();
       virtual void updateUnits(float units, float peak, float offPeak, float extra) = 0;
};

class ResidentialConsumer : public Consumer{
    public:
       float peakUnits;
       float offPeakUnits;

       ResidentialConsumer(int id, string n, float unitC, float peak, float offPeak);
       float calculateBill()override;
       string saveData()override;
       void updateUnits(float units, float peak, float offPeak, float extra) override;
};

class CommercialConsumer : public Consumer{
    public:
       float commercialUnits;

       CommercialConsumer(int id, string n, float unitC);
       float calculateBill()override;
       string saveData()override;
       void updateUnits(float units, float peak, float offPeak, float extra) override;
};

class SolarConsumer : public ResidentialConsumer{
    public:
       float unitExported;
       
       SolarConsumer(int id, string n, float unitC,float peak, float offPeak, float unitEx);
       float calculateBill()override;
       string saveData()override;
       void updateUnits(float units, float peak, float offPeak, float extra) override;
};

string formatName(string name);
string displayName(string name);
string ftos(float v);