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

    // --- load ---
    void load() {
        authors   = Author::load(PATH_AUTHOR);
        genres    = Genre::load(PATH_GENRE);
        customers = Customer::load(PATH_CUSTOMER);
        books     = Book::load(PATH_BOOK);
        borrows   = Borrow::load(PATH_BORROW);
    }

    // --- tim theo id ---
    Author* findAuthor(int id) {
        for (int i = 0; i < (int)authors.size(); i++)
            if (authors[i].getId() == id) return &authors[i];
        return NULL;
    }

    Genre* findGenre(int id) {
        for (int i = 0; i < (int)genres.size(); i++)
            if (genres[i].getId() == id) return &genres[i];
        return NULL;
    }

    Customer* findCustomer(int id) {
        for (int i = 0; i < (int)customers.size(); i++)
            if (customers[i].getId() == id) return &customers[i];
        return NULL;
    }

    Book* findBook(int id) {
        for (int i = 0; i < (int)books.size(); i++)
            if (books[i].getId() == id) return &books[i];
        return NULL;
    }

    Borrow* findBorrow(int id) {
        for (int i = 0; i < (int)borrows.size(); i++)
            if (borrows[i].getId() == id) return &borrows[i];
        return NULL;
    }

    // so ban con lai
    int available(int bookId) {
        Book* b = findBook(bookId);
        if (!b) return 0;
        int n = 0;
        for (int i = 0; i < (int)borrows.size(); i++)
            if (borrows[i].getBookId() == bookId && !borrows[i].isReturned()) n++;
        return b->getAmount() - n;
    }

    // muon sach
    bool borrow(int customerId, int bookId,
                const string& borrowDate, const string& dueDate) {
        if (!findCustomer(customerId) || !findBook(bookId)) return false;
        if (available(bookId) <= 0) return false;
        Borrow bw;
        bw.setId(nextId(borrows));
        bw.setCustomerId(customerId);
        bw.setBookId(bookId);
        bw.setBorrowDate(borrowDate);
        bw.setDueDate(dueDate);
        borrows.push_back(bw);
        Borrow::save(PATH_BORROW, borrows);
        return true;
    }

    // tra sach
    bool returnBook(int borrowId, const string& returnDate) {
        Borrow* bw = findBorrow(borrowId);
        if (!bw || bw->isReturned()) return false;
        bw->setReturned(true);
        bw->setReturnDate(returnDate);
        Borrow::save(PATH_BORROW, borrows);
        return true;
    }

    // xoa co kiem tra rang buoc
    bool removeAuthor(int id) {
        for (int i = 0; i < (int)books.size(); i++)
            if (books[i].getAuthorId() == id) return false;
        for (int i = 0; i < (int)authors.size(); i++) {
            if (authors[i].getId() == id) {
                authors.erase(authors.begin() + i);
                Author::save(PATH_AUTHOR, authors);
                return true;
            }
        }
        return false;
    }

    bool removeGenre(int id) {
        for (int i = 0; i < (int)books.size(); i++) {
            const vector<int>& gids = books[i].getGenreIds();
            for (int j = 0; j < (int)gids.size(); j++)
                if (gids[j] == id) return false;
        }
        for (int i = 0; i < (int)genres.size(); i++) {
            if (genres[i].getId() == id) {
                genres.erase(genres.begin() + i);
                Genre::save(PATH_GENRE, genres);
                return true;
            }
        }
        return false;
    }

    bool removeBook(int id) {
        for (int i = 0; i < (int)borrows.size(); i++)
            if (borrows[i].getBookId() == id && !borrows[i].isReturned()) return false;
        for (int i = 0; i < (int)books.size(); i++) {
            if (books[i].getId() == id) {
                books.erase(books.begin() + i);
                Book::save(PATH_BOOK, books);
                return true;
            }
        }
        return false;
    }

    bool removeCustomer(int id) {
        for (int i = 0; i < (int)borrows.size(); i++)
            if (borrows[i].getCustomerId() == id && !borrows[i].isReturned()) return false;
        for (int i = 0; i < (int)customers.size(); i++) {
            if (customers[i].getId() == id) {
                customers.erase(customers.begin() + i);
                Customer::save(PATH_CUSTOMER, customers);
                return true;
            }
        }
        return false;
    }
};
