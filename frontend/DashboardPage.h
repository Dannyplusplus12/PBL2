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
    void onOverdueClicked();

private:
    Ui::DashboardPage* ui;
    bool m_overdueVisible = false;

    void refreshCards();
    void refreshOverdueTable();
};
