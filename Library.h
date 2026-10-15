#pragma once
#include "Author.h"
#include "Genre.h"
#include "Customer.h"
#include "Book.h"
#include "Borrow.h"
#include "DateUtils.h"

using namespace DateUtils;

// duong dan file du lieu
const string PATH_AUTHOR   = "Data/Author.dat";
const string PATH_GENRE    = "Data/Genre.dat";
const string PATH_CUSTOMER = "Data/Customer.dat";
const string PATH_BOOK     = "Data/Book.dat";
const string PATH_BORROW   = "Data/Borrow.dat";

class Library {
public:
    List<Author>   authors;
    List<Genre>    genres;
    List<Customer> customers;
    List<Book>     books;
    List<Borrow>   borrows;

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

    // kiểm tra số lượng sách / 1 khách đã mượn
    bool canBorrow (int customerId) {
        int cnt = 0;
        for (int i = 0; i < (int)borrows.size(); i++) {
            if (borrows[i].getCustomerId() == customerId && !borrows[i].isReturned())
                cnt++;
        }
        return cnt < 5;
    }

    // khóa customer 
    bool lockCustomer (int customerId) {
        Customer* c = findCustomer(customerId);
        if (!c) return false;
        c->setLock(true);
        Customer::save(PATH_CUSTOMER, customers);
        return true; 
    }

    // mở khóa customer
    bool unlockCustomer (int customerId) {
        Customer* c = findCustomer(customerId);
        if (!c) return false;
        c->setLock(false);
        Customer::save(PATH_CUSTOMER, customers);
        return true; 
    }

    bool lockOverdue(int customerId) {
        Customer* customer = findCustomer(customerId);
        if (!customer)
            return false;

        for (int i = 0; i < (int)borrows.size(); i++)
        {
            if (borrows[i].getCustomerId() == customerId && !borrows[i].isReturned()
                && overdueDays(borrows[i].getDueDate()) > 5)
            {
                customer->setLock(true);
                Customer::save(PATH_CUSTOMER, customers);
                return true;
            }
        }
        return false;
    }

    // muon sach
    bool borrow(int customerId, int bookId, int daysBorrowed) {
        Customer* c = findCustomer(customerId);
        if (!c || c->isLocked()) return false;
        if (!findBook(bookId)) return false;
        if (available(bookId) <= 0) return false;
        if (daysBorrowed <= 0 || daysBorrowed > 60) return false;
        if (!canBorrow(customerId)) return false;

        Borrow bw;
        bw.setId(nextId(borrows));
        bw.setCustomerId(customerId);
        bw.setBookId(bookId);
        const string borrowDate = today();
        bw.setBorrowDate(borrowDate);
        bw.setDueDate(addDays(borrowDate, daysBorrowed));
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

    // gia hạn mượn
    bool Renew (int borrowId, int extraDay) {
        Borrow* b = findBorrow(borrowId);
        if (!b || b->isReturned()) return false;
        if (extraDay <= 0) return false;
        if (b->getRenewCount() >= 1) return false;

        Customer* c = findCustomer(b->getCustomerId());
        if (!c || c->isLocked()) return false;
        if (overdueDays(b->getDueDate()) > 5) return false;

        b->setDueDate(addDays(b->getDueDate(), extraDay));
        b->setRenewCount(b->getRenewCount() + 1);
        Borrow::save(PATH_BORROW, borrows);
        return true;
    }

    // xoa co kiem tra rang buoc
    bool removeAuthor(int id) {
        for (int i = 0; i < (int)books.size(); i++)
            if (books[i].getAuthorId() == id) return false;
        for (int i = 0; i < (int)authors.size(); i++) {
            if (authors[i].getId() == id) {
                authors.erase(i);
                Author::save(PATH_AUTHOR, authors);
                return true;
            }
        }
        return false;
    }

    bool removeGenre(int id) {
        for (int i = 0; i < (int)books.size(); i++) {
            const List<int>& gids = books[i].getGenreIds();
            for (int j = 0; j < (int)gids.size(); j++)
                if (gids[j] == id) return false;
        }
        for (int i = 0; i < (int)genres.size(); i++) {
            if (genres[i].getId() == id) {
                genres.erase(i);
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
                books.erase(i);
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
                customers.erase(i);
                Customer::save(PATH_CUSTOMER, customers);
                return true;
            }
        }
        return false;
    }

    //kiểm tra đã tồn tại chưa khi add mới
    bool authorExists(const string& name, int excludeId = 0) {
        string n = normalize(name);
        for (int i = 0; i < (int)authors.size(); i++)
            if (authors[i].getId() != excludeId && normalize(authors[i].getName()) == n)
                return true;
        return false;
    }

    bool genreExists(const string& name, int excludeId = 0) {
        string n = normalize(name);
        for (int i = 0; i < (int)genres.size(); i++)
            if (genres[i].getId() != excludeId && normalize(genres[i].getName()) == n)
                return true;
        return false;
    }

    bool bookExists(const string& name, int authorId, int excludeId = 0) {
        string n = normalize(name);
        for (int i = 0; i < (int)books.size(); i++)
            if (books[i].getId() != excludeId && books[i].getAuthorId() == authorId && normalize(books[i].getName()) == n)
                return true;
        return false;
    }
};
