#include "BookEntryDialog.h"
#include "ui_BookEntryDialog.h"

#include <QMessageBox>
#include <QPushButton>

BookEntryDialog::BookEntryDialog(Mode mode, int bookId, QWidget* parent)
    : QDialog(parent), ui(new Ui::BookEntryDialog), m_mode(mode), m_bookId(bookId) {
    ui->setupUi(this);

    populateAuthors();

    if (mode == Mode::New) {
        setWindowTitle("Nhập sách mới");
        ui->amountLabel->setText("Số lượng:");
        ui->amountSpin->setRange(1, 99999);
        ui->amountSpin->setValue(1);
        ui->nameEdit->setFocus();
    } else if (mode == Mode::Edit) {
        setWindowTitle("Sửa sách");
        ui->amountLabel->setText("Số lượng:");
        loadBook(bookId);
    } else { // StockIn
        setWindowTitle("Thêm bản vào sách");
        ui->amountLabel->setText("Số bản thêm vào:");
        ui->amountSpin->setRange(1, 99999);
        ui->amountSpin->setValue(1);
        switchToStockIn(bookId);
    }

    connect(ui->nameEdit, &QLineEdit::returnPressed,
            this, &BookEntryDialog::onNameEntered);
}

BookEntryDialog::~BookEntryDialog() {
    delete ui;
}

void BookEntryDialog::populateAuthors() {
    ui->authorCombo->clear();
    for (int i = 0; i < (int)g_lib.authors.size(); ++i)
        ui->authorCombo->addItem(qs(g_lib.authors[i].getName()),
                                 g_lib.authors[i].getId());
    if (ui->authorCombo->count() == 0)
        ui->authorCombo->addItem("(Chưa có tác giả)", -1);
}

void BookEntryDialog::populateGenres(const List<int>& selected) {
    ui->genreList->clear();
    for (int i = 0; i < (int)g_lib.genres.size(); ++i) {
        QListWidgetItem* item = new QListWidgetItem(qs(g_lib.genres[i].getName()));
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
        item->setData(Qt::UserRole, g_lib.genres[i].getId());

        bool checked = false;
        for (int j = 0; j < (int)selected.size(); ++j)
            if (selected[j] == g_lib.genres[i].getId()) { checked = true; break; }
        item->setCheckState(checked ? Qt::Checked : Qt::Unchecked);
        ui->genreList->addItem(item);
    }
}

List<int> BookEntryDialog::selectedGenres() const {
    List<int> ids;
    for (int i = 0; i < ui->genreList->count(); ++i) {
        QListWidgetItem* item = ui->genreList->item(i);
        if (item->checkState() == Qt::Checked)
            ids.push_back(item->data(Qt::UserRole).toInt());
    }
    return ids;
}

bool BookEntryDialog::findBookByName(const std::string& name, int* outId) const {
    const std::string n = normalize(name);
    for (int i = 0; i < (int)g_lib.books.size(); ++i) {
        if (normalize(g_lib.books[i].getName()) == n) {
            *outId = g_lib.books[i].getId();
            return true;
        }
    }
    return false;
}

void BookEntryDialog::onNameEntered() {
    if (m_mode != Mode::New)
        return;
    const std::string name = ss(ui->nameEdit->text().trimmed());
    if (name.empty())
        return;

    int foundId = -1;
    if (!findBookByName(name, &foundId))
        return;

    QMessageBox box(this);
    box.setWindowTitle("Sách đã tồn tại");
    box.setText(QString("Sách \"%1\" đã có trong thư viện.\nBạn muốn làm gì?")
                    .arg(qs(name)));
    QPushButton* createBtn = box.addButton("Tạo sách mới", QMessageBox::AcceptRole);
    QPushButton* addBtn    = box.addButton("Thêm bản vào sách này", QMessageBox::ActionRole);
    box.addButton("Hủy", QMessageBox::RejectRole);
    box.exec();

    if (box.clickedButton() == addBtn)
        switchToStockIn(foundId);
    // createBtn: giữ nguyên chế độ nhập mới, người dùng tiếp tục điền
}

