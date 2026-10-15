#pragma once
#include <string>
#include <iostream>
#include <cstdlib>
#include <cctype>
#include "List.h"
using namespace std;

// tach chuoi theo ky tu phan cach
// (khong tao phan tu rong o cuoi: "a|b|" -> {"a","b"})
inline List<string> split(const string& s, char d) {
    List<string> v;
    string t;
    for (int i = 0; i < (int)s.size(); i++) {
        if (s[i] == d) {
            v.push_back(t);
            t = "";
        } else {
            t += s[i];
        }
    }
    if (!t.empty()) v.push_back(t);
    return v;
}

// id ke tiep (max + 1), yeu cau T co phuong thuc getId()
template<typename T>
int nextId(const List<T>& v) {
    int mx = 0;
    for (int i = 0; i < (int)v.size(); i++)
        if (v[i].getId() > mx) mx = v[i].getId();
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

//bỏ khoảng trắng
inline string trim(const string& s) {
    size_t a = s.find_first_not_of(" \t\r\n");
    if (a == string::npos) return "";
    size_t b = s.find_last_not_of(" \t\r\n");
    return s.substr(a, b - a + 1);
}

//chuẩn hóa tên
inline string normalize(const string& s) {
    string t = trim(s), r;
    bool prevSpace = false;
    for (size_t i = 0; i < t.size(); i++) {
        if (t[i] == ' ') {
            if (!prevSpace) r += ' ';
            prevSpace = true;
        } else {
            r += (char)tolower((unsigned char)t[i]);
            prevSpace = false;
        }
    }
    return r;
}
