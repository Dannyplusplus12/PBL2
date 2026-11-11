#include "BorrowCartDialog.h"
#include "ui_BorrowCartDialog.h"

#include <QMessageBox>
#include <QSpinBox>
#include <QPushButton>

BorrowCartDialog::BorrowCartDialog(int customerId, QWidget* parent)
    : QDialog(parent), ui(new Ui::BorrowCartDialog), m_customerId(customerId) {
    ui->setupUi(this);

    Customer* c = g_lib.findCustomer(customerId);
    setWindowTitle(c ? QString("Mượn sách — %1 (có thể mượn thêm %2 cuốn)")
                              .arg(qs(c->getName()))
                              .arg(remainingBorrowCopies())
                     : QString("Mượn sách"));

    ui->cartTable->setColumnCount(3);
    ui->cartTable->setHorizontalHeaderLabels(
        QStringList() << "Sách" << "Số lượng" << "Thao tác");
    ui->cartTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->cartTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    ui->cartTable->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    ui->cartTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->cartTable->setSelectionMode(QAbstractItemView::NoSelection);
    ui->cartTable->verticalHeader()->setVisible(false);

    ui->daysSpin->setRange(1, Library::maxBorrowDays);
    ui->daysSpin->setValue(5);

    if (QPushButton* okBtn = ui->buttonBox->button(QDialogButtonBox::Ok))
        okBtn->setText("Mượn");

    connect(ui->searchEdit, &QLineEdit::textChanged,
            this, &BorrowCartDialog::onSearchChanged);
    connect(ui->bookList, &QListWidget::currentItemChanged,
            this, &BorrowCartDialog::onBookSelected);
    connect(ui->addBtn, &QPushButton::clicked,
            this, &BorrowCartDialog::onAddToCart);
    connect(ui->qtySpin, QOverload<int>::of(&QSpinBox::valueChanged),
            this, [this](int value) {
        if (m_selectedBookId < 0)
            return;
        const int maxQty = qMin(g_lib.available(m_selectedBookId),
                                remainingForSelectedBook());
        if (value > maxQty) {
            QMessageBox::warning(this, "Giới hạn mượn",
                QString("Có thể thêm tối đa %1 cuốn cho lần nhập này.").arg(maxQty));
            ui->qtySpin->setValue(qMax(1, maxQty));
        }
    });

    rebuildBookList();
}

BorrowCartDialog::~BorrowCartDialog() {
    delete ui;
}

void BorrowCartDialog::rebuildBookList() {
    ui->bookList->clear();
    const QString filter = ui->searchEdit->text().trimmed();

    for (int i = 0; i < (int)g_lib.books.size(); ++i) {
        const Book& b = g_lib.books[i];
        const int avail = g_lib.available(b.getId());
        if (avail <= 0)
            continue;
        const QString name = qs(b.getName());
        if (!filter.isEmpty() && !name.startsWith(filter, Qt::CaseInsensitive))
            continue;

        QListWidgetItem* item = new QListWidgetItem(
            QString("%1  (còn %2 bản)").arg(name).arg(avail));
        item->setData(Qt::UserRole, b.getId());
        ui->bookList->addItem(item);
    }

    m_selectedBookId = -1;
    ui->qtySpin->setEnabled(false);
    ui->addBtn->setEnabled(false);
}

void BorrowCartDialog::onSearchChanged(const QString&) {
    rebuildBookList();
}

void BorrowCartDialog::onBookSelected() {
    QListWidgetItem* item = ui->bookList->currentItem();
    if (!item) {
        m_selectedBookId = -1;
        ui->qtySpin->setEnabled(false);
        ui->addBtn->setEnabled(false);
        return;
    }
    m_selectedBookId = item->data(Qt::UserRole).toInt();
    const int avail = g_lib.available(m_selectedBookId);
    const int maxQty = qMin(avail, remainingForSelectedBook());
    ui->qtySpin->setRange(1, qMax(1, maxQty));
    ui->qtySpin->setValue(1);
    ui->qtySpin->setEnabled(maxQty > 0);
    ui->addBtn->setEnabled(maxQty > 0);
}

void BorrowCartDialog::onAddToCart() {
    addToCart(m_selectedBookId, ui->qtySpin->value());
}

void BorrowCartDialog::addToCart(int bookId, int qty) {
    if (bookId < 0 || qty <= 0)
        return;
    const int avail = g_lib.available(bookId);
    if (avail <= 0)
        return;

    int existingQty = 0;
    for (const CartLine& line : m_cart)
        if (line.bookId == bookId)
            existingQty = line.qty;

    const int otherTotal = cartTotalCopies() - existingQty;
    const int allowed = qMin(avail - existingQty,
                             remainingBorrowCopies() - otherTotal);
    if (qty > allowed) {
        QMessageBox::warning(this, "Giới hạn mượn",
            QString("Khách chỉ có thể mượn thêm %1 cuốn trong giỏ hàng.").arg(qMax(0, allowed)));
        qty = qMax(0, allowed);
    }
    if (qty <= 0)
        return;

    for (int i = 0; i < m_cart.size(); ++i) {
        if (m_cart[i].bookId == bookId) {
            m_cart[i].qty = qMin(m_cart[i].qty + qty, avail);
            refreshCart();
            return;
        }
    }

    CartLine line;
    line.bookId = bookId;
    line.qty = qty;
    m_cart.push_back(line);
    refreshCart();
}

