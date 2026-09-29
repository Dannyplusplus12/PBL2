#pragma once
#include "utils.h"

// lop co so cho cac doi tuong co id + ten
class Entity {
protected:
    int    id;
    string name;

public:
    Entity() : id(0) {}
    Entity(int id, const string& name) : id(id), name(name) {}
    virtual ~Entity() {}

    int           getId()   const { return id;   }
    const string& getName() const { return name; }

    void setId(int i)             { id   = i; }
    void setName(const string& n) { name = n; }

    // dinh dang chung id|name
    virtual string toLine() const { return to_string(id) + "|" + name; }
};
