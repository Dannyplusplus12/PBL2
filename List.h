#pragma once

// Mang dong tu viet (thay cho std::vector)
// Yeu cau kieu T: co constructor mac dinh va phep gan (=)
template<typename T>
class List {
private:
    T*  data;
    int count;   // so phan tu hien co
    int cap;     // suc chua hien tai

    // tang suc chua gap doi
    void grow() {
        int newCap = (cap == 0) ? 4 : cap * 2;
        T* nd = new T[newCap];
        for (int i = 0; i < count; i++) nd[i] = data[i];
        delete[] data;
        data = nd;
        cap  = newCap;
    }

    void copyFrom(const List& o) {
        count = o.count;
        cap   = o.count;
        data  = (cap > 0) ? new T[cap] : nullptr;
        for (int i = 0; i < count; i++) data[i] = o.data[i];
    }

public:
    List() : data(nullptr), count(0), cap(0) {}
    List(const List& o) { copyFrom(o); }
    ~List() { delete[] data; }

    List& operator=(const List& o) {
        if (this != &o) {
            delete[] data;
            copyFrom(o);
        }
        return *this;
    }

    int  size()  const { return count; }
    bool empty() const { return count == 0; }

    T&       operator[](int i)       { return data[i]; }
    const T& operator[](int i) const { return data[i]; }

    void push_back(const T& x) {
        if (count == cap) {
            T copy = x;          // x co the nam trong chinh mang cu
            grow();
            data[count++] = copy;
        } else {
            data[count++] = x;
        }
    }

    // xoa phan tu o vi tri index, don cac phan tu phia sau len
    void erase(int index) {
        if (index < 0 || index >= count) return;
        for (int j = index; j < count - 1; j++) data[j] = data[j + 1];
        count--;
    }

    void clear() { count = 0; }
};
