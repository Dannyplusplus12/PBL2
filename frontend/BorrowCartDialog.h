#pragma once
#include <QDialog>
#include <QVector>
#include "globals.h"

namespace Ui { class BorrowCartDialog; }

// Popup mượn sách: trái = danh sách sách còn bản, phải = giỏ hàng.
class BorrowCartDialog : public QDialog {
    Q_OBJECT
public:
    explicit BorrowCartDialog(int customerId, QWidget* parent = nullptr);
    ~BorrowCartDialog() override;

private slots:
    void onSearchChanged(const QString& text);
    void onBookSelected();
    void onAddToCart();
    void accept() override;

private:
    struct CartLine { int bookId; int qty; };

    Ui::BorrowCartDialog* ui;
    int m_customerId;
    int m_selectedBookId = -1;
    QVector<CartLine> m_cart;

    void rebuildBookList();
    void addToCart(int bookId, int qty);
    void refreshCart();
    int cartTotalCopies() const;
    int currentActiveCopies() const;
    int remainingBorrowCopies() const;
    int remainingForSelectedBook() const;
};
