#pragma once
#include "utils.h"
#include <fstream>

// format: id|name
class Author {
private:
    int    id;
    string name;

public:
    // --- constructor ---
    Author() : id(0) {}
    Author(int id, const string& name) : id(id), name(name) {}

    // --- getter ---
    int           getId()   const { return id;   }
    const string& getName() const { return name; }

    // --- setter ---
    void setId(int i)             { id   = i; }
    void setName(const string& n) { name = n; }

    // --- serialize ---
    string toLine() const { return to_string(id) + "|" + name; }

    static Author fromLine(const string& line) {
        vector<string> f = split(line, '|');
        Author a;
        a.id   = stoi(f[0]);
        a.name = f[1];
        return a;
    }

    // --- file ---
    static vector<Author> load(const string& path) {
        vector<Author> v;
        ifstream f(path);
        string line;
        while (getline(f, line))
            if (!line.empty()) v.push_back(fromLine(line));
        return v;
    }

    static void save(const string& path, const vector<Author>& v) {
        ofstream f(path);
        for (int i = 0; i < (int)v.size(); i++)
            f << v[i].toLine() << "\n";
    }
};
