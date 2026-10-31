#include "DashboardPage.h"
#include "ui_DashboardPage.h"

DashboardPage::DashboardPage(QWidget* parent)
    : QWidget(parent), ui(new Ui::DashboardPage) {
    ui->setupUi(this);

    ui->pageTitle->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

    ui->cardOverdue->installEventFilter(this);
    ui->cardOverdueTitle->setAttribute(Qt::WA_TransparentForMouseEvents);
    ui->cardOverdueValue->setAttribute(Qt::WA_TransparentForMouseEvents);
    ui->cardOverdue->setCursor(Qt::PointingHandCursor);

    ui->overdueSection->setVisible(false);
    ui->overdueTable->setColumnCount(3);
    ui->overdueTable->setHorizontalHeaderLabels(
        QStringList() << "Khách hàng" << "Số điện thoại" << "Sách quá hạn");
    ui->overdueTable->horizontalHeader()->setStretchLastSection(true);
    ui->overdueTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->overdueTable->setSelectionBehavior(QAbstractItemView::SelectRows);
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

bool DashboardPage::eventFilter(QObject* watched, QEvent* event) {
    if (watched == ui->cardOverdue && event->type() == QEvent::MouseButtonPress) {
        onOverdueClicked();
        return true;
    }
    return QWidget::eventFilter(watched, event);
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

    ui->cardBooksValue->setText(QString::number(totalBooks));
    ui->cardCustomersValue->setText(QString::number(totalCustomers));
    ui->cardBorrowingValue->setText(QString::number(active));
    ui->cardOverdueValue->setText(QString::number(overdue));
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
