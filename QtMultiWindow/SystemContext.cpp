#include "SystemContext.h"

#include "Consumer.h"
#include "FileManager.h"
#include "MonthlyRecords.h"

#include <QFile>
#include <QTextStream>

#include <filesystem>
#include <fstream>
#include <sstream>
#include <vector>

namespace fs = std::filesystem;

namespace
{
    fs::path locateDataRoot()
    {
        fs::path dir = fs::current_path();

        while (true)
        {
            if (fs::exists(dir / "users.txt") || fs::exists(dir / "Consumers.txt") || fs::exists(dir / "rates.txt") || fs::exists(dir / "bills.txt"))
                return dir;

            if (fs::exists(dir / "Project"))
                return dir / "Project";

            if (!dir.has_parent_path() || dir.parent_path() == dir)
                break;

            dir = dir.parent_path();
        }

        return fs::current_path();
    }

    fs::path dataReadPath(const std::string& fileName)
    {
        fs::path root = locateDataRoot();

        if (fs::exists(root / fileName))
            return root / fileName;

        if (fs::exists(root / "Project" / fileName))
            return root / "Project" / fileName;

        return root / fileName;
    }

    fs::path dataWritePath(const std::string& fileName)
    {
        fs::path root = locateDataRoot();

        if (fs::exists(root / fileName))
            return root / fileName;

        if (fs::exists(root / "Project"))
            return root / "Project" / fileName;

        return root / fileName;
    }

    struct BillRow
    {
        int consumerID;
        std::string month;
        float amount;
        bool paid;
    };
}

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

