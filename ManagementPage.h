#pragma once
#include <QWidget>
#include "globals.h"

namespace Ui { class ManagementPage; }

class ManagementPage : public QWidget {
    Q_OBJECT
public:
    explicit ManagementPage(QWidget* parent = nullptr);
    ~ManagementPage() override;

    void refresh();

private slots:
    void onBookSearchChanged(const QString& text);
    void onAddBookClicked();
    void onEditBookClicked();
    void onDeleteBookClicked();
    void onStockOutClicked();

    void onAuthorSearchChanged(const QString& text);
    void onAddAuthorClicked();
    void onEditAuthorClicked();
    void onDeleteAuthorClicked();

    void onGenreSearchChanged(const QString& text);
    void onAddGenreClicked();
    void onEditGenreClicked();
    void onDeleteGenreClicked();

private:
    Ui::ManagementPage* ui;

    void refreshBooks();
    void refreshAuthors();
    void refreshGenres();

    int selectedBookId() const;
    int selectedAuthorId() const;
    int selectedGenreId() const;
};
