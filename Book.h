#pragma once
#include "utils.h"
#include <fstream>

// format: id|name|authorId|amount|g1 g2 g3
class Book {
private:
    int         id;
    string      name;
    int         authorId;
    int         amount;
    vector<int> genreIds;

public:
    // --- constructor ---
    Book() : id(0), authorId(0), amount(0) {}
    Book(int id, const string& name, int authorId, int amount, const vector<int>& genreIds)
        : id(id), name(name), authorId(authorId), amount(amount), genreIds(genreIds) {}

    // --- getter ---
    int                getId()       const { return id;       }
    const string&      getName()     const { return name;     }
    int                getAuthorId() const { return authorId; }
    int                getAmount()   const { return amount;   }
    const vector<int>& getGenreIds() const { return genreIds; }

    // --- setter ---
    void setId(int i)                      { id       = i; }
    void setName(const string& n)          { name     = n; }
    void setAuthorId(int aid)              { authorId = aid; }
    void setAmount(int a)                  { amount   = a; }
    void setGenreIds(const vector<int>& v) { genreIds = v; }

    // --- serialize ---
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
        vector<string> f = split(line, '|');
        Book b;
        b.id       = stoi(f[0]);
        b.name     = f[1];
        b.authorId = stoi(f[2]);
        b.amount   = stoi(f[3]);
        if (f.size() > 4) {
            vector<string> gs = split(f[4], ' ');
            for (int i = 0; i < (int)gs.size(); i++)
                if (!gs[i].empty()) b.genreIds.push_back(stoi(gs[i]));
        }
        return b;
    }

    // --- file ---
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
        for (int i = 0; i < (int)v.size(); i++)
            f << v[i].toLine() << "\n";
    }
};
