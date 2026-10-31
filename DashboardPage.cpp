#include "DashboardPage.h"
#include "ui_DashboardPage.h"

DashboardPage::DashboardPage(QWidget* parent)
    : QWidget(parent), ui(new Ui::DashboardPage) {
    ui->setupUi(this);

    ui->overdueSection->setVisible(false);
    ui->overdueTable->setColumnCount(3);
    ui->overdueTable->setHorizontalHeaderLabels(
        QStringList() << "Khách hàng" << "Số điện thoại" << "Sách quá hạn");
    ui->overdueTable->horizontalHeader()->setStretchLastSection(true);
    ui->overdueTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->overdueTable->setSelectionBehavior(QAbstractItemView::SelectRows);

    connect(ui->cardOverdue, &QPushButton::clicked,
            this, &DashboardPage::onOverdueClicked);
}

DashboardPage::~DashboardPage() {
    delete ui;
}

void DashboardPage::refresh() {
    refreshCards();
    refreshOverdueTable();
}

void DashboardPage::onOverdueClicked() {
    m_overdueVisible = !m_overdueVisible;
    ui->overdueSection->setVisible(m_overdueVisible);
    if (m_overdueVisible)
        refreshOverdueTable();
}

void DashboardPage::refreshCards() {
    const int totalBooks     = (int)g_lib.books.size();
    const int totalCustomers = (int)g_lib.customers.size();

    int active  = 0;
    int overdue = 0;
    for (int i = 0; i < (int)g_lib.borrows.size(); ++i) {
        if (g_lib.borrows[i].isReturned())
            continue;
        ++active;
        if (isOverdue(g_lib.borrows[i].getDueDate()))
            ++overdue;
    }

    ui->cardBooks->setText(
        QString("Tổng số sách<br><span style='font-size:26px;font-weight:700;'>%1</span>")
            .arg(totalBooks));
    ui->cardCustomers->setText(
        QString("Khách hàng<br><span style='font-size:26px;font-weight:700;'>%1</span>")
            .arg(totalCustomers));
    ui->cardBorrowing->setText(
        QString("Đang mượn<br><span style='font-size:26px;font-weight:700;'>%1</span>")
            .arg(active));
    ui->cardOverdue->setText(
        QString("Quá hạn<br><span style='font-size:26px;font-weight:700;color:#ef4444;'>%1</span>")
            .arg(overdue));
}

void DashboardPage::refreshOverdueTable() {
    ui->overdueTable->setRowCount(0);

    for (int ci = 0; ci < (int)g_lib.customers.size(); ++ci) {
        const int cid = g_lib.customers[ci].getId();
        QStringList books;

        for (int bi = 0; bi < (int)g_lib.borrows.size(); ++bi) {
            const Borrow& bw = g_lib.borrows[bi];
            if (bw.getCustomerId() != cid || bw.isReturned())
                continue;
            if (!isOverdue(bw.getDueDate()))
                continue;
            Book* b = g_lib.findBook(bw.getBookId());
            books << QString("%1 (quá %2 ngày)")
                        .arg(b ? qs(b->getName()) : QString("?"))
                        .arg(overdueDays(bw.getDueDate()));
        }

        if (books.isEmpty())
            continue;

        const int row = ui->overdueTable->rowCount();
        ui->overdueTable->insertRow(row);
        ui->overdueTable->setItem(row, 0, new QTableWidgetItem(qs(g_lib.customers[ci].getName())));
        ui->overdueTable->setItem(row, 1, new QTableWidgetItem(qs(g_lib.customers[ci].getPhone())));
        ui->overdueTable->setItem(row, 2, new QTableWidgetItem(books.join(";  ")));
    }
}
