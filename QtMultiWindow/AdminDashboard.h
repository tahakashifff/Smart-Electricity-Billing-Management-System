#pragma once

#include <QDialog>

class SystemContext;
class QTextEdit;
class QLineEdit;
class QComboBox;

class AdminDashboard : public QDialog
{
    Q_OBJECT

public:
    explicit AdminDashboard(SystemContext* context, QWidget* parent = nullptr);

private slots:
    void onAddConsumer();
    void onUpdateConsumer();
    void onDisplayAll();
    void onSearchConsumer();
    void onSetRates();
    void onRegisterUser();
    void onSearchUser();
    void onShowStatistics();
    void onBackToLogin();

private:
    SystemContext* context;
    QTextEdit* output;

    QLineEdit* addId;
    QLineEdit* addName;
    QLineEdit* addMonth;
    QComboBox* addType;
    QLineEdit* addUnits;
    QLineEdit* addPeakUnits;
    QLineEdit* addExportedUnits;

    QLineEdit* updateId;
    QLineEdit* updateMonth;
    QLineEdit* updateUnits;
    QLineEdit* updatePeakUnits;
    QLineEdit* updateExportedUnits;

    QLineEdit* searchConsumerId;

    QLineEdit* peakRate;
    QLineEdit* offPeakRate;
    QLineEdit* commercialRate;
    QLineEdit* solarRate;

    QLineEdit* regUser;
    QLineEdit* regPass;
    QLineEdit* regConsumerId;

    QLineEdit* searchUserName;

    void buildUi();
    void appendOutput(const QString& text);
};
