#pragma once

#include <QDialog>

class SystemContext;
class QTextEdit;
class QLineEdit;

class UserDashboard : public QDialog
{
    Q_OBJECT

public:
    explicit UserDashboard(SystemContext* context, int consumerID = 0, QWidget* parent = nullptr);

private slots:
    void onViewCurrentBill();
    void onViewHistory();
    void onPayBill();
    void onBackToLogin();

private:
    SystemContext* context;
    int consumerID;
    QTextEdit* output;
    QLineEdit* linkedConsumerId;
    QLineEdit* month;

    void buildUi();
    void appendOutput(const QString& text);
};
