#pragma once
#include "Entity.h"
#include <fstream>

// format: id|name|phone|address
class Customer : public Entity {
private:
    string phone;
    string address;

public:
    Customer() {}
    Customer(int id, const string& name, const string& phone, const string& address)
        : Entity(id, name), phone(phone), address(address) {}

    const string& getPhone()   const { return phone;   }
    const string& getAddress() const { return address; }

    void setPhone(const string& p)   { phone   = p; }
    void setAddress(const string& a) { address = a; }

    string toLine() const override {
        return Entity::toLine() + "|" + phone + "|" + address;
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
