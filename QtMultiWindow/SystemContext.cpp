#include "SystemContext.h"

#include "FileManager.h"
#include "MonthlyRecords.h"

#include <QFile>
#include <QTextStream>

SystemContext::SystemContext() : userCount(0)
{
    FileManager::loadRates();
    FileManager::loadConsumers(getConsumer(), getCount());
    FileManager::loadUsers(users, userCount);
}

QString SystemContext::money(float value)
{
    return QString::number(value, 'f', 2);
}

int SystemContext::findUserIndex(const QString& username) const
{
    for (int i = 0; i < userCount; ++i)
    {
        if (QString::fromStdString(users[i].getUsername()) == username)
            return i;
    }
    return -1;
}

bool SystemContext::adminLogin(const QString& username, const QString& password) const
{
    return username == "admin" && password == "admin123";
}

int SystemContext::userLogin(const QString& username, const QString& password) const
{
    for (int i = 0; i < userCount; ++i)
    {
        if (users[i].login(username.toStdString(), password.toStdString()))
            return users[i].getConsumerID();
    }
    return 0;
}

QString SystemContext::searchUserText(const QString& username) const
{
    int index = findUserIndex(username);
    if (index >= 0)
        return QString("Username: %1\nLinked Consumer ID: %2").arg(username).arg(users[index].getConsumerID());
    return "User not found.";
}

QString SystemContext::registerUserAndSaveUnits(const QString& username, const QString& password, int consumerID, int type, const QString& name, const QString& month, float units, float peakUnits, float exportedUnits)
{
    if (month.trimmed().isEmpty())
        return "Month is required.";

    Consumer* existingConsumer = searchConsumerbyID(consumerID);
    if (existingConsumer == NULL && userLogin(username, password) == 0 && searchConsumerbyID(consumerID) == NULL)
    {
        if (userCount >= 100)
            return "User storage full.";

        users[userCount++] = LoginManager(username.toStdString(), password.toStdString(), consumerID);
        FileManager::saveUsers(users, userCount);
    }
    else
    {
        int userIndex = findUserIndex(username);
        if (userIndex >= 0 && users[userIndex].getConsumerID() != consumerID)
            return "Username already exists for a different consumer.";
    }

    if (type < 1 || type > 3)
        return "Invalid consumer type.";

    float bill = 0;

    if (existingConsumer != NULL)
    {
        float offPeak = units - peakUnits;
        float extra = exportedUnits;
        existingConsumer->updateUnits(units, peakUnits, offPeak, extra);
        bill = existingConsumer->calculateBill();
    }
    else
    {
        std::string n = formatName(name.toStdString());
        Consumer* created = NULL;

        if (type == 1)
        {
            float offPeak = units - peakUnits;
            created = new ResidentialConsumer(consumerID, n, units, peakUnits, offPeak);
        }
        else if (type == 2)
        {
            created = new CommercialConsumer(consumerID, n, units);
        }
        else
        {
            float offPeak = units - peakUnits;
            created = new SolarConsumer(consumerID, n, units, peakUnits, offPeak, exportedUnits);
        }

        int& countRef = getCount();
        if (countRef >= 100)
        {
            delete created;
            return "Consumer storage full.";
        }

        getConsumer()[countRef] = created;
        bill = created->calculateBill();
        ++countRef;
    }

    if (FileManager::billExists(consumerID, month.toStdString()))
        return "A bill already exists for this month.";

    FileManager::saveBillRecord(MonthlyRecords(consumerID, month.toStdString(), bill));
    FileManager::saveConsumers(getConsumer(), getCount());

    return existingConsumer != NULL
        ? QString("User saved and units updated. Bill: %1 Rs").arg(money(bill))
        : QString("User registered and consumer saved. Bill: %1 Rs").arg(money(bill));
}

QString SystemContext::searchConsumerText(int id)
{
    Consumer* c = searchConsumerbyID(id);
    if (c == NULL)
        return "Consumer not found.";

    QString text = QString::fromStdString(c->Display());
    text += QString("Total Paid: %1 Rs\n").arg(money(FileManager::getPaidTotal(id)));
    text += QString("Total Unpaid: %1 Rs\n").arg(money(FileManager::getUnpaidTotal(id)));
    float credit = FileManager::getCreditTotal(id);
    if (credit > 0)
        text += QString("Credit Due: %1 Rs\n").arg(money(credit));
    return text;
}

QString SystemContext::billHistoryText(int consumerID)
{
    QFile file("bills.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return "No bill records found.";

    QString text;
    QTextStream in(&file);
    int id;
    QString month;
    float amount;
    int paid;
    bool found = false;

    while (!in.atEnd())
    {
        in >> id >> month >> amount >> paid;
        if (id == consumerID)
        {
            found = true;
            text += QString("Month: %1\n").arg(month);
            text += amount < 0 ? QString("Credit: %1 Rs\n").arg(money(-amount)) : QString("Amount: %1 Rs\n").arg(money(amount));
            text += paid ? "Status: PAID\n" : (amount < 0 ? "Status: CREDIT PENDING\n" : "Status: UNPAID\n");
            text += "----------------------\n";
        }
    }

    return found ? text : QString("No bill records found for this consumer.");
}

QString SystemContext::payBillText(int consumerID, const QString& month)
{
    FileManager::payConsumerBill(consumerID, month.toStdString());
    return QString("Payment processed for %1.").arg(month);
}
