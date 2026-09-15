#include <iostream>
#include <iomanip>
#include "Library.h"

Library lib;

// ====================================================================
// HIEN THI
// ====================================================================

void printSach(const Book& b) {
    Author* a = lib.findAuthor(b.getAuthorId());
    string gnames;
    const vector<int>& gids = b.getGenreIds();
    for (int i = 0; i < (int)gids.size(); i++) {
        Genre* g = lib.findGenre(gids[i]);
        if (g) { if (!gnames.empty()) gnames += ", "; gnames += g->getName(); }
    }
    cout << left
         << setw(5)  << b.getId()
         << setw(28) << b.getName().substr(0, 26)
         << setw(20) << (a ? a->getName().substr(0, 18) : "?")
         << setw(5)  << b.getAmount()
         << setw(5)  << lib.available(b.getId())
         << gnames   << "\n";
}

void printBorrow(const Borrow& bw) {
    Customer* c = lib.findCustomer(bw.getCustomerId());
    Book*     b = lib.findBook(bw.getBookId());
    cout << left
         << setw(5)  << bw.getId()
         << setw(20) << (c ? c->getName().substr(0, 18) : "?")
         << setw(24) << (b ? b->getName().substr(0, 22) : "?")
         << setw(12) << bw.getBorrowDate()
         << setw(12) << bw.getDueDate()
         << (bw.isReturned() ? "Da tra (" + bw.getReturnDate() + ")" : "Dang muon")
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
            for (int i = 0; i < (int)lib.authors.size(); i++)
                cout << setw(5) << lib.authors[i].getId() << lib.authors[i].getName() << "\n";
            pause();
        }
        else if (ch == "2") {
            header("THEM TAC GIA");
            Author a;
            a.setId(nextId(lib.authors));
            string n = getStr("  Ten: ");
            if (n.empty()) { cout << "  Ten khong duoc trong!\n"; pause(); continue; }
            a.setName(n);
            lib.authors.push_back(a);
            Author::save(PATH_AUTHOR, lib.authors);
            cout << "  Da them tac gia #" << a.getId() << "\n";
            pause();
        }
        else if (ch == "3") {
            header("SUA TAC GIA");
            cout << left << setw(5) << "ID" << "Ten\n";
            for (int i = 0; i < (int)lib.authors.size(); i++)
                cout << setw(5) << lib.authors[i].getId() << lib.authors[i].getName() << "\n";
            int id = getInt("\n  ID can sua: ");
            Author* a = lib.findAuthor(id);
            if (!a) { cout << "  Khong tim thay!\n"; pause(); continue; }
            cout << "  Ten hien tai: " << a->getName() << "\n";
            string v = getStr("  Ten moi (bo trong = giu nguyen): ");
            if (!v.empty()) a->setName(v);
            Author::save(PATH_AUTHOR, lib.authors);
            cout << "  Da cap nhat!\n";
            pause();
        }
        else if (ch == "4") {
            header("XOA TAC GIA");
            cout << left << setw(5) << "ID" << "Ten\n";
            for (int i = 0; i < (int)lib.authors.size(); i++)
                cout << setw(5) << lib.authors[i].getId() << lib.authors[i].getName() << "\n";
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
            for (int i = 0; i < (int)lib.genres.size(); i++)
                cout << setw(5) << lib.genres[i].getId() << lib.genres[i].getName() << "\n";
            pause();
        }
        else if (ch == "2") {
            header("THEM THE LOAI");
            Genre g;
            g.setId(nextId(lib.genres));
            string n = getStr("  Ten: ");
            if (n.empty()) { cout << "  Ten khong duoc trong!\n"; pause(); continue; }
            g.setName(n);
            lib.genres.push_back(g);
            Genre::save(PATH_GENRE, lib.genres);
            cout << "  Da them the loai #" << g.getId() << "\n";
            pause();
        }
        else if (ch == "3") {
            header("SUA THE LOAI");
            cout << left << setw(5) << "ID" << "Ten\n";
            for (int i = 0; i < (int)lib.genres.size(); i++)
                cout << setw(5) << lib.genres[i].getId() << lib.genres[i].getName() << "\n";
            int id = getInt("\n  ID can sua: ");
            Genre* g = lib.findGenre(id);
            if (!g) { cout << "  Khong tim thay!\n"; pause(); continue; }
            cout << "  Ten hien tai: " << g->getName() << "\n";
            string v = getStr("  Ten moi (bo trong = giu nguyen): ");
            if (!v.empty()) g->setName(v);
            Genre::save(PATH_GENRE, lib.genres);
            cout << "  Da cap nhat!\n";
            pause();
        }
        else if (ch == "4") {
            header("XOA THE LOAI");
            cout << left << setw(5) << "ID" << "Ten\n";
            for (int i = 0; i < (int)lib.genres.size(); i++)
                cout << setw(5) << lib.genres[i].getId() << lib.genres[i].getName() << "\n";
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
            for (int i = 0; i < (int)lib.customers.size(); i++)
                cout << setw(5)  << lib.customers[i].getId()
                     << setw(22) << lib.customers[i].getName().substr(0, 20)
                     << setw(14) << lib.customers[i].getPhone()
                     << lib.customers[i].getAddress() << "\n";
            pause();
        }
        else if (ch == "2") {
            header("THEM KHACH HANG");
            Customer c;
            c.setId(nextId(lib.customers));
            c.setName(getStr("  Ho ten: "));
            c.setPhone(getStr("  So dien thoai: "));
            c.setAddress(getStr("  Dia chi: "));
            lib.customers.push_back(c);
            Customer::save(PATH_CUSTOMER, lib.customers);
            cout << "  Da them khach hang #" << c.getId() << "\n";
            pause();
        }
        else if (ch == "3") {
            header("SUA KHACH HANG");
            cout << left << setw(5) << "ID" << setw(22) << "Ho ten" << "SDT\n";
            for (int i = 0; i < (int)lib.customers.size(); i++)
                cout << setw(5) << lib.customers[i].getId()
                     << setw(22) << lib.customers[i].getName()
                     << lib.customers[i].getPhone() << "\n";
            int id = getInt("\n  ID can sua: ");
            Customer* c = lib.findCustomer(id);
            if (!c) { cout << "  Khong tim thay!\n"; pause(); continue; }
            cout << "  [Ten: " << c->getName()
                 << " | SDT: " << c->getPhone()
                 << " | DC: "  << c->getAddress() << "]\n";
            string v;
            v = getStr("  Ten moi    (bo trong = giu nguyen): "); if (!v.empty()) c->setName(v);
            v = getStr("  SDT moi    (bo trong = giu nguyen): "); if (!v.empty()) c->setPhone(v);
            v = getStr("  DC moi     (bo trong = giu nguyen): "); if (!v.empty()) c->setAddress(v);
            Customer::save(PATH_CUSTOMER, lib.customers);
            cout << "  Da cap nhat!\n";
            pause();
        }
        else if (ch == "4") {
            header("XOA KHACH HANG");
            cout << left << setw(5) << "ID" << "Ho ten\n";
            for (int i = 0; i < (int)lib.customers.size(); i++)
                cout << setw(5) << lib.customers[i].getId() << lib.customers[i].getName() << "\n";
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
            for (int i = 0; i < (int)lib.books.size(); i++) printSach(lib.books[i]);
            pause();
        }
        else if (ch == "2") {
            header("THEM SACH");
            cout << "  Tac gia:\n";
            for (int i = 0; i < (int)lib.authors.size(); i++)
                cout << "    " << lib.authors[i].getId() << ". " << lib.authors[i].getName() << "\n";
            cout << "  The loai:\n";
            for (int i = 0; i < (int)lib.genres.size(); i++) {
                cout << "    " << lib.genres[i].getId() << ". " << lib.genres[i].getName();
                cout << (i % 4 == 3 ? "\n" : "   ");
            }
            cout << "\n\n";
            Book b;
            b.setId(nextId(lib.books));
            b.setName(getStr("  Ten sach: "));
            b.setAuthorId(getInt("  ID tac gia: "));
            b.setAmount(getInt("  So luong: "));
            string gs = getStr("  ID the loai (cach nhau dau cach, VD: 1 3 7): ");
            vector<int> gids;
            vector<string> parts = split(gs, ' ');
            for (int i = 0; i < (int)parts.size(); i++)
                try { if (!parts[i].empty()) gids.push_back(stoi(parts[i])); } catch (...) {}
            b.setGenreIds(gids);
            lib.books.push_back(b);
            Book::save(PATH_BOOK, lib.books);
            cout << "  Da them sach #" << b.getId() << "\n";
            pause();
        }
        else if (ch == "3") {
            header("SUA SACH");
            cout << left << setw(5) << "ID" << setw(28) << "Ten" << "Tac gia\n";
            for (int i = 0; i < (int)lib.books.size(); i++) {
                Author* a = lib.findAuthor(lib.books[i].getAuthorId());
                cout << setw(5) << lib.books[i].getId()
                     << setw(28) << lib.books[i].getName().substr(0, 26)
                     << (a ? a->getName() : "?") << "\n";
            }
            int id = getInt("\n  ID can sua: ");
            Book* b = lib.findBook(id);
            if (!b) { cout << "  Khong tim thay!\n"; pause(); continue; }
            Author* a = lib.findAuthor(b->getAuthorId());
            cout << "  [Ten: " << b->getName()
                 << " | TG: " << (a ? a->getName() : "?")
                 << " | SL: " << b->getAmount() << "]\n";
            string v;
            v = getStr("  Ten moi       (bo trong = giu nguyen): ");
            if (!v.empty()) b->setName(v);
            v = getStr("  ID tac gia    (bo trong = giu nguyen): ");
            if (!v.empty()) try { b->setAuthorId(stoi(v)); } catch (...) {}
            v = getStr("  So luong      (bo trong = giu nguyen): ");
            if (!v.empty()) try { b->setAmount(stoi(v)); } catch (...) {}
            v = getStr("  The loai moi  (VD: 1 3, bo trong = giu nguyen): ");
            if (!v.empty()) {
                vector<int> gids;
                vector<string> parts = split(v, ' ');
                for (int i = 0; i < (int)parts.size(); i++)
                    try { if (!parts[i].empty()) gids.push_back(stoi(parts[i])); } catch (...) {}
                b->setGenreIds(gids);
            }
            Book::save(PATH_BOOK, lib.books);
            cout << "  Da cap nhat!\n";
            pause();
        }
        else if (ch == "4") {
            header("XOA SACH");
            cout << left << setw(5) << "ID" << setw(28) << "Ten" << "Tac gia\n";
            for (int i = 0; i < (int)lib.books.size(); i++) {
                Author* a = lib.findAuthor(lib.books[i].getAuthorId());
                cout << setw(5) << lib.books[i].getId()
                     << setw(28) << lib.books[i].getName().substr(0, 26)
                     << (a ? a->getName() : "?") << "\n";
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
            for (int i = 0; i < (int)lib.books.size(); i++) {
                string bn = lib.books[i].getName();
                transform(bn.begin(), bn.end(), bn.begin(), ::tolower);
                if (bn.find(kw) != string::npos) {
                    Author* a = lib.findAuthor(lib.books[i].getAuthorId());
                    cout << setw(5) << lib.books[i].getId()
                         << setw(28) << lib.books[i].getName().substr(0, 26)
                         << (a ? a->getName() : "?") << "\n";
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
            for (int i = 0; i < (int)lib.borrows.size(); i++)
                if (!lib.borrows[i].isReturned()) { printBorrow(lib.borrows[i]); count++; }
            if (count == 0) cout << "  (Khong co phieu nao dang muon)\n";
            pause();
        }
        else if (ch == "2") {
            header("LICH SU MUON TRA");
            cout << left << setw(5) << "ID" << setw(20) << "Khach hang"
                 << setw(24) << "Sach" << setw(12) << "Ngay muon"
                 << setw(12) << "Han tra" << "Trang thai\n";
            cout << string(85, '-') << "\n";
            for (int i = 0; i < (int)lib.borrows.size(); i++) printBorrow(lib.borrows[i]);
            if (lib.borrows.empty()) cout << "  (Chua co lich su)\n";
            pause();
        }
        else if (ch == "3") {
            header("MUON SACH");
            cout << "  Sach co san:\n";
            cout << left << "  " << setw(5) << "ID" << setw(28) << "Ten sach"
                 << setw(20) << "Tac gia" << "Con lai\n";
            cout << "  " << string(60, '-') << "\n";
            for (int i = 0; i < (int)lib.books.size(); i++) {
                int av = lib.available(lib.books[i].getId());
                if (av > 0) {
                    Author* a = lib.findAuthor(lib.books[i].getAuthorId());
                    cout << "  " << setw(5) << lib.books[i].getId()
                         << setw(28) << lib.books[i].getName().substr(0, 26)
                         << setw(20) << (a ? a->getName().substr(0, 18) : "?")
                         << av << "\n";
                }
            }
            cout << "\n  Khach hang:\n";
            for (int i = 0; i < (int)lib.customers.size(); i++)
                cout << "  " << setw(5) << lib.customers[i].getId()
                     << lib.customers[i].getName() << "\n";
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
            for (int i = 0; i < (int)lib.borrows.size(); i++) {
                if (!lib.borrows[i].isReturned()) {
                    Customer* c = lib.findCustomer(lib.borrows[i].getCustomerId());
                    Book*     b = lib.findBook(lib.borrows[i].getBookId());
                    cout << "  " << setw(5) << lib.borrows[i].getId()
                         << setw(20) << (c ? c->getName().substr(0, 18) : "?")
                         << setw(24) << (b ? b->getName().substr(0, 22) : "?")
                         << lib.borrows[i].getDueDate() << "\n";
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
        for (int i = 0; i < (int)lib.borrows.size(); i++)
            if (!lib.borrows[i].isReturned()) active++;

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
