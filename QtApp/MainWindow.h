#pragma once

#include <QMainWindow>

class QLineEdit;
class QTextEdit;
class QComboBox;
class QtSystemController;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);

private slots:
    void onAdminLogin();
    void onUserLogin();
    void onAddConsumer();
    void onRegisterUser();
    void onSetRates();
    void onSearchConsumer();
    void onAllConsumers();
    void onStatistics();
    void onViewHistory();
    void onPayBill();

private:
    QtSystemController* controller;
    QTextEdit* output;

    QLineEdit* adminUser;
    QLineEdit* adminPass;
    QLineEdit* userName;
    QLineEdit* userPass;

    QComboBox* consumerType;
    QLineEdit* consumerId;
    QLineEdit* consumerName;
    QLineEdit* consumerMonth;
    QLineEdit* consumerUnits;
    QLineEdit* consumerPeakUnits;
    QLineEdit* consumerExported;

    QLineEdit* regUser;
    QLineEdit* regPass;
    QLineEdit* regConsumerId;

    QLineEdit* ratePeak;
    QLineEdit* rateOffPeak;
    QLineEdit* rateCommercial;
    QLineEdit* rateSolar;

    QLineEdit* searchId;
    QLineEdit* historyId;
    QLineEdit* payId;
    QLineEdit* payMonth;

    void buildUi();
    void appendOutput(const QString& text);
};
