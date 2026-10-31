#include "BorrowReturnPage.h"
#include "ui_BorrowReturnPage.h"
#include "BorrowCartDialog.h"

#include <QMessageBox>
#include <QPushButton>
#include <QColor>
#include <QHBoxLayout>

BorrowReturnPage::BorrowReturnPage(QWidget* parent)
    : QWidget(parent), ui(new Ui::BorrowReturnPage) {
    ui->setupUi(this);

    ui->pageTitle->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    ui->splitter->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->splitter->setChildrenCollapsible(false);
    ui->borrowTable->setMinimumWidth(0);
    ui->borrowTable->setSizeAdjustPolicy(QAbstractScrollArea::AdjustIgnored);

    ui->splitter->setStretchFactor(0, 1);
    ui->splitter->setStretchFactor(1, 2);
    ui->splitter->setSizes(QList<int>() << 340 << 680);

    ui->borrowTable->setColumnCount(6);
    ui->borrowTable->setHorizontalHeaderLabels(
        QStringList() << "Sách" << "Ngày mượn" << "Hạn trả"
                      << "Ngày trả" << "Trạng thái" << "Thao tác");
    ui->borrowTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->borrowTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->borrowTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->borrowTable->verticalHeader()->setVisible(false);

    connect(ui->searchEdit, &QLineEdit::textChanged,
            this, &BorrowReturnPage::onSearchTextChanged);
    connect(ui->customerList, &QListWidget::currentItemChanged,
            this, &BorrowReturnPage::onCustomerSelected);
    connect(ui->addCustomerBtn, &QPushButton::clicked,
            this, &BorrowReturnPage::onAddCustomerClicked);
    connect(ui->saveBtn, &QPushButton::clicked,
            this, &BorrowReturnPage::onSaveCustomerClicked);
    connect(ui->cancelBtn, &QPushButton::clicked,
            this, &BorrowReturnPage::onCancelFormClicked);
    connect(ui->editBtn, &QPushButton::clicked,
            this, &BorrowReturnPage::onEditCustomerClicked);
    connect(ui->toggleLockBtn, &QPushButton::clicked,
            this, &BorrowReturnPage::onToggleLockClicked);
    connect(ui->deleteCustomerBtn, &QPushButton::clicked,
            this, &BorrowReturnPage::onDeleteCustomerClicked);
    connect(ui->borrowBtn, &QPushButton::clicked,
            this, &BorrowReturnPage::onBorrowClicked);

    showPlaceholder();
}

BorrowReturnPage::~BorrowReturnPage() {
    delete ui;
}

void BorrowReturnPage::refresh() {
    rebuildCustomerList();
    if (m_selectedCustomerId == -1)
        showPlaceholder();
    else
        showDetail(m_selectedCustomerId);
}

void BorrowReturnPage::onSearchTextChanged(const QString&) {
    rebuildCustomerList();
}

void BorrowReturnPage::rebuildCustomerList() {
    const QString filter = ui->searchEdit->text().trimmed();
    const int prevSelected = m_selectedCustomerId;

    ui->customerList->clear();
    for (int i = 0; i < (int)g_lib.customers.size(); ++i) {
        const Customer& c = g_lib.customers[i];
        const QString name = qs(c.getName());
        const QString phone = qs(c.getPhone());

        if (!filter.isEmpty()
            && !name.startsWith(filter, Qt::CaseInsensitive)
            && !phone.startsWith(filter))
            continue;

        QString text = name + "\n" + phone;
        if (c.isLocked())
            text += "  (khóa)";

        QListWidgetItem* item = new QListWidgetItem(text);
        item->setData(Qt::UserRole, c.getId());
        if (customerHasOverdue(c.getId()))
            item->setBackground(QColor("#fde2e2"));
        ui->customerList->addItem(item);

        if (c.getId() == prevSelected)
            ui->customerList->setCurrentItem(item);
    }
}

