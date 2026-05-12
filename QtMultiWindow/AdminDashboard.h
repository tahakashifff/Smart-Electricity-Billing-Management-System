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
    void onSearchUser();
    void onRegisterUser();

private:
    SystemContext* context;
    QTextEdit* output;

    QLineEdit* searchUserName;
    QLineEdit* regUser;
    QLineEdit* regPass;
    QLineEdit* regConsumerId;

    QComboBox* consumerType;
    QLineEdit* consumerName;
    QLineEdit* consumerMonth;
    QLineEdit* consumerUnits;
    QLineEdit* consumerPeakUnits;
    QLineEdit* consumerExported;

    void buildUi();
    void appendOutput(const QString& text);
};
