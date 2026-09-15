#pragma once
#include "utils.h"
#include <fstream>

// format: id|customerId|bookId|borrowDate|dueDate|returnDate|returned(0/1)
struct Borrow {
    int    id         = 0;
    int    customerId = 0;
    int    bookId     = 0;
    string borrowDate;
    string dueDate;
    string returnDate;   // trong neu chua tra
    bool   returned   = false;

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
        auto f = split(line, '|');
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
        for (auto& b : v) f << b.toLine() << "\n";
    }
};