bool BorrowReturnPage::customerHasOverdue(int customerId) const {
    for (int i = 0; i < (int)g_lib.borrows.size(); ++i) {
        const Borrow& bw = g_lib.borrows[i];
        if (bw.getCustomerId() == customerId && !bw.isReturned()
            && isOverdue(bw.getDueDate()))
            return true;
    }
    return false;
}

void BorrowReturnPage::showPlaceholder() {
    m_selectedCustomerId = -1;
    ui->rightStack->setCurrentWidget(ui->placeholderPage);
}

void BorrowReturnPage::onCustomerSelected() {
    QListWidgetItem* item = ui->customerList->currentItem();
    if (!item)
        return;
    m_selectedCustomerId = item->data(Qt::UserRole).toInt();
    showDetail(m_selectedCustomerId);
}

void BorrowReturnPage::showDetail(int customerId) {
    Customer* c = g_lib.findCustomer(customerId);
    if (!c)
        return;
    m_selectedCustomerId = customerId;

    ui->detailName->setText(qs(c->getName()));
    ui->detailInfo->setText("SĐT: " + qs(c->getPhone())
                            + "\nĐịa chỉ: " + qs(c->getAddress()));

    const bool locked = c->isLocked();
    ui->detailLock->setText(locked ? "Trạng thái: ĐÃ KHÓA"
                                   : "Trạng thái: Hoạt động");
    ui->detailLock->setStyleSheet(locked
        ? "color:#dc2626;font-weight:700;" : "color:#16a34a;font-weight:700;");
    ui->toggleLockBtn->setText(locked ? "Mở khóa" : "Khóa");

    fillCustomerTable(customerId);
    ui->rightStack->setCurrentWidget(ui->detailPage);
}

void BorrowReturnPage::fillCustomerTable(int customerId) {
    ui->borrowTable->setRowCount(0);

    for (int i = 0; i < (int)g_lib.borrows.size(); ++i) {
        const Borrow& bw = g_lib.borrows[i];
        if (bw.getCustomerId() != customerId)
            continue;
        Book* b = g_lib.findBook(bw.getBookId());

        const int row = ui->borrowTable->rowCount();
        ui->borrowTable->insertRow(row);
        ui->borrowTable->setItem(row, 0, new QTableWidgetItem(b ? qs(b->getName()) : "?"));
        ui->borrowTable->setItem(row, 1, new QTableWidgetItem(qs(bw.getBorrowDate())));
        ui->borrowTable->setItem(row, 2, new QTableWidgetItem(qs(bw.getDueDate())));

        if (bw.isReturned()) {
            ui->borrowTable->setItem(row, 3, new QTableWidgetItem(qs(bw.getReturnDate())));
            ui->borrowTable->setItem(row, 4, new QTableWidgetItem("Đã trả"));
        } else {
            ui->borrowTable->setItem(row, 3, new QTableWidgetItem(""));
            QString status = "Đang mượn";
            if (isOverdue(bw.getDueDate()))
                status = "QUÁ HẠN " + QString::number(overdueDays(bw.getDueDate())) + " ngày";
            if (bw.getRenewCount() > 0)
                status += " (GH " + QString::number(bw.getRenewCount()) + ")";

            QTableWidgetItem* st = new QTableWidgetItem(status);
            if (isOverdue(bw.getDueDate()))
                st->setForeground(QColor("#dc2626"));
            ui->borrowTable->setItem(row, 4, st);

            const int bid = bw.getId();

            QWidget* actions = new QWidget(ui->borrowTable);
            QHBoxLayout* lay = new QHBoxLayout(actions);
            lay->setContentsMargins(4, 2, 4, 2);
            lay->setSpacing(6);

            QPushButton* returnBtn = new QPushButton("Trả", actions);
            QPushButton* renewBtn = new QPushButton("Gia hạn", actions);
            renewBtn->setObjectName("renewBtn");

            connect(returnBtn, &QPushButton::clicked, this, [this, bid]() {
                if (g_lib.returnBook(bid, today()))
                    refresh();
                else
                    QMessageBox::warning(this, "Lỗi", "Không thể trả sách.");
            });
            connect(renewBtn, &QPushButton::clicked, this, [this, bid]() {
                if (g_lib.Renew(bid, 5))
                    refresh();
                else
                    QMessageBox::warning(this, "Lỗi",
                        "Không thể gia hạn (đã gia hạn 1 lần, quá hạn trên 5 ngày hoặc khách đang bị khóa).");
            });

            lay->addWidget(returnBtn);
            lay->addWidget(renewBtn);
            lay->addStretch();
            ui->borrowTable->setCellWidget(row, 5, actions);
        }
    }
}

