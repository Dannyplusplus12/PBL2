#include "ManagementPage.h"
#include "ui_ManagementPage.h"

#include "BookEntryDialog.h"
#include "NameDialog.h"

#include <QMessageBox>

ManagementPage::ManagementPage(QWidget* parent)
    : QWidget(parent), ui(new Ui::ManagementPage) {
    ui->setupUi(this);

    ui->pageTitle->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

    ui->booksTable->setColumnCount(6);
    ui->booksTable->setHorizontalHeaderLabels(
        QStringList() << "ID" << "Tên sách" << "Tác giả"
                      << "Số lượng" << "Còn lại" << "Thể loại");
    ui->booksTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->booksTable->horizontalHeader()->setSectionResizeMode(5, QHeaderView::Stretch);
    ui->booksTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->booksTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->booksTable->verticalHeader()->setVisible(false);

    ui->authorsTable->setColumnCount(2);
    ui->authorsTable->setHorizontalHeaderLabels(QStringList() << "ID" << "Tên tác giả");
    ui->authorsTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->authorsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->authorsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->authorsTable->verticalHeader()->setVisible(false);

    ui->genresTable->setColumnCount(2);
    ui->genresTable->setHorizontalHeaderLabels(QStringList() << "ID" << "Tên thể loại");
    ui->genresTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->genresTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->genresTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->genresTable->verticalHeader()->setVisible(false);

    connect(ui->searchBookEdit, &QLineEdit::textChanged,
            this, &ManagementPage::onBookSearchChanged);
    connect(ui->addBookBtn, &QPushButton::clicked, this, &ManagementPage::onAddBookClicked);
    connect(ui->editBookBtn, &QPushButton::clicked, this, &ManagementPage::onEditBookClicked);
    connect(ui->deleteBookBtn, &QPushButton::clicked, this, &ManagementPage::onDeleteBookClicked);

    connect(ui->searchAuthorEdit, &QLineEdit::textChanged,
            this, &ManagementPage::onAuthorSearchChanged);
    connect(ui->addAuthorBtn, &QPushButton::clicked, this, &ManagementPage::onAddAuthorClicked);
    connect(ui->editAuthorBtn, &QPushButton::clicked, this, &ManagementPage::onEditAuthorClicked);
    connect(ui->deleteAuthorBtn, &QPushButton::clicked, this, &ManagementPage::onDeleteAuthorClicked);

    connect(ui->searchGenreEdit, &QLineEdit::textChanged,
            this, &ManagementPage::onGenreSearchChanged);
    connect(ui->addGenreBtn, &QPushButton::clicked, this, &ManagementPage::onAddGenreClicked);
    connect(ui->editGenreBtn, &QPushButton::clicked, this, &ManagementPage::onEditGenreClicked);
    connect(ui->deleteGenreBtn, &QPushButton::clicked, this, &ManagementPage::onDeleteGenreClicked);
}

ManagementPage::~ManagementPage() {
    delete ui;
}

void ManagementPage::refresh() {
    refreshBooks();
    refreshAuthors();
    refreshGenres();
}

void ManagementPage::refreshBooks() {
    const QString filter = ui->searchBookEdit->text().trimmed();
    ui->booksTable->setRowCount(0);

    for (int i = 0; i < (int)g_lib.books.size(); ++i) {
        const Book& b = g_lib.books[i];
        Author* a = g_lib.findAuthor(b.getAuthorId());
        const QString name = qs(b.getName());
        const QString authorName = a ? qs(a->getName()) : QString("?");

        if (!filter.isEmpty()
            && !name.contains(filter, Qt::CaseInsensitive)
            && !authorName.contains(filter, Qt::CaseInsensitive))
            continue;

        QString genres;
        const List<int>& gids = b.getGenreIds();
        for (int j = 0; j < (int)gids.size(); ++j) {
            Genre* g = g_lib.findGenre(gids[j]);
            if (g) {
                if (!genres.isEmpty()) genres += ", ";
                genres += qs(g->getName());
            }
        }

        const int row = ui->booksTable->rowCount();
        ui->booksTable->insertRow(row);
        QTableWidgetItem* idItem = new QTableWidgetItem(QString::number(b.getId()));
        idItem->setData(Qt::UserRole, b.getId());
        ui->booksTable->setItem(row, 0, idItem);
        ui->booksTable->setItem(row, 1, new QTableWidgetItem(name));
        ui->booksTable->setItem(row, 2, new QTableWidgetItem(authorName));
        ui->booksTable->setItem(row, 3, new QTableWidgetItem(QString::number(b.getAmount())));
        ui->booksTable->setItem(row, 4, new QTableWidgetItem(QString::number(g_lib.available(b.getId()))));
        ui->booksTable->setItem(row, 5, new QTableWidgetItem(genres));
    }
}

