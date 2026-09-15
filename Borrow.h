#pragma once
#include "utils.h"
#include <fstream>

// format: id|customerId|bookId|borrowDate|dueDate|returnDate|returned(0/1)
class Borrow {
private:
    int    id;
    int    customerId;
    int    bookId;
    string borrowDate;
    string dueDate;
    string returnDate;   // trong neu chua tra
    bool   returned;

public:
    // --- constructor ---
    Borrow() : id(0), customerId(0), bookId(0), returned(false) {}

    // --- getter ---
    int           getId()         const { return id;         }
    int           getCustomerId() const { return customerId; }
    int           getBookId()     const { return bookId;     }
    const string& getBorrowDate() const { return borrowDate; }
    const string& getDueDate()    const { return dueDate;    }
    const string& getReturnDate() const { return returnDate; }
    bool          isReturned()    const { return returned;   }

    // --- setter ---
    void setId(int i)                   { id         = i; }
    void setCustomerId(int cid)         { customerId = cid; }
    void setBookId(int bid)             { bookId     = bid; }
    void setBorrowDate(const string& d) { borrowDate = d; }
    void setDueDate(const string& d)    { dueDate    = d; }
    void setReturnDate(const string& d) { returnDate = d; }
    void setReturned(bool r)            { returned   = r; }

    // --- serialize ---
    string toLine() const {
        return to_string(id)         + "|"
             + to_string(customerId) + "|"
             + to_string(bookId)     + "|"
             + borrowDate            + "|"
             + dueDate               + "|"
             + returnDate            + "|"
             + (returned ? "1" : "0");
    }

    static Borrow fromLine(const string& line) {
        vector<string> f = split(line, '|');
        Borrow b;
        b.id         = stoi(f[0]);
        b.customerId = stoi(f[1]);
        b.bookId     = stoi(f[2]);
        b.borrowDate = f[3];
        b.dueDate    = f[4];
        b.returnDate = f.size() > 5 ? f[5] : "";
        b.returned   = f.size() > 6 && f[6] == "1";
        return b;
    }

    // --- file ---
    static vector<Borrow> load(const string& path) {
        vector<Borrow> v;
        ifstream f(path);
        string line;
        while (getline(f, line))
            if (!line.empty()) v.push_back(fromLine(line));
        return v;
    }

    static void save(const string& path, const vector<Borrow>& v) {
        ofstream f(path);
        for (int i = 0; i < (int)v.size(); i++)
            f << v[i].toLine() << "\n";
    }
};