void BorrowCartDialog::refreshCart() {
    ui->cartTable->setRowCount(0);
    for (int i = 0; i < m_cart.size(); ++i) {
        const int bookId = m_cart[i].bookId;
        const int qty = m_cart[i].qty;
        Book* b = g_lib.findBook(bookId);

        const int row = ui->cartTable->rowCount();
        ui->cartTable->insertRow(row);
        ui->cartTable->setItem(row, 0, new QTableWidgetItem(b ? qs(b->getName()) : "?"));

        QSpinBox* spin = new QSpinBox(ui->cartTable);
        const int otherTotal = cartTotalCopies() - qty;
        const int maxQty = qMin(g_lib.available(bookId),
                                qMax(0, remainingBorrowCopies() - otherTotal));
        spin->setRange(0, maxQty);
        spin->setValue(qty);
        const int idx = i;
        connect(spin, QOverload<int>::of(&QSpinBox::valueChanged),
                this, [this, idx](int v) {
            if (idx >= m_cart.size())
                return;
            const int otherTotal = cartTotalCopies() - m_cart[idx].qty;
            const int allowed = qMin(g_lib.available(m_cart[idx].bookId),
                                     qMax(0, remainingBorrowCopies() - otherTotal));
            if (v > allowed) {
                QMessageBox::warning(this, "Giới hạn mượn",
                    QString("Tổng số sách trong giỏ không được vượt quá %1 cuốn.")
                        .arg(remainingBorrowCopies()));
                v = allowed;
            }
            if (v <= 0)
                m_cart.removeAt(idx);
            else
                m_cart[idx].qty = v;
            refreshCart();
        });
        ui->cartTable->setCellWidget(row, 1, spin);

        QPushButton* removeBtn = new QPushButton("Xóa", ui->cartTable);
        connect(removeBtn, &QPushButton::clicked, this, [this, idx]() {
            if (idx >= m_cart.size())
                return;
            m_cart.removeAt(idx);
            refreshCart();
        });
        ui->cartTable->setCellWidget(row, 2, removeBtn);
    }
}

int BorrowCartDialog::cartTotalCopies() const {
    int n = 0;
    for (const CartLine& l : m_cart)
        n += l.qty;
    return n;
}

int BorrowCartDialog::currentActiveCopies() const {
    int n = 0;
    for (int i = 0; i < (int)g_lib.borrows.size(); ++i)
        if (g_lib.borrows[i].getCustomerId() == m_customerId
            && !g_lib.borrows[i].isReturned())
            ++n;
    return n;
}

int BorrowCartDialog::remainingBorrowCopies() const {
    return qMax(0, Library::maxBorrowCopies - currentActiveCopies());
}

int BorrowCartDialog::remainingForSelectedBook() const {
    int existingQty = 0;
    for (const CartLine& line : m_cart)
        if (line.bookId == m_selectedBookId)
            existingQty = line.qty;
    return qMax(0, remainingBorrowCopies() - cartTotalCopies() + existingQty);
}

void BorrowCartDialog::accept() {
    Customer* c = g_lib.findCustomer(m_customerId);
    if (!c) {
        QDialog::reject();
        return;
    }
    if (c->isLocked()) {
        QMessageBox::warning(this, "Lỗi", "Khách hàng đang bị khóa, không thể mượn sách.");
        return;
    }
    if (m_cart.isEmpty()) {
        QMessageBox::information(this, "Thông báo", "Giỏ hàng đang trống.");
        return;
    }

    const int days = ui->daysSpin->value();
    const int total = cartTotalCopies();
    const int maxAllowed = remainingBorrowCopies();
    if (total > maxAllowed) {
        QMessageBox::warning(this, "Lỗi",
            QString("Khách chỉ được mượn tối đa %1 cuốn cùng lúc (có thể mượn thêm %2 cuốn).")
                .arg(Library::maxBorrowCopies)
                .arg(maxAllowed));
        return;
    }

    int ok = 0;
    for (const CartLine& l : m_cart) {
        for (int k = 0; k < l.qty; ++k)
            if (g_lib.borrow(m_customerId, l.bookId, days))
                ++ok;
    }
    QMessageBox::information(this, "Thành công", QString("Đã mượn %1 cuốn sách.").arg(ok));
    QDialog::accept();
}
