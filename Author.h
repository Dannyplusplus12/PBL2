#pragma once
#include "utils.h"
#include <fstream>

// format: id|name
struct Author {
    int    id   = 0;
    string name;

    string toLine() const {
        return to_string(id) + "|" + name;
    }

    static Author fromLine(const string& line) {
        auto f = split(line, '|');
        Author a;
        a.id   = stoi(f[0]);
        a.name = f[1];
        return a;
    }

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
        for (auto& a : v) f << a.toLine() << "\n";
    }
};
