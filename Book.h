#pragma once
#include "utils.h"
#include <fstream>

// format: id|name|authorId|amount|g1 g2 g3
struct Book {
    int         id       = 0;
    string      name;
    int         authorId = 0;
    int         amount   = 0;
    vector<int> genreIds;

    string toLine() const {
        stringstream ss;
        ss << id << "|" << name << "|" << authorId << "|" << amount << "|";
        for (size_t i = 0; i < genreIds.size(); i++) {
            if (i) ss << " ";
            ss << genreIds[i];
        }
        return ss.str();
    }

    static Book fromLine(const string& line) {
        auto f = split(line, '|');
        Book b;
        b.id       = stoi(f[0]);
        b.name     = f[1];
        b.authorId = stoi(f[2]);
        b.amount   = stoi(f[3]);
        if (f.size() > 4)
            for (auto& s : split(f[4], ' '))
                if (!s.empty()) b.genreIds.push_back(stoi(s));
        return b;
    }

    static vector<Book> load(const string& path) {
        vector<Book> v;
        ifstream f(path);
        string line;
        while (getline(f, line))
            if (!line.empty()) v.push_back(fromLine(line));
        return v;
    }

    static void save(const string& path, const vector<Book>& v) {
        ofstream f(path);
        for (auto& b : v) f << b.toLine() << "\n";
    }
};
