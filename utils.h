#pragma once
#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <algorithm>
using namespace std;

// tach chuoi theo ky tu phan cach
inline vector<string> split(const string& s, char d) {
    vector<string> v;
    stringstream ss(s);
    string t;
    while (getline(ss, t, d)) v.push_back(t);
    return v;
}

// id ke tiep (max + 1), yeu cau T co truong .id
template<typename T>
int nextId(const vector<T>& v) {
    int mx = 0;
    for (auto& x : v) if (x.id > mx) mx = x.id;
    return mx + 1;
}

// nhap chuoi
inline string getStr(const string& p) {
    cout << p;
    string s;
    getline(cin, s);
    return s;
}

// nhap so nguyen, lap lai neu sai
inline int getInt(const string& p) {
    while (true) {
        string s = getStr(p);
        try { if (!s.empty()) return stoi(s); }
        catch (...) {}
        cout << "  Vui long nhap so nguyen!\n";
    }
}

// xoa man hinh (Windows)
inline void clr() { system("cls"); }

// cho Enter
inline void pause() {
    cout << "\n  [Enter de tiep tuc...]";
    string d; getline(cin, d);
}

// in tieu de section
inline void header(const string& t) {
    cout << "\n=== " << t << " ===\n";
}
