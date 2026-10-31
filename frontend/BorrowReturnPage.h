#pragma once
#include <QWidget>
#include "globals.h"

namespace Ui { class BorrowReturnPage; }

class BorrowReturnPage : public QWidget {
    Q_OBJECT
public:
    explicit BorrowReturnPage(QWidget* parent = nullptr);
    ~BorrowReturnPage() override;

    void refresh();

private slots:
    void onSearchTextChanged(const QString& text);
    void onCustomerSelected();
    void onAddCustomerClicked();
    void onSaveCustomerClicked();
    void onCancelFormClicked();
    void onEditCustomerClicked();
    void onToggleLockClicked();
    void onDeleteCustomerClicked();
    void onBorrowClicked();

private:
    Ui::BorrowReturnPage* ui;
    int m_editingCustomerId = -1;   // -1 = đang thêm mới
    int m_selectedCustomerId = -1;

    void rebuildCustomerList();
    void showPlaceholder();
    void showDetail(int customerId);
    void fillCustomerTable(int customerId);
    bool customerHasOverdue(int customerId) const;
};
