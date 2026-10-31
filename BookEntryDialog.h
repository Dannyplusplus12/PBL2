#pragma once
#include <QDialog>
#include "globals.h"

namespace Ui { class BookEntryDialog; }

// Dialog nhập / sửa sách.
// - New     : nhập sách mới (kèm tìm trùng tên khi nhấn Enter)
// - Edit    : sửa sách đã có
// - StockIn : thêm bản vào sách đã có (tự fill dữ liệu cũ)
class BookEntryDialog : public QDialog {
    Q_OBJECT
public:
    enum class Mode { New, Edit, StockIn };

    explicit BookEntryDialog(Mode mode = Mode::New,
                             int bookId = -1,
                             QWidget* parent = nullptr);
    ~BookEntryDialog() override;

private slots:
    void onNameEntered();
    void accept() override;

private:
    Ui::BookEntryDialog* ui;
    Mode m_mode;
    int  m_bookId;

    void populateAuthors();
    void populateGenres(const List<int>& selected);
    List<int> selectedGenres() const;
    bool findBookByName(const std::string& name, int* outId) const;
    void loadBook(int bookId);
    void switchToStockIn(int bookId);
};
