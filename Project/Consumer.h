using namespace std;

class Consumer{
    protected:
       int ConsumerID;
       string name;
       float unitConsumed;

    public:
       Consumer();
       Consumer(int id , string n , float unitC);
       ~Consumer();
       virtual float calculateBill()=0;
       int getID();
       string getName();
       float getunitConsumed();
       virtual string Display();
};

class ResidentialConsumer : public Consumer{
    public:
       float peakUnits;
       float offPeakUnits;

       ResidentialConsumer(int id, string n, float unitC, float peak, float offPeak);
       float calculateBill()override;
};

class CommercialConsumer : public Consumer{
    public:
       float commercialUnits;

       CommercialConsumer(int id, string n, float unitC);
       float calculateBill()override;
};

class SolarConsumer : public ResidentialConsumer{
    public:
       float unitExported;
       
       SolarConsumer(int id, string n, float unitC,float peak, float offPeak, float unitEx);
       float calculateBill()override;
};