void ManagementPage::refreshAuthors() {
    const QString filter = ui->searchAuthorEdit->text().trimmed();
    ui->authorsTable->setRowCount(0);

    for (int i = 0; i < (int)g_lib.authors.size(); ++i) {
        const Author& a = g_lib.authors[i];
        const QString name = qs(a.getName());
        if (!filter.isEmpty() && !name.contains(filter, Qt::CaseInsensitive))
            continue;

        const int row = ui->authorsTable->rowCount();
        ui->authorsTable->insertRow(row);
        QTableWidgetItem* idItem = new QTableWidgetItem(QString::number(a.getId()));
        idItem->setData(Qt::UserRole, a.getId());
        ui->authorsTable->setItem(row, 0, idItem);
        ui->authorsTable->setItem(row, 1, new QTableWidgetItem(name));
    }
}

void ManagementPage::refreshGenres() {
    const QString filter = ui->searchGenreEdit->text().trimmed();
    ui->genresTable->setRowCount(0);

    for (int i = 0; i < (int)g_lib.genres.size(); ++i) {
        const Genre& g = g_lib.genres[i];
        const QString name = qs(g.getName());
        if (!filter.isEmpty() && !name.contains(filter, Qt::CaseInsensitive))
            continue;

        const int row = ui->genresTable->rowCount();
        ui->genresTable->insertRow(row);
        QTableWidgetItem* idItem = new QTableWidgetItem(QString::number(g.getId()));
        idItem->setData(Qt::UserRole, g.getId());
        ui->genresTable->setItem(row, 0, idItem);
        ui->genresTable->setItem(row, 1, new QTableWidgetItem(name));
    }
}

int ManagementPage::selectedBookId() const {
    const int row = ui->booksTable->currentRow();
    if (row < 0) return -1;
    QTableWidgetItem* it = ui->booksTable->item(row, 0);
    return it ? it->data(Qt::UserRole).toInt() : -1;
}

int ManagementPage::selectedAuthorId() const {
    const int row = ui->authorsTable->currentRow();
    if (row < 0) return -1;
    QTableWidgetItem* it = ui->authorsTable->item(row, 0);
    return it ? it->data(Qt::UserRole).toInt() : -1;
}

int ManagementPage::selectedGenreId() const {
    const int row = ui->genresTable->currentRow();
    if (row < 0) return -1;
    QTableWidgetItem* it = ui->genresTable->item(row, 0);
    return it ? it->data(Qt::UserRole).toInt() : -1;
}

void ManagementPage::onBookSearchChanged(const QString&) {
    refreshBooks();
}

void ManagementPage::onAddBookClicked() {
    BookEntryDialog dlg(BookEntryDialog::Mode::New, -1, this);
    if (dlg.exec() == QDialog::Accepted)
        refreshBooks();
}

void ManagementPage::onEditBookClicked() {
    const int id = selectedBookId();
    if (id < 0) {
        QMessageBox::information(this, "Thông báo", "Chọn một sách để sửa.");
        return;
    }
    BookEntryDialog dlg(BookEntryDialog::Mode::Edit, id, this);
    if (dlg.exec() == QDialog::Accepted)
        refreshBooks();
}

void ManagementPage::onDeleteBookClicked() {
    const int id = selectedBookId();
    if (id < 0) {
        QMessageBox::information(this, "Thông báo", "Chọn một sách để xóa.");
        return;
    }
    if (QMessageBox::question(this, "Xác nhận", "Bạn chắc chắn muốn xóa sách này?")
        != QMessageBox::Yes)
        return;
    if (g_lib.removeBook(id))
        refreshBooks();
    else
        QMessageBox::warning(this, "Lỗi", "Không thể xóa (sách đang có người mượn).");
}

void ManagementPage::onAuthorSearchChanged(const QString&) {
    refreshAuthors();
}

