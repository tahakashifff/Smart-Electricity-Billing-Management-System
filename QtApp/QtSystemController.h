#pragma once

#include <QString>
#include "Admin.h"

class QtSystemController : public Admin
{
private:
    LoginManager users[100];
    int userCount;

    static QString money(float value);
    static QString showName(const std::string& value);

public:
    QtSystemController();

    bool adminLogin(const QString& username, const QString& password) const;
    int userLogin(const QString& username, const QString& password) const;

    QString addConsumerRecord(int type, int id, const QString& name, const QString& month, float units, float peakUnits, float exportedUnits);
    QString registerUserRecord(const QString& username, const QString& password, int consumerID);
    QString setRatesRecord(float peak, float offpeak, float commercial, float solar);
    QString updateConsumerRecord(int id, const QString& month, float units, float peakUnits, float exportedUnits);

    QString searchConsumerText(int id);
    QString allConsumersText();
    QString statisticsText();
    QString billHistoryText(int consumerID);
    QString payBillText(int consumerID, const QString& month);
};
