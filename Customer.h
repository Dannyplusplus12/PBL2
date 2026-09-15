#pragma once
#include "utils.h"
#include <fstream>

// format: id|name|phone|address
class Customer {
private:
    int    id;
    string name;
    string phone;
    string address;

public:
    // --- constructor ---
    Customer() : id(0) {}
    Customer(int id, const string& name, const string& phone, const string& address)
        : id(id), name(name), phone(phone), address(address) {}

    // --- getter ---
    int           getId()      const { return id;      }
    const string& getName()    const { return name;    }
    const string& getPhone()   const { return phone;   }
    const string& getAddress() const { return address; }

    // --- setter ---
    void setId(int i)              { id      = i; }
    void setName(const string& n)  { name    = n; }
    void setPhone(const string& p) { phone   = p; }
    void setAddress(const string& a){ address = a; }

    // --- serialize ---
    string toLine() const {
        return to_string(id) + "|" + name + "|" + phone + "|" + address;
    }

    static Customer fromLine(const string& line) {
        vector<string> f = split(line, '|');
        Customer c;
        c.id      = stoi(f[0]);
        c.name    = f[1];
        c.phone   = f.size() > 2 ? f[2] : "";
        c.address = f.size() > 3 ? f[3] : "";
        return c;
    }

    // --- file ---
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
        for (int i = 0; i < (int)v.size(); i++)
            f << v[i].toLine() << "\n";
    }
};