void ManagementPage::onAddAuthorClicked() {
    NameDialog dlg("Thêm tác giả", "", this);
    if (dlg.exec() != QDialog::Accepted)
        return;
    const std::string n = ss(dlg.name());
    if (n.empty()) {
        QMessageBox::warning(this, "Lỗi", "Tên không được để trống.");
        return;
    }
    if (g_lib.authorExists(n)) {
        QMessageBox::warning(this, "Lỗi", "Tác giả này đã tồn tại.");
        return;
    }
    Author a;
    a.setId(nextId(g_lib.authors));
    a.setName(n);
    g_lib.authors.push_back(a);
    Author::save(PATH_AUTHOR, g_lib.authors);
    refreshAuthors();
}

void ManagementPage::onEditAuthorClicked() {
    const int id = selectedAuthorId();
    if (id < 0) {
        QMessageBox::information(this, "Thông báo", "Chọn một tác giả để sửa.");
        return;
    }
    Author* a = g_lib.findAuthor(id);
    if (!a)
        return;
    NameDialog dlg("Sửa tác giả", qs(a->getName()), this);
    if (dlg.exec() != QDialog::Accepted)
        return;
    const std::string n = ss(dlg.name());
    if (n.empty()) {
        QMessageBox::warning(this, "Lỗi", "Tên không được để trống.");
        return;
    }
    if (g_lib.authorExists(n, id)) {
        QMessageBox::warning(this, "Lỗi", "Tác giả này đã tồn tại.");
        return;
    }
    a->setName(n);
    Author::save(PATH_AUTHOR, g_lib.authors);
    refreshAuthors();
}

void ManagementPage::onDeleteAuthorClicked() {
    const int id = selectedAuthorId();
    if (id < 0) {
        QMessageBox::information(this, "Thông báo", "Chọn một tác giả để xóa.");
        return;
    }
    if (QMessageBox::question(this, "Xác nhận", "Bạn chắc chắn muốn xóa tác giả này?")
        != QMessageBox::Yes)
        return;
    if (g_lib.removeAuthor(id))
        refreshAuthors();
    else
        QMessageBox::warning(this, "Lỗi", "Không thể xóa (còn sách tham chiếu tới tác giả).");
}

void ManagementPage::onGenreSearchChanged(const QString&) {
    refreshGenres();
}

void ManagementPage::onAddGenreClicked() {
    NameDialog dlg("Thêm thể loại", "", this);
    if (dlg.exec() != QDialog::Accepted)
        return;
    const std::string n = ss(dlg.name());
    if (n.empty()) {
        QMessageBox::warning(this, "Lỗi", "Tên không được để trống.");
        return;
    }
    if (g_lib.genreExists(n)) {
        QMessageBox::warning(this, "Lỗi", "Thể loại này đã tồn tại.");
        return;
    }
    Genre g;
    g.setId(nextId(g_lib.genres));
    g.setName(n);
    g_lib.genres.push_back(g);
    Genre::save(PATH_GENRE, g_lib.genres);
    refreshGenres();
}

void ManagementPage::onEditGenreClicked() {
    const int id = selectedGenreId();
    if (id < 0) {
        QMessageBox::information(this, "Thông báo", "Chọn một thể loại để sửa.");
        return;
    }
    Genre* g = g_lib.findGenre(id);
    if (!g)
        return;
    NameDialog dlg("Sửa thể loại", qs(g->getName()), this);
    if (dlg.exec() != QDialog::Accepted)
        return;
    const std::string n = ss(dlg.name());
    if (n.empty()) {
        QMessageBox::warning(this, "Lỗi", "Tên không được để trống.");
        return;
    }
    if (g_lib.genreExists(n, id)) {
        QMessageBox::warning(this, "Lỗi", "Thể loại này đã tồn tại.");
        return;
    }
    g->setName(n);
    Genre::save(PATH_GENRE, g_lib.genres);
    refreshGenres();
}

void ManagementPage::onDeleteGenreClicked() {
    const int id = selectedGenreId();
    if (id < 0) {
        QMessageBox::information(this, "Thông báo", "Chọn một thể loại để xóa.");
        return;
    }
    if (QMessageBox::question(this, "Xác nhận", "Bạn chắc chắn muốn xóa thể loại này?")
        != QMessageBox::Yes)
        return;
    if (g_lib.removeGenre(id))
        refreshGenres();
    else
        QMessageBox::warning(this, "Lỗi", "Không thể xóa (còn sách dùng thể loại này).");
}



