#include "BookEntryDialog.h"
#include "ui_BookEntryDialog.h"

#include <QMessageBox>
#include <QPushButton>

BookEntryDialog::BookEntryDialog(Mode mode, int bookId, QWidget* parent)
    : QDialog(parent), ui(new Ui::BookEntryDialog),
      m_mode(mode), m_editBookId(bookId) {
    ui->setupUi(this);

    populateAuthors();
    populateGenres(List<int>());   // luôn nạp danh sách thể loại

    if (mode == Mode::New) {
        setWindowTitle("Nhập sách");
        ui->amountLabel->setText("Số lượng:");
        ui->amountSpin->setRange(1, 99999);
        ui->amountSpin->setValue(1);
        ui->nameEdit->setFocus();
    } else { // Edit
        setWindowTitle("Sửa sách");
        ui->checkBtn->setVisible(false);
        ui->amountLabel->setText("Số lượng:");
        loadBookForEdit(bookId);
    }

    connect(ui->nameEdit, &QLineEdit::returnPressed,
            this, &BookEntryDialog::onNameEntered);
    connect(ui->checkBtn, &QPushButton::clicked,
            this, &BookEntryDialog::onCheckClicked);
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

bool BookEntryDialog::sameGenres(const List<int>& a, const List<int>& b) const {
    if (a.size() != b.size())
        return false;
    for (int i = 0; i < (int)a.size(); ++i) {
        bool found = false;
        for (int j = 0; j < (int)b.size(); ++j)
            if (a[i] == b[j]) { found = true; break; }
        if (!found)
            return false;
    }
    return true;
}

void BookEntryDialog::checkName() {
    const std::string name = ss(ui->nameEdit->text().trimmed());
    if (name.empty())
        return;

    int foundId = -1;
    if (findBookByName(name, &foundId)) {
        fillFromBook(foundId);
        ui->amountLabel->setText("Số bản thêm vào:");
        ui->amountSpin->setRange(1, 99999);
        ui->amountSpin->setValue(1);
        ui->hintLabel->setText(
            QString("Đã tìm thấy sách hiện có (ID %1). Nhập số bản cần thêm.").arg(foundId));
    } else {
        ui->amountLabel->setText("Số lượng:");
        ui->amountSpin->setRange(1, 99999);
        ui->hintLabel->setText("Không tìm thấy sách trùng tên. Sẽ tạo sách mới.");
    }
}

void BookEntryDialog::fillFromBook(int bookId) {
    Book* b = g_lib.findBook(bookId);
    if (!b)
        return;
    int idx = ui->authorCombo->findData(b->getAuthorId());
    if (idx >= 0)
        ui->authorCombo->setCurrentIndex(idx);
    populateGenres(b->getGenreIds());
}

void BookEntryDialog::loadBookForEdit(int bookId) {
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

void BookEntryDialog::onCheckClicked() {
    checkName();
}

void BookEntryDialog::onNameEntered() {
    if (m_mode != Mode::New)
        return;
    checkName();
}

void BookEntryDialog::accept() {
    const std::string name = ss(ui->nameEdit->text().trimmed());
    const int authorId = ui->authorCombo->currentData().toInt();
    const int amount = ui->amountSpin->value();
    const List<int> genres = selectedGenres();

    // ---- Sửa sách ----
    if (m_mode == Mode::Edit) {
        if (name.empty()) { QMessageBox::warning(this, "Lỗi", "Tên sách không được để trống."); return; }
        if (authorId < 0)  { QMessageBox::warning(this, "Lỗi", "Vui lòng chọn tác giả."); return; }
        if (amount <= 0)   { QMessageBox::warning(this, "Lỗi", "Số lượng phải lớn hơn 0."); return; }

        Book* b = g_lib.findBook(m_editBookId);
        if (!b)
            return;
        if (g_lib.bookExists(name, authorId, m_editBookId)) {
            QMessageBox::warning(this, "Lỗi", "Sách này đã tồn tại (cùng tên + cùng tác giả).");
            return;
        }
        b->setName(name);
        b->setAuthorId(authorId);
        b->setAmount(amount);
        b->setGenreIds(genres);
        Book::save(PATH_BOOK, g_lib.books);
        QDialog::accept();
        return;
    }

    // ---- Nhập sách ----
    if (name.empty()) { QMessageBox::warning(this, "Lỗi", "Tên sách không được để trống."); return; }
    if (authorId < 0)  { QMessageBox::warning(this, "Lỗi", "Vui lòng chọn tác giả."); return; }
    if (amount <= 0)   { QMessageBox::warning(this, "Lỗi", "Số lượng phải lớn hơn 0."); return; }

    auto createNew = [&]() -> bool {
        if (g_lib.bookExists(name, authorId))
            return false;
        Book b;
        b.setId(nextId(g_lib.books));
        b.setName(name);
        b.setAuthorId(authorId);
        b.setAmount(amount);
        b.setGenreIds(genres);
        g_lib.books.push_back(b);
        Book::save(PATH_BOOK, g_lib.books);
        return true;
    };

    // kiểm tra lại tên ngay tại thời điểm OK
    int foundId = -1;
    findBookByName(name, &foundId);

    if (foundId == -1) {
        if (!createNew())
            QMessageBox::warning(this, "Lỗi", "Sách này đã tồn tại (cùng tên + cùng tác giả).");
        else
            QDialog::accept();
        return;
    }

    Book* found = g_lib.findBook(foundId);
    if (!found) {
        if (!createNew())
            QMessageBox::warning(this, "Lỗi", "Sách này đã tồn tại (cùng tên + cùng tác giả).");
        else
            QDialog::accept();
        return;
    }

    const bool sameAuthor = (authorId == found->getAuthorId());
    const bool sameGen = sameGenres(genres, found->getGenreIds());

    if (sameAuthor && sameGen) {
        // không thay đổi thông tin -> chỉ thêm số lượng
        found->setAmount(found->getAmount() + amount);
        Book::save(PATH_BOOK, g_lib.books);
        QDialog::accept();
        return;
    }

    // tác giả / thể loại đã thay đổi -> hỏi lại
    QMessageBox box(this);
    box.setWindowTitle("Thông tin đã thay đổi");
    box.setText("Tác giả / thể loại đã khác với sách hiện có.\nBạn muốn làm gì?");
    QPushButton* createBtn = box.addButton("Tạo sách mới", QMessageBox::AcceptRole);
    QPushButton* addBtn    = box.addButton("Thêm vào sách cũ", QMessageBox::ActionRole);
    box.addButton("Hủy", QMessageBox::RejectRole);
    box.exec();

    if (box.clickedButton() == addBtn) {
        // thêm vào sách cũ, giữ nguyên thông tin sách cũ trong data
        found->setAmount(found->getAmount() + amount);
        Book::save(PATH_BOOK, g_lib.books);
        QDialog::accept();
    } else if (box.clickedButton() == createBtn) {
        if (!createNew())
            QMessageBox::warning(this, "Lỗi", "Sách này đã tồn tại (cùng tên + cùng tác giả).");
        else
            QDialog::accept();
    }
    // Hủy: giữ nguyên dialog để người dùng sửa tiếp
}

