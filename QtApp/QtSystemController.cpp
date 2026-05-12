#include "QtSystemController.h"
#include "FileManager.h"
#include "Consumer.h"
#include "MonthlyRecords.h"

#include <QFile>
#include <QTextStream>

QtSystemController::QtSystemController() : userCount(0)
{
    FileManager::loadRates();
    FileManager::loadConsumers(getConsumer(), getCount());
    FileManager::loadUsers(users, userCount);
}

QString QtSystemController::money(float value)
{
    return QString::number(value, 'f', 2);
}

QString QtSystemController::showName(const std::string& value)
{
    return QString::fromStdString(displayName(value));
}

bool QtSystemController::adminLogin(const QString& username, const QString& password) const
{
    return username == "admin" && password == "admin123";
}

int QtSystemController::userLogin(const QString& username, const QString& password) const
{
    for (int i = 0; i < userCount; ++i)
    {
        if (QString::fromStdString(users[i].getUsername()) == username && users[i].login(username.toStdString(), password.toStdString()))
        {
            return users[i].getConsumerID();
        }
    }

    return 0;
}

QString QtSystemController::addConsumerRecord(int type, int id, const QString& name, const QString& month, float units, float peakUnits, float exportedUnits)
{
    std::string n = formatName(name.toStdString());

    if (searchConsumerbyID(id) != NULL)
        return "Consumer with this ID already exists.";

    Consumer* created = NULL;

    if (type == 1)
    {
        float offPeak = units - peakUnits;
        created = new ResidentialConsumer(id, n, units, peakUnits, offPeak);
    }
    else if (type == 2)
    {
        created = new CommercialConsumer(id, n, units);
    }
    else if (type == 3)
    {
        float offPeak = units - peakUnits;
        created = new SolarConsumer(id, n, units, peakUnits, offPeak, exportedUnits);
    }
    else
    {
        return "Invalid consumer type.";
    }

    int& countRef = getCount();
    getConsumer()[countRef] = created;

    float bill = created->calculateBill();
    if (!FileManager::billExists(id, month.toStdString()))
    {
        FileManager::saveBillRecord(MonthlyRecords(id, month.toStdString(), bill));
    }

    ++countRef;
    FileManager::saveConsumers(getConsumer(), getCount());
    return bill < 0 ? QString("Credit saved: %1 Rs").arg(money(-bill)) : QString("Bill saved: %1 Rs").arg(money(bill));
}

QString QtSystemController::registerUserRecord(const QString& username, const QString& password, int consumerID)
{
    if (searchConsumerbyID(consumerID) == NULL)
        return "Consumer does not exist.";

    if (userCount >= 100)
        return "User storage full.";

    users[userCount++] = LoginManager(username.toStdString(), password.toStdString(), consumerID);
    FileManager::saveUsers(users, userCount);
    return "User registered successfully.";
}

QString QtSystemController::setRatesRecord(float peak, float offpeak, float commercial, float solar)
{
    setRates(peak, offpeak, commercial, solar);
    FileManager::saveRates();
    return "Rates updated successfully.";
}

QString QtSystemController::updateConsumerRecord(int id, const QString& month, float units, float peakUnits, float exportedUnits)
{
    Consumer* c = searchConsumerbyID(id);
    if (c == NULL)
        return "Consumer not found.";

    if (FileManager::billExists(id, month.toStdString()))
        return "Bill already exists for this month.";

    float peak = 0;
    float offPeak = 0;
    float extra = 0;

    if (dynamic_cast<SolarConsumer*>(c) != NULL)
    {
        peak = peakUnits;
        offPeak = units - peakUnits;
        extra = exportedUnits;
    }
    else if (dynamic_cast<ResidentialConsumer*>(c) != NULL)
    {
        peak = peakUnits;
        offPeak = units - peakUnits;
    }

    c->updateUnits(units, peak, offPeak, extra);
    float bill = c->calculateBill();
    FileManager::saveBillRecord(MonthlyRecords(id, month.toStdString(), bill));
    return bill < 0 ? QString("Credit saved: %1 Rs").arg(money(-bill)) : QString("Bill saved: %1 Rs").arg(money(bill));
}

QString QtSystemController::searchConsumerText(int id)
{
    Consumer* c = searchConsumerbyID(id);
    if (c == NULL)
        return "Consumer not found.";

    QString text;
    text += QString::fromStdString(c->Display());
    text += QString("Total Paid: %1 Rs\n").arg(money(FileManager::getPaidTotal(id)));
    text += QString("Total Unpaid: %1 Rs\n").arg(money(FileManager::getUnpaidTotal(id)));
    float credit = FileManager::getCreditTotal(id);
    if (credit > 0)
        text += QString("Credit Due: %1 Rs\n").arg(money(credit));
    return text;
}

QString QtSystemController::allConsumersText()
{
    if (getCount() == 0)
        return "No consumers found.";

    QString text;
    for (int i = 0; i < getCount(); ++i)
    {
        int id = getConsumer()[i]->getID();
        text += "------------------------\n";
        text += QString::fromStdString(getConsumer()[i]->Display());
        text += QString("Total Paid: %1 Rs\n").arg(money(FileManager::getPaidTotal(id)));
        text += QString("Total Unpaid: %1 Rs\n").arg(money(FileManager::getUnpaidTotal(id)));
        float credit = FileManager::getCreditTotal(id);
        if (credit > 0)
            text += QString("Credit Due: %1 Rs\n").arg(money(credit));
    }
    return text;
}

QString QtSystemController::statisticsText()
{
    if (getCount() == 0)
        return "No consumers available.";

    float totalCollected = 0;
    float totalOutstanding = 0;
    float highestUnpaid = FileManager::getUnpaidTotal(getConsumer()[0]->getID());
    float lowestUnpaid = highestUnpaid;
    int highestID = getConsumer()[0]->getID();
    int lowestID = highestID;

    for (int i = 0; i < getCount(); ++i)
    {
        int id = getConsumer()[i]->getID();
        float unpaid = FileManager::getUnpaidTotal(id);
        float paid = FileManager::getPaidTotal(id);
        totalCollected += paid;
        totalOutstanding += unpaid;
        if (unpaid > highestUnpaid) { highestUnpaid = unpaid; highestID = id; }
        if (unpaid < lowestUnpaid) { lowestUnpaid = unpaid; lowestID = id; }
    }

    QString text;
    text += "Total Consumers: " + QString::number(getCount()) + "\n";
    text += QString("Total Collected: %1 Rs\n").arg(money(totalCollected));
    text += QString("Total Outstanding: %1 Rs\n").arg(money(totalOutstanding));
    text += QString("Highest Unpaid Bill: %1 Rs (ID: %2)\n").arg(money(highestUnpaid)).arg(highestID);
    text += QString("Lowest Unpaid Bill: %1 Rs (ID: %2)\n").arg(money(lowestUnpaid)).arg(lowestID);
    return text;
}

QString QtSystemController::billHistoryText(int consumerID)
{
    QFile file("bills.txt");
    if (!file.exists())
        return "No bill records found.";

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return "Unable to open bills.txt.";

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

QString QtSystemController::payBillText(int consumerID, const QString& month)
{
    FileManager::payConsumerBill(consumerID, month.toStdString());
    return QString("Payment processed for %1.").arg(month);
}
