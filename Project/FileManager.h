#pragma once
using namespace std;
#include "Consumer.h"

class FileManager
{
public:
    static void saveRates();
    static void loadRates();
    static void saveConsumers(Consumer* consumer[], int count);
    static void loadConsumers(Consumer* consumer[], int& count);
};