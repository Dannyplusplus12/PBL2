#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "DashboardPage.h"
#include "BorrowReturnPage.h"
#include "ManagementPage.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    m_dashboard = new DashboardPage(this);
    m_borrow    = new BorrowReturnPage(this);
    m_manage    = new ManagementPage(this);

    ui->contentStack->addWidget(m_dashboard);   // index 0
    ui->contentStack->addWidget(m_borrow);      // index 1
    ui->contentStack->addWidget(m_manage);      // index 2

    connect(ui->navList, &QListWidget::currentRowChanged,
            this, &MainWindow::onNavChanged);

    ui->navList->setCurrentRow(0);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::onNavChanged(int row) {
    ui->contentStack->setCurrentIndex(row);
    switch (row) {
    case 0: m_dashboard->refresh(); break;
    case 1: m_borrow->refresh();    break;
    case 2: m_manage->refresh();    break;
    default: break;
    }
}
