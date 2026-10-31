#pragma once
#include "Entity.h"
#include <fstream>

// format: id|name|authorId|amount|g1 g2 g3
class Book : public Entity {
private:
    int         authorId;
    int         amount;
    List<int> genreIds;

public:
    Book() : authorId(0), amount(0) {}
    Book(int id, const string& name, int authorId, int amount, const List<int>& genreIds)
        : Entity(id, name), authorId(authorId), amount(amount), genreIds(genreIds) {}

    int                getAuthorId() const { return authorId; }
    int                getAmount()   const { return amount;   }
    const List<int>& getGenreIds() const { return genreIds; }

    void setAuthorId(int aid)              { authorId = aid; }
    void setAmount(int a)                  { amount   = a;   }
    void setGenreIds(const List<int>& v) { genreIds = v;   }

    string toLine() const override {
        string s = to_string(id) + "|" + name + "|" + to_string(authorId) + "|" + to_string(amount) + "|";
        for (int i = 0; i < (int)genreIds.size(); i++) {
            if (i) s += " ";
            s += to_string(genreIds[i]);
        }
        return s;
    }

    static Book fromLine(const string& line) {
        List<string> f = split(line, '|');
        Book b;
        b.id       = stoi(f[0]);
        b.name     = f[1];
        b.authorId = stoi(f[2]);
        b.amount   = stoi(f[3]);
        if (f.size() > 4) {
            List<string> gs = split(f[4], ' ');
            for (int i = 0; i < (int)gs.size(); i++)
                if (!gs[i].empty()) b.genreIds.push_back(stoi(gs[i]));
        }
        return b;
    }

    static List<Book> load(const string& path) {
        List<Book> v;
        ifstream f(path);
        string line;
        while (getline(f, line))
            if (!line.empty()) v.push_back(fromLine(line));
        return v;
    }

    static void save(const string& path, const List<Book>& v) {
        ofstream f(path);
        for (int i = 0; i < (int)v.size(); i++)
            f << v[i].toLine() << "\n";
    }
};
