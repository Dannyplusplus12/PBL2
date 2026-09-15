#include <iostream>
#include <iomanip>
#include "Library.h"

Library lib;

// ====================================================================
// HIEN THI
// ====================================================================

void printSach(const Book& b) {
    Author* a = lib.findAuthor(b.authorId);
    string gnames;
    for (int gid : b.genreIds) {
        Genre* g = lib.findGenre(gid);
        if (g) { if (!gnames.empty()) gnames += ", "; gnames += g->name; }
    }
    cout << left
         << setw(5)  << b.id
         << setw(28) << b.name.substr(0, 26)
         << setw(20) << (a ? a->name.substr(0, 18) : "?")
         << setw(5)  << b.amount
         << setw(5)  << lib.available(b.id)
         << gnames   << "\n";
}

void printBorrow(const Borrow& bw) {
    Customer* c = lib.findCustomer(bw.customerId);
    Book*     b = lib.findBook(bw.bookId);
    cout << left
         << setw(5)  << bw.id
         << setw(20) << (c ? c->name.substr(0, 18) : "?")
         << setw(24) << (b ? b->name.substr(0, 22) : "?")
         << setw(12) << bw.borrowDate
         << setw(12) << bw.dueDate
         << (bw.returned ? "Da tra (" + bw.returnDate + ")" : "Dang muon")
         << "\n";
}

// ====================================================================
// TAC GIA
// ====================================================================
void menuTacGia() {
    while (true) {
        clr();
        header("TAC GIA");
        cout << "  [1] Danh sach  [2] Them  [3] Sua  [4] Xoa  [0] Quay lai\n";
        string ch = getStr("  Chon: ");
        clr();

        if (ch == "1") {
            header("DANH SACH TAC GIA");
            cout << left << setw(5) << "ID" << "Ten\n";
            cout << string(40, '-') << "\n";
            for (auto& a : lib.authors)
                cout << setw(5) << a.id << a.name << "\n";
            pause();
        }
        else if (ch == "2") {
            header("THEM TAC GIA");
            Author a;
            a.id   = nextId(lib.authors);
            a.name = getStr("  Ten: ");
            if (a.name.empty()) { cout << "  Ten khong duoc trong!\n"; pause(); continue; }
            lib.authors.push_back(a);
            Author::save(PATH_AUTHOR, lib.authors);
            cout << "  Da them tac gia #" << a.id << "\n";
            pause();
        }
        else if (ch == "3") {
            header("SUA TAC GIA");
            cout << left << setw(5) << "ID" << "Ten\n";
            for (auto& a : lib.authors) cout << setw(5) << a.id << a.name << "\n";
            int id = getInt("\n  ID can sua: ");
            Author* a = lib.findAuthor(id);
            if (!a) { cout << "  Khong tim thay!\n"; pause(); continue; }
            cout << "  Ten hien tai: " << a->name << "\n";
            string v = getStr("  Ten moi (bo trong = giu nguyen): ");
            if (!v.empty()) a->name = v;
            Author::save(PATH_AUTHOR, lib.authors);
            cout << "  Da cap nhat!\n";
            pause();
        }
        else if (ch == "4") {
            header("XOA TAC GIA");
            cout << left << setw(5) << "ID" << "Ten\n";
            for (auto& a : lib.authors) cout << setw(5) << a.id << a.name << "\n";
            int id = getInt("\n  ID can xoa: ");
            if (lib.removeAuthor(id)) cout << "  Da xoa!\n";
            else cout << "  Khong the xoa (dang duoc sach tham chieu)!\n";
            pause();
        }
        else if (ch == "0") break;
    }
}

// ====================================================================
// THE LOAI
// ====================================================================
void menuTheLoai() {
    while (true) {
        clr();
        header("THE LOAI");
        cout << "  [1] Danh sach  [2] Them  [3] Sua  [4] Xoa  [0] Quay lai\n";
        string ch = getStr("  Chon: ");
        clr();

        if (ch == "1") {
            header("DANH SACH THE LOAI");
            cout << left << setw(5) << "ID" << "Ten\n";
            cout << string(40, '-') << "\n";
            for (auto& g : lib.genres)
                cout << setw(5) << g.id << g.name << "\n";
            pause();
        }
        else if (ch == "2") {
            header("THEM THE LOAI");
            Genre g;
            g.id   = nextId(lib.genres);
            g.name = getStr("  Ten: ");
            if (g.name.empty()) { cout << "  Ten khong duoc trong!\n"; pause(); continue; }
            lib.genres.push_back(g);
            Genre::save(PATH_GENRE, lib.genres);
            cout << "  Da them the loai #" << g.id << "\n";
            pause();
        }
        else if (ch == "3") {
            header("SUA THE LOAI");
            cout << left << setw(5) << "ID" << "Ten\n";
            for (auto& g : lib.genres) cout << setw(5) << g.id << g.name << "\n";
            int id = getInt("\n  ID can sua: ");
            Genre* g = lib.findGenre(id);
            if (!g) { cout << "  Khong tim thay!\n"; pause(); continue; }
            cout << "  Ten hien tai: " << g->name << "\n";
            string v = getStr("  Ten moi (bo trong = giu nguyen): ");
            if (!v.empty()) g->name = v;
            Genre::save(PATH_GENRE, lib.genres);
            cout << "  Da cap nhat!\n";
            pause();
        }
        else if (ch == "4") {
            header("XOA THE LOAI");
            cout << left << setw(5) << "ID" << "Ten\n";
            for (auto& g : lib.genres) cout << setw(5) << g.id << g.name << "\n";
            int id = getInt("\n  ID can xoa: ");
            if (lib.removeGenre(id)) cout << "  Da xoa!\n";
            else cout << "  Khong the xoa (dang duoc sach su dung)!\n";
            pause();
        }
        else if (ch == "0") break;
    }
}

