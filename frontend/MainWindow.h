#pragma once
#include <QMainWindow>
#include "globals.h"

class DashboardPage;
class BorrowReturnPage;
class ManagementPage;

namespace Ui { class MainWindow; }

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override;

private slots:
    void onNavChanged(int row);

private:
    Ui::MainWindow* ui;
    DashboardPage*    m_dashboard = nullptr;
    BorrowReturnPage* m_borrow    = nullptr;
    ManagementPage*   m_manage    = nullptr;
};
