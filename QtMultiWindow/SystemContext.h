#pragma once

#include <QString>
#include "Admin.h"

class SystemContext : public Admin
{
private:
    LoginManager users[100];
    int userCount;

    static QString money(float value);
    int findUserIndex(const QString& username) const;
    bool monthExistsForConsumer(int consumerID, const QString& month) const;

public:
    SystemContext();

    bool adminLogin(const QString& username, const QString& password) const;
    int userLogin(const QString& username, const QString& password) const;

    QString addConsumerText(int id, const QString& name, const QString& month, int type, float units, float peakUnits, float exportedUnits);
    QString updateConsumerText(int id, const QString& month, float units, float peakUnits, float exportedUnits);
    QString displayAllConsumersText();
    QString searchConsumerText(int id);
    QString setRatesText(float peakRate, float offPeakRate, float commercialRate, float solarRate);
    QString registerUserText(const QString& username, const QString& password, int consumerID);
    QString searchUserText(const QString& username) const;
    QString statisticsText();

    QString viewCurrentBillText(int consumerID);
    QString billHistoryText(int consumerID);
    QString payBillText(int consumerID, const QString& month);
};