// ====================================================================
// KHACH HANG
// ====================================================================
void menuKhach() {
    while (true) {
        clr();
        header("KHACH HANG");
        cout << "  [1] Danh sach  [2] Them  [3] Sua  [4] Xoa  [0] Quay lai\n";
        string ch = getStr("  Chon: ");
        clr();

        if (ch == "1") {
            header("DANH SACH KHACH HANG");
            cout << left << setw(5) << "ID" << setw(22) << "Ho ten"
                 << setw(14) << "Dien thoai" << "Dia chi\n";
            cout << string(65, '-') << "\n";
            for (auto& c : lib.customers)
                cout << setw(5)  << c.id
                     << setw(22) << c.name.substr(0, 20)
                     << setw(14) << c.phone
                     << c.address << "\n";
            pause();
        }
        else if (ch == "2") {
            header("THEM KHACH HANG");
            Customer c;
            c.id      = nextId(lib.customers);
            c.name    = getStr("  Ho ten: ");
            c.phone   = getStr("  So dien thoai: ");
            c.address = getStr("  Dia chi: ");
            lib.customers.push_back(c);
            Customer::save(PATH_CUSTOMER, lib.customers);
            cout << "  Da them khach hang #" << c.id << "\n";
            pause();
        }
        else if (ch == "3") {
            header("SUA KHACH HANG");
            cout << left << setw(5) << "ID" << setw(22) << "Ho ten" << "SDT\n";
            for (auto& c : lib.customers)
                cout << setw(5) << c.id << setw(22) << c.name << c.phone << "\n";
            int id = getInt("\n  ID can sua: ");
            Customer* c = lib.findCustomer(id);
            if (!c) { cout << "  Khong tim thay!\n"; pause(); continue; }
            cout << "  [Ten: " << c->name << " | SDT: " << c->phone << " | DC: " << c->address << "]\n";
            string v;
            v = getStr("  Ten moi    (bo trong = giu nguyen): "); if (!v.empty()) c->name    = v;
            v = getStr("  SDT moi    (bo trong = giu nguyen): "); if (!v.empty()) c->phone   = v;
            v = getStr("  DC moi     (bo trong = giu nguyen): "); if (!v.empty()) c->address = v;
            Customer::save(PATH_CUSTOMER, lib.customers);
            cout << "  Da cap nhat!\n";
            pause();
        }
        else if (ch == "4") {
            header("XOA KHACH HANG");
            cout << left << setw(5) << "ID" << "Ho ten\n";
            for (auto& c : lib.customers) cout << setw(5) << c.id << c.name << "\n";
            int id = getInt("\n  ID can xoa: ");
            if (lib.removeCustomer(id)) cout << "  Da xoa!\n";
            else cout << "  Khong the xoa (khach dang co sach chua tra)!\n";
            pause();
        }
        else if (ch == "0") break;
    }
}

