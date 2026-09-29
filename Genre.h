#pragma once
#include "Entity.h"
#include <fstream>

// format: id|name
class Genre : public Entity {
public:
    Genre() {}
    Genre(int id, const string& name) : Entity(id, name) {}

    static Genre fromLine(const string& line) {
        vector<string> f = split(line, '|');
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
        for (int i = 0; i < (int)v.size(); i++)
            f << v[i].toLine() << "\n";
    }
};
