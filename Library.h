#pragma once
#include "Author.h"
#include "Genre.h"
#include "Customer.h"
#include "Book.h"
#include "Borrow.h"

// duong dan file du lieu
const string PATH_AUTHOR   = "Data/Author.dat";
const string PATH_GENRE    = "Data/Genre.dat";
const string PATH_CUSTOMER = "Data/Customer.dat";
const string PATH_BOOK     = "Data/Book.dat";
const string PATH_BORROW   = "Data/Borrow.dat";

class Library {
public:
    vector<Author>   authors;
    vector<Genre>    genres;
    vector<Customer> customers;
    vector<Book>     books;
    vector<Borrow>   borrows;

    // --- load / save ---
    void load() {
        authors   = Author::load(PATH_AUTHOR);
        genres    = Genre::load(PATH_GENRE);
        customers = Customer::load(PATH_CUSTOMER);
        books     = Book::load(PATH_BOOK);
        borrows   = Borrow::load(PATH_BORROW);
    }

    // --- tim theo id ---
    Author*   findAuthor  (int id) { for (auto& x : authors)   if (x.id == id) return &x; return nullptr; }
    Genre*    findGenre   (int id) { for (auto& x : genres)    if (x.id == id) return &x; return nullptr; }
    Customer* findCustomer(int id) { for (auto& x : customers) if (x.id == id) return &x; return nullptr; }
    Book*     findBook    (int id) { for (auto& x : books)     if (x.id == id) return &x; return nullptr; }
    Borrow*   findBorrow  (int id) { for (auto& x : borrows)   if (x.id == id) return &x; return nullptr; }

    // so ban con lai cua sach
    int available(int bookId) {
        Book* b = findBook(bookId);
        if (!b) return 0;
        int n = 0;
        for (auto& bw : borrows)
            if (bw.bookId == bookId && !bw.returned) n++;
        return b->amount - n;
    }

    // muon sach — false neu het ban hoac id khong hop le
    bool borrow(int customerId, int bookId,
                const string& borrowDate, const string& dueDate) {
        if (!findCustomer(customerId) || !findBook(bookId)) return false;
        if (available(bookId) <= 0) return false;
        Borrow bw;
        bw.id         = nextId(borrows);
        bw.customerId = customerId;
        bw.bookId     = bookId;
        bw.borrowDate = borrowDate;
        bw.dueDate    = dueDate;
        borrows.push_back(bw);
        Borrow::save(PATH_BORROW, borrows);
        return true;
    }

    // tra sach — false neu phieu khong ton tai hoac da tra
    bool returnBook(int borrowId, const string& returnDate) {
        Borrow* bw = findBorrow(borrowId);
        if (!bw || bw->returned) return false;
        bw->returned   = true;
        bw->returnDate = returnDate;
        Borrow::save(PATH_BORROW, borrows);
        return true;
    }

    // xoa co kiem tra rang buoc
    bool removeAuthor(int id) {
        for (auto& b : books) if (b.authorId == id) return false;
        authors.erase(remove_if(authors.begin(), authors.end(),
            [id](const Author& a){ return a.id == id; }), authors.end());
        Author::save(PATH_AUTHOR, authors);
        return true;
    }

    bool removeGenre(int id) {
        for (auto& b : books)
            if (find(b.genreIds.begin(), b.genreIds.end(), id) != b.genreIds.end())
                return false;
        genres.erase(remove_if(genres.begin(), genres.end(),
            [id](const Genre& g){ return g.id == id; }), genres.end());
        Genre::save(PATH_GENRE, genres);
        return true;
    }

    bool removeBook(int id) {
        for (auto& bw : borrows) if (bw.bookId == id && !bw.returned) return false;
        books.erase(remove_if(books.begin(), books.end(),
            [id](const Book& b){ return b.id == id; }), books.end());
        Book::save(PATH_BOOK, books);
        return true;
    }

    bool removeCustomer(int id) {
        for (auto& bw : borrows) if (bw.customerId == id && !bw.returned) return false;
        customers.erase(remove_if(customers.begin(), customers.end(),
            [id](const Customer& c){ return c.id == id; }), customers.end());
        Customer::save(PATH_CUSTOMER, customers);
        return true;
    }
};
