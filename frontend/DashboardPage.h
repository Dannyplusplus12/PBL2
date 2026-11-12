#pragma once
#include <QWidget>
#include "globals.h"

namespace Ui { class DashboardPage; }

class DashboardPage : public QWidget {
    Q_OBJECT
public:
    explicit DashboardPage(QWidget* parent = nullptr);
    ~DashboardPage() override;

    void refresh();

protected:
    bool eventFilter(QObject* watched, QEvent* event) override;

private slots:
    void onBorrowingClicked();
    void onOverdueClicked();

private:
    Ui::DashboardPage* ui;
    bool m_detailsVisible = false;
    bool m_showingOverdue = false;

    void refreshCards();
    void refreshBorrowTable();
};