void BorrowReturnPage::onAddCustomerClicked() {
    m_editingCustomerId = -1;
    ui->formTitle->setText("Thêm khách hàng");
    ui->nameEdit->clear();
    ui->phoneEdit->clear();
    ui->addressEdit->clear();
    ui->rightStack->setCurrentWidget(ui->formPage);
    ui->nameEdit->setFocus();
}

void BorrowReturnPage::onEditCustomerClicked() {
    Customer* c = g_lib.findCustomer(m_selectedCustomerId);
    if (!c)
        return;
    m_editingCustomerId = c->getId();
    ui->formTitle->setText("Sửa khách hàng");
    ui->nameEdit->setText(qs(c->getName()));
    ui->phoneEdit->setText(qs(c->getPhone()));
    ui->addressEdit->setText(qs(c->getAddress()));
    ui->rightStack->setCurrentWidget(ui->formPage);
}

void BorrowReturnPage::onCancelFormClicked() {
    if (m_selectedCustomerId == -1)
        showPlaceholder();
    else
        showDetail(m_selectedCustomerId);
}

void BorrowReturnPage::onSaveCustomerClicked() {
    const QString name = ui->nameEdit->text().trimmed();
    const QString phone = ui->phoneEdit->text().trimmed();
    const QString address = ui->addressEdit->text().trimmed();

    if (name.isEmpty()) {
        QMessageBox::warning(this, "Lỗi", "Tên khách hàng không được để trống.");
        return;
    }

    if (m_editingCustomerId == -1) {
        Customer c;
        c.setId(nextId(g_lib.customers));
        c.setName(ss(name));
        c.setPhone(ss(phone));
        c.setAddress(ss(address));
        c.setLock(false);
        g_lib.customers.push_back(c);
        Customer::save(PATH_CUSTOMER, g_lib.customers);
        m_selectedCustomerId = c.getId();
    } else {
        Customer* c = g_lib.findCustomer(m_editingCustomerId);
        if (!c)
            return;
        c->setName(ss(name));
        c->setPhone(ss(phone));
        c->setAddress(ss(address));
        Customer::save(PATH_CUSTOMER, g_lib.customers);
        m_selectedCustomerId = m_editingCustomerId;
    }

    refresh();
}

void BorrowReturnPage::onToggleLockClicked() {
    Customer* c = g_lib.findCustomer(m_selectedCustomerId);
    if (!c)
        return;
    if (c->isLocked())
        g_lib.unlockCustomer(c->getId());
    else
        g_lib.lockCustomer(c->getId());
    refresh();
}

void BorrowReturnPage::onDeleteCustomerClicked() {
    if (m_selectedCustomerId == -1)
        return;
    if (QMessageBox::question(this, "Xác nhận",
                              "Bạn chắc chắn muốn xóa khách hàng này?")
        != QMessageBox::Yes)
        return;

    if (g_lib.removeCustomer(m_selectedCustomerId)) {
        m_selectedCustomerId = -1;
        refresh();
    } else {
        QMessageBox::warning(this, "Lỗi",
            "Không thể xóa (khách còn sách chưa trả).");
    }
}

void BorrowReturnPage::onBorrowClicked() {
    Customer* c = g_lib.findCustomer(m_selectedCustomerId);
    if (!c)
        return;
    if (c->isLocked()) {
        QMessageBox::warning(this, "Lỗi", "Khách hàng đang bị khóa, không thể mượn.");
        return;
    }

    BorrowCartDialog dlg(m_selectedCustomerId, this);
    if (dlg.exec() == QDialog::Accepted)
        refresh();
}


