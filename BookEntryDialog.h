#pragma once
#include <QDialog>
#include "globals.h"

namespace Ui { class BookEntryDialog; }

// Dialog nhập / sửa sách.
// - New : "Nhập sách" thông minh (kiểm tra tên trùng, thêm bản vào sách cũ hoặc tạo mới)
// - Edit: "Sửa sách" (sửa thông tin sách)
class BookEntryDialog : public QDialog {
    Q_OBJECT
public:
    enum class Mode { New, Edit };

    explicit BookEntryDialog(Mode mode = Mode::New,
                             int bookId = -1,
                             QWidget* parent = nullptr);
    ~BookEntryDialog() override;

private slots:
    void onCheckClicked();   // nút "Kiểm tra"
    void onNameEntered();    // nhấn Enter ở ô tên
    void accept() override;  // OK

private:
    Ui::BookEntryDialog* ui;
    Mode m_mode;
    int  m_editBookId;

    void populateAuthors();
    void populateGenres(const List<int>& selected);
    List<int> selectedGenres() const;
    bool findBookByName(const std::string& name, int* outId) const;
    bool sameGenres(const List<int>& a, const List<int>& b) const;

    void checkName();             // kiểm tra tên + tự fill tác giả/thể loại nếu có
    void fillFromBook(int bookId);
    void loadBookForEdit(int bookId);
};
