#pragma once
#include "Entity.h"
#include <fstream>

// format: id|name
class Author : public Entity {
public:
    Author() {}
    Author(int id, const string& name) : Entity(id, name) {}

    static Author fromLine(const string& line) {
        vector<string> f = split(line, '|');
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
        for (int i = 0; i < (int)v.size(); i++)
            f << v[i].toLine() << "\n";
    }
};
