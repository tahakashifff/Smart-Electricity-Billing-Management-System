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

public:
    SystemContext();

    bool adminLogin(const QString& username, const QString& password) const;
    int userLogin(const QString& username, const QString& password) const;

    QString registerUserAndSaveUnits(const QString& username, const QString& password, int consumerID, int type, const QString& name, const QString& month, float units, float peakUnits, float exportedUnits);
    QString searchUserText(const QString& username) const;
    QString searchConsumerText(int id);
    QString billHistoryText(int consumerID);
    QString payBillText(int consumerID, const QString& month);
};
