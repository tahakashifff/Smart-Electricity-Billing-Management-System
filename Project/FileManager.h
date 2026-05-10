#pragma once
#include <iostream>
#include <fstream>
#include "Consumer.h"
#include "Admin.h"
#include "LoginManager.h"
#include "MonthlyRecords.h"


class FileManager
{
public:
    static void saveRates();
    static void loadRates();
    static void saveConsumers(Consumer* consumer[], int count);
    static void loadConsumers(Consumer* consumer[], int& count);
    static void saveUsers(LoginManager users[], int count);
    static void loadUsers(LoginManager users[], int& count);
    static void saveBillRecord(MonthlyRecords record);
    static void viewBillHistory(int consumerID);
    static void payConsumerBill(int consumerID, string month);
    static float getUnpaidTotal(int consumerID);
    static float getPaidTotal(int consumerID);
    static float getCreditTotal(int consumerID);
    static bool billExists(int consumerID, string month);

};