#pragma once
#include "Entity.h"
#include <fstream>

// format: id|name|phone|address|lock
class Customer : public Entity {
private:
    string phone;
    string address;
    bool lock;

public:
    Customer(): lock(false) {}
    Customer(int id, const string& name, const string& phone, const string& address)
        : Entity(id, name), phone(phone), address(address), lock(false) {}

    const string& getPhone()   const { return phone;   }
    const string& getAddress() const { return address; }
    bool          isLocked()   const { return lock;    }

    void setPhone(const string& p)   { phone   = p; }
    void setAddress(const string& a) { address = a; }
    void setLock(bool l)             { lock = l;    }

    string toLine() const override {
        return Entity::toLine() + "|" + phone + "|" + address + "|" + (lock ? "1" : "0");
    }

    static Customer fromLine(const string& line) {
        vector<string> f = split(line, '|');
        Customer c;
        c.id      = stoi(f[0]);
        c.name    = f[1];
        c.phone   = f.size() > 2 ? f[2] : "";
        c.address = f.size() > 3 ? f[3] : "";
        c.lock = f.size() > 4 ? stoi(f[4]) : 0;
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
