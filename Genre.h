#pragma once
#include "utils.h"
#include <fstream>

// format: id|name
struct Genre {
    int    id   = 0;
    string name;

    string toLine() const {
        return to_string(id) + "|" + name;
    }

    static Genre fromLine(const string& line) {
        auto f = split(line, '|');
        Genre g;
        g.id   = stoi(f[0]);
        g.name = f[1];
        return g;
    }

    static vector<Genre> load(const string& path) {
        vector<Genre> v;
        ifstream f(path);
        string line;
        while (getline(f, line))
            if (!line.empty()) v.push_back(fromLine(line));
        return v;
    }

    static void save(const string& path, const vector<Genre>& v) {
        ofstream f(path);
        for (auto& g : v) f << g.toLine() << "\n";
    }
};