// ====================================================================
// SACH
// ====================================================================
void menuSach() {
    while (true) {
        clr();
        header("QUAN LY SACH");
        cout << "  [1] Danh sach  [2] Them  [3] Sua  [4] Xoa  [5] Tim kiem  [0] Quay lai\n";
        string ch = getStr("  Chon: ");
        clr();

        if (ch == "1") {
            header("DANH SACH SACH");
            cout << left << setw(5) << "ID" << setw(28) << "Ten sach"
                 << setw(20) << "Tac gia" << setw(5) << "SL" << setw(5) << "Con" << "The loai\n";
            cout << string(75, '-') << "\n";
            for (auto& b : lib.books) printSach(b);
            pause();
        }
        else if (ch == "2") {
            header("THEM SACH");
            cout << "  Tac gia:\n";
            for (auto& a : lib.authors) cout << "    " << a.id << ". " << a.name << "\n";
            cout << "  The loai:\n";
            for (size_t i = 0; i < lib.genres.size(); i++) {
                cout << "    " << lib.genres[i].id << ". " << lib.genres[i].name;
                cout << (i % 4 == 3 ? "\n" : "   ");
            }
            cout << "\n\n";
            Book b;
            b.id       = nextId(lib.books);
            b.name     = getStr("  Ten sach: ");
            b.authorId = getInt("  ID tac gia: ");
            b.amount   = getInt("  So luong: ");
            string gs  = getStr("  ID the loai (cach nhau dau cach, VD: 1 3 7): ");
            for (auto& s : split(gs, ' '))
                try { if (!s.empty()) b.genreIds.push_back(stoi(s)); } catch (...) {}
            lib.books.push_back(b);
            Book::save(PATH_BOOK, lib.books);
            cout << "  Da them sach #" << b.id << "\n";
            pause();
        }
        else if (ch == "3") {
            header("SUA SACH");
            cout << left << setw(5) << "ID" << setw(28) << "Ten" << "Tac gia\n";
            for (auto& b : lib.books) {
                Author* a = lib.findAuthor(b.authorId);
                cout << setw(5) << b.id << setw(28) << b.name.substr(0, 26)
                     << (a ? a->name : "?") << "\n";
            }
            int id = getInt("\n  ID can sua: ");
            Book* b = lib.findBook(id);
            if (!b) { cout << "  Khong tim thay!\n"; pause(); continue; }
            Author* a = lib.findAuthor(b->authorId);
            cout << "  [Ten: " << b->name << " | TG: " << (a ? a->name : "?")
                 << " | SL: " << b->amount << "]\n";
            string v;
            v = getStr("  Ten moi       (bo trong = giu nguyen): ");
            if (!v.empty()) b->name = v;
            v = getStr("  ID tac gia    (bo trong = giu nguyen): ");
            if (!v.empty()) try { b->authorId = stoi(v); } catch (...) {}
            v = getStr("  So luong      (bo trong = giu nguyen): ");
            if (!v.empty()) try { b->amount = stoi(v); } catch (...) {}
            v = getStr("  The loai moi  (VD: 1 3, bo trong = giu nguyen): ");
            if (!v.empty()) {
                b->genreIds.clear();
                for (auto& s : split(v, ' '))
                    try { if (!s.empty()) b->genreIds.push_back(stoi(s)); } catch (...) {}
            }
            Book::save(PATH_BOOK, lib.books);
            cout << "  Da cap nhat!\n";
            pause();
        }
        else if (ch == "4") {
            header("XOA SACH");
            cout << left << setw(5) << "ID" << setw(28) << "Ten" << "Tac gia\n";
            for (auto& b : lib.books) {
                Author* a = lib.findAuthor(b.authorId);
                cout << setw(5) << b.id << setw(28) << b.name.substr(0, 26)
                     << (a ? a->name : "?") << "\n";
            }
            int id = getInt("\n  ID can xoa: ");
            if (lib.removeBook(id)) cout << "  Da xoa!\n";
            else cout << "  Khong the xoa (dang co nguoi muon)!\n";
            pause();
        }
        else if (ch == "5") {
            header("TIM KIEM SACH");
            string kw = getStr("  Tu khoa: ");
            transform(kw.begin(), kw.end(), kw.begin(), ::tolower);
            cout << "\n" << left << setw(5) << "ID" << setw(28) << "Ten sach" << "Tac gia\n";
            cout << string(55, '-') << "\n";
            bool found = false;
            for (auto& b : lib.books) {
                string bn = b.name;
                transform(bn.begin(), bn.end(), bn.begin(), ::tolower);
                if (bn.find(kw) != string::npos) {
                    Author* a = lib.findAuthor(b.authorId);
                    cout << setw(5) << b.id << setw(28) << b.name.substr(0, 26)
                         << (a ? a->name : "?") << "\n";
                    found = true;
                }
            }
            if (!found) cout << "  Khong tim thay ket qua.\n";
            pause();
        }
        else if (ch == "0") break;
    }
}

