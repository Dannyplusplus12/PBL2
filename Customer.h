#pragma once
#include "utils.h"
#include <fstream>

// format: id|name|phone|address
struct Customer {
    int    id      = 0;
    string name;
    string phone;
    string address;

    string toLine() const {
        return to_string(id) + "|" + name + "|" + phone + "|" + address;
    }

    static Customer fromLine(const string& line) {
        auto f = split(line, '|');
        Customer c;
        c.id      = stoi(f[0]);
        c.name    = f[1];
        c.phone   = f.size() > 2 ? f[2] : "";
        c.address = f.size() > 3 ? f[3] : "";
        return c;
    }

    static vector<Customer> load(const string& path) {
        vector<Customer> v;
        ifstream f(path);
        string line;
        while (getline(f, line))
            if (!line.empty()) v.push_back(fromLine(line));
        return v;
    }

    static void save(const string& path, const vector<Customer>& v) {
        ofstream f(path);
        for (auto& c : v) f << c.toLine() << "\n";
    }
};