void BookEntryDialog::loadBook(int bookId) {
    Book* b = g_lib.findBook(bookId);
    if (!b)
        return;
    ui->nameEdit->setText(qs(b->getName()));
    int idx = ui->authorCombo->findData(b->getAuthorId());
    if (idx >= 0)
        ui->authorCombo->setCurrentIndex(idx);
    populateGenres(b->getGenreIds());

    const int borrowed = b->getAmount() - g_lib.available(bookId);
    ui->amountSpin->setRange(borrowed, 99999);
    ui->amountSpin->setValue(b->getAmount());
}

void BookEntryDialog::switchToStockIn(int bookId) {
    m_mode = Mode::StockIn;
    m_bookId = bookId;
    setWindowTitle("Thêm bản vào sách");

    Book* b = g_lib.findBook(bookId);
    if (!b)
        return;

    ui->nameEdit->setText(qs(b->getName()));
    ui->nameEdit->setReadOnly(true);

    int idx = ui->authorCombo->findData(b->getAuthorId());
    if (idx >= 0)
        ui->authorCombo->setCurrentIndex(idx);
    ui->authorCombo->setEnabled(false);

    populateGenres(b->getGenreIds());
    ui->genreList->setEnabled(false);

    ui->amountLabel->setText("Số bản thêm vào:");
    ui->amountSpin->setRange(1, 99999);
    ui->amountSpin->setValue(1);

    ui->hintLabel->setText(QString("Thêm bản vào sách \"%1\" (hiện có %2 bản).")
                               .arg(qs(b->getName()))
                               .arg(b->getAmount()));
    ui->amountSpin->setFocus();
    ui->amountSpin->selectAll();
}

void BookEntryDialog::accept() {
    const std::string name = ss(ui->nameEdit->text().trimmed());
    const int authorId = ui->authorCombo->currentData().toInt();
    const int amount = ui->amountSpin->value();
    const List<int> genres = selectedGenres();

    if (m_mode == Mode::StockIn) {
        Book* b = g_lib.findBook(m_bookId);
        if (!b)
            return;
        b->setAmount(b->getAmount() + amount);
        Book::save(PATH_BOOK, g_lib.books);
        QDialog::accept();
        return;
    }

    if (name.empty()) {
        QMessageBox::warning(this, "Lỗi", "Tên sách không được để trống.");
        return;
    }
    if (authorId < 0) {
        QMessageBox::warning(this, "Lỗi", "Vui lòng chọn tác giả.");
        return;
    }
    if (amount <= 0) {
        QMessageBox::warning(this, "Lỗi", "Số lượng phải lớn hơn 0.");
        return;
    }

    if (m_mode == Mode::New) {
        if (g_lib.bookExists(name, authorId)) {
            QMessageBox::warning(this, "Lỗi",
                "Sách này đã tồn tại (cùng tên + cùng tác giả).");
            return;
        }
        Book b;
        b.setId(nextId(g_lib.books));
        b.setName(name);
        b.setAuthorId(authorId);
        b.setAmount(amount);
        b.setGenreIds(genres);
        g_lib.books.push_back(b);
        Book::save(PATH_BOOK, g_lib.books);
        QDialog::accept();
        return;
    }

    // Edit
    Book* b = g_lib.findBook(m_bookId);
    if (!b)
        return;
    if (g_lib.bookExists(name, authorId, m_bookId)) {
        QMessageBox::warning(this, "Lỗi",
            "Sách này đã tồn tại (cùng tên + cùng tác giả).");
        return;
    }
    b->setName(name);
    b->setAuthorId(authorId);
    b->setAmount(amount);
    b->setGenreIds(genres);
    Book::save(PATH_BOOK, g_lib.books);
    QDialog::accept();
}