bool SystemContext::monthExistsForConsumer(int consumerID, const QString& month) const
{
    return FileManager::billExists(consumerID, month.toStdString());
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

QString SystemContext::addConsumerText(int id, const QString& name, const QString& month, int type, float units, float peakUnits, float exportedUnits)
{
    if (getCount() >= 100)
        return "Storage full.";

    if (id <= 0)
        return "Invalid consumer ID.";

    if (searchConsumerbyID(id) != NULL)
        return QString("Consumer with ID %1 already exists!").arg(id);

    if (name.trimmed().isEmpty())
        return "Name cannot be empty.";

    if (month.trimmed().isEmpty())
        return "Month cannot be empty.";

    if (monthExistsForConsumer(id, month))
        return QString("A bill for %1 already exists for this consumer!").arg(month);

    if (type < 1 || type > 3)
        return "Invalid consumer type.";

    if (units < 0)
        return "Units cannot be negative.";

    if ((type == 1 || type == 3) && (peakUnits < 0 || peakUnits > units))
        return "Peak units cannot exceed total units consumed.";

    if (type == 3 && exportedUnits < 0)
        return "Exported units cannot be negative.";

    std::string formattedName = formatName(name.trimmed().toStdString());
    Consumer* created = NULL;

    if (type == 1)
    {
        float offPeak = units - peakUnits;
        created = new ResidentialConsumer(id, formattedName, units, peakUnits, offPeak);
    }
    else if (type == 2)
    {
        created = new CommercialConsumer(id, formattedName, units);
    }
    else
    {
        float offPeak = units - peakUnits;
        created = new SolarConsumer(id, formattedName, units, peakUnits, offPeak, exportedUnits);
    }

    getConsumer()[getCount()] = created;
    ++getCount();

    float bill = created->calculateBill();

    FileManager::saveBillRecord(MonthlyRecords(id, month.toStdString(), bill));
    FileManager::saveConsumers(getConsumer(), getCount());

    if (bill < 0)
        return QString("Consumer added successfully! Credit for %1: %2 Rs (company owes consumer)").arg(month).arg(money(-bill));

    return QString("Consumer added successfully! Bill for %1: %2 Rs ").arg(month).arg(money(bill));
}

QString SystemContext::updateConsumerText(int id, const QString& month, float units, float peakUnits, float exportedUnits)
{
    Consumer* c = searchConsumerbyID(id);

    if (c == NULL)
        return "Consumer not found!";

    if (month.trimmed().isEmpty())
        return "Month cannot be empty.";

    if (monthExistsForConsumer(id, month))
        return QString("A bill for %1 already exists for this consumer!").arg(month);

    if (units < 0)
        return "Units cannot be negative.";

    float peak = 0;
    float offPeak = 0;
    float extra = 0;

    SolarConsumer* sc = dynamic_cast<SolarConsumer*>(c);
    ResidentialConsumer* rc = dynamic_cast<ResidentialConsumer*>(c);

    if (sc != NULL || rc != NULL)
    {
        if (peakUnits < 0 || peakUnits > units)
            return "Peak units cannot exceed total units consumed.";

        peak = peakUnits;
        offPeak = units - peak;
    }

    if (sc != NULL)
    {
        if (exportedUnits < 0)
            return "Exported units cannot be negative.";

        extra = exportedUnits;
    }

    c->updateUnits(units, peak, offPeak, extra);

    float newBill = c->calculateBill();

    FileManager::saveBillRecord(MonthlyRecords(c->getID(), month.toStdString(), newBill));
    FileManager::saveConsumers(getConsumer(), getCount());

    if (newBill < 0)
        return QString("Consumer updated successfully! Credit for %1: %2 Rs (company owes consumer)").arg(month).arg(money(-newBill));

    return QString("Consumer updated successfully! Bill for %1: %2 Rs ").arg(month).arg(money(newBill));
}

QString SystemContext::displayAllConsumersText()
{
    if (getCount() == 0)
        return "No consumers found!";

    QString text;

    for (int i = 0; i < getCount(); ++i)
    {
        int id = getConsumer()[i]->getID();
        float unpaid = FileManager::getUnpaidTotal(id);
        float paid = FileManager::getPaidTotal(id);
        float credit = FileManager::getCreditTotal(id);

        text += "------------------------\n";
        text += QString::fromStdString(getConsumer()[i]->Display());
        text += QString("Total Paid: %1 Rs\n").arg(money(paid));
        text += QString("Total Unpaid: %1 Rs\n").arg(money(unpaid));
        if (credit > 0)
            text += QString("Credit Due: %1 Rs\n").arg(money(credit));
    }

    return text;
}

QString SystemContext::searchConsumerText(int id)
{
    Consumer* c = searchConsumerbyID(id);
    if (c == NULL)
        return "Consumer not found!";

    QString text = QString::fromStdString(c->Display());
    text += QString("Total Paid: %1 Rs\n").arg(money(FileManager::getPaidTotal(id)));
    text += QString("Total Unpaid: %1 Rs\n").arg(money(FileManager::getUnpaidTotal(id)));

    float credit = FileManager::getCreditTotal(id);
    if (credit > 0)
        text += QString("Credit Due: %1 Rs\n").arg(money(credit));

    return text;
}

QString SystemContext::setRatesText(float peakRate, float offPeakRate, float commercialRate, float solarRateValue)
{
    if (peakRate < 0 || offPeakRate < 0 || commercialRate < 0 || solarRateValue < 0)
        return "Rates cannot be negative.";

    setRates(peakRate, offPeakRate, commercialRate, solarRateValue);
    FileManager::saveRates();

    return "Rates updated successfully!";
}

QString SystemContext::registerUserText(const QString& username, const QString& password, int consumerID)
{
    if (username.trimmed().isEmpty())
        return "Username cannot be empty.";

    if (password.trimmed().isEmpty())
        return "Password cannot be empty.";

    if (searchConsumerbyID(consumerID) == NULL)
        return "Consumer does not exist!";

    if (findUserIndex(username.trimmed()) >= 0)
        return "Username already exists.";

    if (userCount >= 100)
        return "User storage full.";

    users[userCount] = LoginManager(username.trimmed().toStdString(), password.toStdString(), consumerID);
    ++userCount;

    FileManager::saveUsers(users, userCount);

    return "User registered successfully!";
}

QString SystemContext::searchUserText(const QString& username) const
{
    int index = findUserIndex(username.trimmed());
    if (index >= 0)
        return QString("Username: %1\nLinked Consumer ID: %2").arg(username.trimmed()).arg(users[index].getConsumerID());

    return "User not found.";
}

QString SystemContext::statisticsText()
{
    if (getCount() == 0)
        return "No consumers available!";

    float totalCollected = 0;
    float totalOutstanding = 0;

    int highestID = getConsumer()[0]->getID();
    int lowestID = getConsumer()[0]->getID();

    float highestUnpaid = FileManager::getUnpaidTotal(highestID);
    float lowestUnpaid = highestUnpaid;

    for (int i = 0; i < getCount(); ++i)
    {
        int id = getConsumer()[i]->getID();
        float unpaid = FileManager::getUnpaidTotal(id);
        float paid = FileManager::getPaidTotal(id);

        totalCollected += paid;
        totalOutstanding += unpaid;

        if (unpaid > highestUnpaid)
        {
            highestUnpaid = unpaid;
            highestID = id;
        }

        if (unpaid < lowestUnpaid)
        {
            lowestUnpaid = unpaid;
            lowestID = id;
        }
    }

    QString text;
    text += "===== SYSTEM STATISTICS =====\n";
    text += QString("Total Collected: %1 Rs\n").arg(money(totalCollected));
    text += QString("Total Outstanding: %1 Rs\n").arg(money(totalOutstanding));
    text += QString("Highest Unpaid Bill: %1 Rs (ID: %2)\n").arg(money(highestUnpaid)).arg(highestID);
    text += QString("Lowest Unpaid Bill: %1 Rs (ID: %2)").arg(money(lowestUnpaid)).arg(lowestID);

    return text;
}

QString SystemContext::viewCurrentBillText(int consumerID)
{
    Consumer* c = searchConsumerbyID(consumerID);
    if (c == NULL)
        return "Consumer not found!";

    QString text = QString::fromStdString(c->Display());
    text += QString("Type: %1\n")
        .arg(dynamic_cast<SolarConsumer*>(c) != NULL
                 ? "Solar"
                 : (dynamic_cast<ResidentialConsumer*>(c) != NULL ? "Residential" : "Commercial"));
    text += QString("Bill: %1 Rs").arg(money(c->calculateBill()));

    return text;
}

QString SystemContext::billHistoryText(int consumerID)
{
    std::ifstream file(dataReadPath("bills.txt"));
    if (!file)
        return "No bill records found!";

    int id;
    std::string month;
    float amount;
    bool paid;

    bool found = false;
    float totalUnpaid = 0;

    QString text;

    while (file >> id >> month >> amount >> paid)
    {
        if (id != consumerID)
            continue;

        found = true;
        text += QString("Month: %1\n").arg(QString::fromStdString(month));

        if (amount < 0)
            text += QString("Amount: CREDIT %1 Rs\n").arg(money(-amount));
        else
            text += QString("Amount: %1 Rs\n").arg(money(amount));

        if (paid)
            text += "Status: PAID\n";
        else if (amount < 0)
            text += "Status: CREDIT PENDING\n";
        else
        {
            text += "Status: UNPAID\n";
            totalUnpaid += amount;
        }

        text += "----------------------\n";
    }

    if (!found)
        return "No bill records found for this consumer!";

    if (totalUnpaid > 0)
        text += QString("Remaining Due: %1 Rs").arg(money(totalUnpaid));
    else
        text += "All bills paid!";

    return text;
}

QString SystemContext::payBillText(int consumerID, const QString& month)
{
    QString trimmedMonth = month.trimmed();
    if (trimmedMonth.isEmpty())
        return "Month is required.";

    std::ifstream file(dataReadPath("bills.txt"));
    if (!file)
        return "No bill records found!";

    std::vector<BillRow> rows;

    int id;
    std::string m;
    float amount;
    bool paid;

    bool found = false;
    bool alreadyPaid = false;
    bool creditEntry = false;

    while (file >> id >> m >> amount >> paid)
    {
        BillRow row{ id, m, amount, paid };

        if (id == consumerID && m == trimmedMonth.toStdString())
        {
            found = true;

            if (amount < 0)
                creditEntry = true;
            else if (paid)
                alreadyPaid = true;
            else
                row.paid = true;
        }

        rows.push_back(row);
    }

    file.close();

    if (!found)
        return QString("No bill record found for month: %1").arg(trimmedMonth);

    if (alreadyPaid)
        return QString("Bill for %1 is already paid!").arg(trimmedMonth);

    if (creditEntry)
        return QString("Bill for %1 is a credit entry and cannot be paid.").arg(trimmedMonth);

    std::ofstream out(dataWritePath("bills.txt"), std::ios::out | std::ios::trunc);
    for (size_t i = 0; i < rows.size(); ++i)
    {
        out << rows[i].consumerID << " "
            << rows[i].month << " "
            << rows[i].amount << " "
            << (rows[i].paid ? 1 : 0) << "\n";
    }
    out.close();

    return QString("Bill for %1 paid successfully!").arg(trimmedMonth);
}