// ====================================================================
// MUON / TRA SACH
// ====================================================================
void menuMuonTra() {
    while (true) {
        clr();
        header("MUON / TRA SACH");
        cout << "  [1] Dang muon  [2] Lich su  [3] Muon sach  [4] Tra sach  [0] Quay lai\n";
        string ch = getStr("  Chon: ");
        clr();

        if (ch == "1") {
            header("DANG MUON");
            cout << left << setw(5) << "ID" << setw(20) << "Khach hang"
                 << setw(24) << "Sach" << setw(12) << "Ngay muon" << "Han tra\n";
            cout << string(75, '-') << "\n";
            int count = 0;
            for (auto& bw : lib.borrows)
                if (!bw.returned) { printBorrow(bw); count++; }
            if (count == 0) cout << "  (Khong co phieu nao dang muon)\n";
            pause();
        }
        else if (ch == "2") {
            header("LICH SU MUON TRA");
            cout << left << setw(5) << "ID" << setw(20) << "Khach hang"
                 << setw(24) << "Sach" << setw(12) << "Ngay muon"
                 << setw(12) << "Han tra" << "Trang thai\n";
            cout << string(85, '-') << "\n";
            for (auto& bw : lib.borrows) printBorrow(bw);
            if (lib.borrows.empty()) cout << "  (Chua co lich su)\n";
            pause();
        }
        else if (ch == "3") {
            header("MUON SACH");
            cout << "  Sach co san:\n";
            cout << left << "  " << setw(5) << "ID" << setw(28) << "Ten sach"
                 << setw(20) << "Tac gia" << "Con lai\n";
            cout << "  " << string(60, '-') << "\n";
            for (auto& b : lib.books) {
                int av = lib.available(b.id);
                if (av > 0) {
                    Author* a = lib.findAuthor(b.authorId);
                    cout << "  " << setw(5) << b.id << setw(28) << b.name.substr(0, 26)
                         << setw(20) << (a ? a->name.substr(0, 18) : "?") << av << "\n";
                }
            }
            cout << "\n  Khach hang:\n";
            for (auto& c : lib.customers)
                cout << "  " << setw(5) << c.id << c.name << "\n";
            cout << "\n";
            int cid = getInt("  ID khach hang: ");
            int bid = getInt("  ID sach: ");
            string bd = getStr("  Ngay muon (YYYY-MM-DD): ");
            string dd = getStr("  Han tra   (YYYY-MM-DD): ");
            if (lib.borrow(cid, bid, bd, dd))
                cout << "  Muon thanh cong! Con lai: " << lib.available(bid) << " ban.\n";
            else
                cout << "  That bai (sach het luot hoac ID khong hop le)!\n";
            pause();
        }
        else if (ch == "4") {
            header("TRA SACH");
            cout << "  Phieu dang muon:\n";
            cout << "  " << left << setw(5) << "ID" << setw(20) << "Khach hang"
                 << setw(24) << "Sach" << "Han tra\n";
            cout << "  " << string(60, '-') << "\n";
            bool any = false;
            for (auto& bw : lib.borrows) {
                if (!bw.returned) {
                    Customer* c = lib.findCustomer(bw.customerId);
                    Book*     b = lib.findBook(bw.bookId);
                    cout << "  " << setw(5) << bw.id
                         << setw(20) << (c ? c->name.substr(0, 18) : "?")
                         << setw(24) << (b ? b->name.substr(0, 22) : "?")
                         << bw.dueDate << "\n";
                    any = true;
                }
            }
            if (!any) { cout << "  (Khong co phieu nao)\n"; pause(); continue; }
            cout << "\n";
            int bwId = getInt("  ID phieu muon: ");
            string rd = getStr("  Ngay tra (YYYY-MM-DD): ");
            if (lib.returnBook(bwId, rd))
                cout << "  Tra sach thanh cong!\n";
            else
                cout << "  That bai (phieu khong ton tai hoac da tra roi)!\n";
            pause();
        }
        else if (ch == "0") break;
    }
}

// ====================================================================
// MAIN
// ====================================================================
int main() {
    lib.load();

    while (true) {
        clr();

        // thong ke nhanh
        int active = 0;
        for (auto& bw : lib.borrows) if (!bw.returned) active++;

        cout << "\n";
        cout << "  +-----------------------------------------+\n";
        cout << "  |     HE THONG QUAN LY THU VIEN          |\n";
        cout << "  +-----------------------------------------+\n";
        cout << "  |  Sach: "      << left << setw(6)  << lib.books.size()
             <<   "  Khach: "      << setw(6)  << lib.customers.size()
             <<   "  Dang muon: "  << setw(4)  << active << "|\n";
        cout << "  +-----------------------------------------+\n\n";
        cout << "  [1] Sach           [2] Tac gia\n";
        cout << "  [3] The loai       [4] Khach hang\n";
        cout << "  [5] Muon / Tra     [0] Thoat\n\n";
        string ch = getStr("  Chon: ");

        if      (ch == "1") menuSach();
        else if (ch == "2") menuTacGia();
        else if (ch == "3") menuTheLoai();
        else if (ch == "4") menuKhach();
        else if (ch == "5") menuMuonTra();
        else if (ch == "0") { cout << "\n  Tam biet!\n"; break; }
    }

    return 0;
}
