#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <random>
#include <ctime>
#include <algorithm>

using namespace std;

const int RECORD_COUNT = 20;

mt19937 rng(random_device{}());

int randInt(int lo, int hi) {
    uniform_int_distribution<int> dist(lo, hi);
    return dist(rng);
}

string pick(const vector<string>& pool) {
    return pool[randInt(0, (int)pool.size() - 1)];
}

// ---------------- Data pools ----------------
const vector<string> AUTHOR_NAMES = {
    "Nguyen Nhat Anh", "To Hoai", "Nam Cao", "Vu Trong Phung", "Ngo Tat To",
    "Nguyen Du", "Xuan Dieu", "Han Mac Tu", "Nguyen Huy Thiep", "Bao Ninh",
    "Ho Anh Thai", "Nguyen Ngoc Tu", "Duong Thuy", "Nguyen Nhat Anh Jr",
    "Le Luu", "Ma Van Khang", "Nguyen Khai", "To Huu", "Che Lan Vien", "Huy Can"
};

const vector<string> GENRE_NAMES = {
    "Kinh Di", "Tinh Cam", "Hai Huoc", "Trinh Tham", "Khoa Hoc Vien Tuong",
    "Lich Su", "Tam Ly", "Phieu Luu", "Kinh Dien", "Ngon Tinh",
    "Truyen Tranh", "Thieu Nhi", "Kinh Te", "Tam Linh", "Chinh Tri",
    "The Thao", "Du Lich", "Am Nhac", "Nghe Thuat", "Giao Duc"
};

const vector<string> BOOK_TITLES = {
    "Toi Thay Hoa Vang Tren Co Xanh", "Dat Rung Phuong Nam", "So Do",
    "Tat Den", "Truyen Kieu", "Vo Nhat", "Chi Pheo", "Nhat Ky Trong Tu",
    "Nhung Nguoi Khon Kho", "Doc Ta Ky Hiep", "Mat Biec", "Cho Toi Xin Mot Ve Di Tuoi Tho",
    "Song Xa Me", "Nhung Ngoi Sao Xa Xoi", "Vang Trang Va Nhung Vi Sao",
    "Ben Kia Song", "Mua Thu Vang", "Mien Dat Hua", "Con Duong Trai Nghiem", "Giac Mo My"
};

const vector<string> BOOK_DESCRIPTIONS = {
    "Cau chuyen ve tuoi tho", "Tieu thuyet kinh dien", "Hoi ky day cam xuc",
    "Cau chuyen tinh yeu", "Tac pham noi tieng", "Truyen ngan chon loc",
    "Ky su phieu luu", "Tac pham van hoc Viet Nam", ""
};

const vector<string> CUSTOMER_NAMES = {
    "Tran Van An", "Le Thi Binh", "Pham Van Cuong", "Hoang Thi Dung",
    "Vu Van Em", "Do Thi Phuong", "Bui Van Giang", "Dang Thi Hoa",
    "Ngo Van Inh", "Duong Thi Kim", "Ly Van Long", "Trinh Thi Mai",
    "Phan Van Nam", "Ta Thi Oanh", "Chu Van Phu", "Vuong Thi Quyen",
    "Luu Van Son", "Mai Thi Thu", "Dinh Van Uy", "Cao Thi Van"
};

const vector<string> ADDRESSES = {
    "18 Nguyen Hoang, Ha Noi", "22 Le Loi, Da Nang", "5 Tran Phu, Hue",
    "100 Nguyen Trai, Ho Chi Minh", "45 Hai Ba Trung, Ha Noi",
    "12 Vo Van Kiet, Ho Chi Minh", "77 Bach Dang, Da Nang",
    "9 Ly Thuong Kiet, Ha Noi", "33 Nguyen Van Linh, Da Nang",
    "60 Dien Bien Phu, Ho Chi Minh"
};

// ---------------- Date helpers ----------------
struct Date { int y, m, d; };

Date randomDateIn2026() {
    int month = randInt(1, 8);
    int maxDay = (month == 2) ? 28 : 30;
    int day = randInt(1, maxDay);
    return { 2026, month, day };
}

string dateToStr(const Date& date) {
    char buf[16];
    snprintf(buf, sizeof(buf), "%04d-%02d-%02d", date.y, date.m, date.d);
    return string(buf);
}

Date addDays(const Date& date, int days) {
    tm t{};
    t.tm_year = date.y - 1900;
    t.tm_mon = date.m - 1;
    t.tm_mday = date.d;
    t.tm_hour = 12;
    time_t asTime = mktime(&t);
    asTime += days * 24 * 60 * 60;
    tm* result = localtime(&asTime);
    return { result->tm_year + 1900, result->tm_mon + 1, result->tm_mday };
}

// ---------------- Generators ----------------
void genAuthor() {
    ofstream out("Author.dat");
    for (int i = 1; i <= RECORD_COUNT; ++i) {
        out << i << "|" << AUTHOR_NAMES[i - 1] << "\n";
    }
}

void genGenre() {
    ofstream out("Genre.dat");
    for (int i = 1; i <= RECORD_COUNT; ++i) {
        out << i << "|" << GENRE_NAMES[i - 1] << "\n";
    }
}

void genCustomer() {
    ofstream out("Customer.dat");
    for (int i = 1; i <= RECORD_COUNT; ++i) {
        string des = (randInt(0, 3) == 0) ? "Khach quen" : "";
        string phone = "0" + to_string(randInt(300000000, 999999999));
        out << i << "|" << CUSTOMER_NAMES[i - 1] << "|" << des << "|"
            << pick(ADDRESSES) << "|" << phone << "\n";
    }
}

void genBook() {
    ofstream out("Book.dat");
    for (int i = 1; i <= RECORD_COUNT; ++i) {
        int authorId = randInt(1, RECORD_COUNT);
        int amount = randInt(1, 50);

        vector<int> genreIds;
        int genreCount = randInt(1, 3);
        while ((int)genreIds.size() < genreCount) {
            int g = randInt(1, RECORD_COUNT);
            if (find(genreIds.begin(), genreIds.end(), g) == genreIds.end()) {
                genreIds.push_back(g);
            }
        }

        out << i << "|" << BOOK_TITLES[i - 1] << "|" << pick(BOOK_DESCRIPTIONS)
            << "|" << authorId << "|" << amount << "|";
        for (size_t j = 0; j < genreIds.size(); ++j) {
            if (j > 0) out << " ";
            out << genreIds[j];
        }
        out << "\n";
    }
}

void genBorrow() {
    ofstream out("Borrow.dat");
    for (int i = 1; i <= RECORD_COUNT; ++i) {
        int customerId = randInt(1, RECORD_COUNT);
        int bookId = randInt(1, RECORD_COUNT);

        Date borrowDate = randomDateIn2026();
        Date dueDate = addDays(borrowDate, 14);

        bool returned = randInt(0, 1) == 1;
        string returnDateStr = "";
        string status = "dang_muon";
        if (returned) {
            Date returnDate = addDays(borrowDate, randInt(1, 20));
            returnDateStr = dateToStr(returnDate);
            status = "da_tra";
        }

        out << i << "|" << customerId << "|" << bookId << "|"
            << dateToStr(borrowDate) << "|" << dateToStr(dueDate) << "|"
            << returnDateStr << "|" << status << "\n";
    }
}

int main() {
    genAuthor();
    genGenre();
    genCustomer();
    genBook();
    genBorrow();
    cout << "Da sinh xong 5 file .dat, moi file " << RECORD_COUNT << " dong.\n";
    return 0;
}
