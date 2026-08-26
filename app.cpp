#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <memory>
#include <type_traits>
#include <algorithm>

using namespace std;

vector<string> split(const string& line, char delim) {
    vector<string> tokens;
    stringstream ss(line);
    string token;
    while (getline(ss, token, delim)) tokens.push_back(token);
    return tokens;
}

vector<int> splitToInts(const string& s, char delim) {
    vector<int> result;
    for (const string& tok : split(s, delim)) {
        if (!tok.empty()) result.push_back(stoi(tok));
    }
    return result;
}

// ---------------- Entity (base cho Author, Genre, Customer, Book) ----------------
class Entity {
protected:
    int id;
    string name;
public:
    Entity() : id(0) {}
    Entity(int id, const string& name) : id(id), name(name) {}
    virtual ~Entity() = default;

    int getId() const { return id; }
    string getName() const { return name; }
    void setName(const string& n) { name = n; }

    virtual string toDatLine() const = 0;
    virtual void fromDatLine(const string& line) = 0;

    // Template method: doc toan bo file .dat thanh danh sach entity cu the.
    // Vong lap doc file chi viet 1 lan o day (ke thua), nhung tung dong duoc
    // parse dung theo tung lop nho goi qua ham ao fromDatLine (da hinh).
    // Phai la ham template vi can biet kieu T cu the de tao doi tuong moi
    // (new T()) - C++ khong co "virtual static".
    template <typename T>
    static vector<unique_ptr<T>> loadFromFile(const string& path) {
        static_assert(is_base_of<Entity, T>::value, "T phai ke thua Entity");
        vector<unique_ptr<T>> result;
        ifstream in(path);
        string line;
        while (getline(in, line)) {
            if (line.empty()) continue;
            unique_ptr<T> obj(new T());
            obj->fromDatLine(line);
            result.push_back(move(obj));
        }
        return result;
    }

    template <typename T>
    static void saveToFile(const string& path, const vector<unique_ptr<T>>& items) {
        static_assert(is_base_of<Entity, T>::value, "T phai ke thua Entity");
        ofstream out(path);
        for (const auto& item : items) {
            out << item->toDatLine() << "\n";
        }
    }
};

// ---------------- Author ----------------
// Format: id|name
class Author : public Entity {
public:
    Author() = default;
    Author(int id, const string& name) : Entity(id, name) {}

    string toDatLine() const override {
        return to_string(id) + "|" + name;
    }

    void fromDatLine(const string& line) override {
        auto f = split(line, '|');
        id = stoi(f[0]);
        name = f[1];
    }
};

// ---------------- Genre ----------------
// Format: id|name
class Genre : public Entity {
public:
    Genre() = default;
    Genre(int id, const string& name) : Entity(id, name) {}

    string toDatLine() const override {
        return to_string(id) + "|" + name;
    }

    void fromDatLine(const string& line) override {
        auto f = split(line, '|');
        id = stoi(f[0]);
        name = f[1];
    }
};

// ---------------- Customer (Khach) ----------------
// Format: id|name|description|address|phone
class Customer : public Entity {
    string des;
    string address;
    string phone;
public:
    Customer() = default;
    Customer(int id, const string& name, const string& des,
              const string& address, const string& phone)
        : Entity(id, name), des(des), address(address), phone(phone) {}

    string getDes() const { return des; }
    string getAddress() const { return address; }
    string getPhone() const { return phone; }

    string toDatLine() const override {
        return to_string(id) + "|" + name + "|" + des + "|" + address + "|" + phone;
    }

    void fromDatLine(const string& line) override {
        auto f = split(line, '|');
        id = stoi(f[0]);
        name = f[1];
        des = f[2];
        address = f[3];
        phone = f[4];
    }
};

// ---------------- Book (Sach) ----------------
// Format: id|name|description|a_id|amount|g_id1 g_id2 g_id3
class Book : public Entity {
    string des;
    int authorId = 0;
    int amount = 0;
    vector<int> genreIds;
public:
    Book() = default;
    Book(int id, const string& name, const string& des, int authorId,
          int amount, const vector<int>& genreIds)
        : Entity(id, name), des(des), authorId(authorId), amount(amount), genreIds(genreIds) {}

    string getDes() const { return des; }
    int getAuthorId() const { return authorId; }
    int getAmount() const { return amount; }
    const vector<int>& getGenreIds() const { return genreIds; }

    string toDatLine() const override {
        stringstream ss;
        ss << id << "|" << name << "|" << des << "|" << authorId << "|" << amount << "|";
        for (size_t i = 0; i < genreIds.size(); ++i) {
            if (i > 0) ss << " ";
            ss << genreIds[i];
        }
        return ss.str();
    }

    void fromDatLine(const string& line) override {
        auto f = split(line, '|');
        id = stoi(f[0]);
        name = f[1];
        des = f[2];
        authorId = stoi(f[3]);
        amount = stoi(f[4]);
        genreIds = f.size() > 5 ? splitToInts(f[5], ' ') : vector<int>();
    }
};

// ---------------- Borrow (Muon) ----------------
enum class BorrowStatus { DANG_MUON, DA_TRA };

string statusToString(BorrowStatus s) {
    return s == BorrowStatus::DANG_MUON ? "dang_muon" : "da_tra";
}

BorrowStatus statusFromString(const string& s) {
    return s == "da_tra" ? BorrowStatus::DA_TRA : BorrowStatus::DANG_MUON;
}

// Format: id|c_id|b_id|borrow_date|due_date|return_date|status
// Borrow KHONG ke thua Entity (khong co "ten") nen khong dung chung duoc
// Entity::loadFromFile/saveToFile - phai tu viet ham doc/ghi rieng ben duoi.
class Borrow {
    int id;
    int customerId;
    int bookId;
    string borrowDate;
    string dueDate;
    string returnDate;
    BorrowStatus status;
public:
    Borrow() : id(0), customerId(0), bookId(0), status(BorrowStatus::DANG_MUON) {}
    Borrow(int id, int customerId, int bookId, const string& borrowDate,
            const string& dueDate, const string& returnDate, BorrowStatus status)
        : id(id), customerId(customerId), bookId(bookId), borrowDate(borrowDate),
          dueDate(dueDate), returnDate(returnDate), status(status) {}

    int getId() const { return id; }
    int getCustomerId() const { return customerId; }
    int getBookId() const { return bookId; }
    string getBorrowDate() const { return borrowDate; }
    string getDueDate() const { return dueDate; }
    string getReturnDate() const { return returnDate; }
    BorrowStatus getStatus() const { return status; }

    string toDatLine() const {
        return to_string(id) + "|" + to_string(customerId) + "|" + to_string(bookId) + "|" +
               borrowDate + "|" + dueDate + "|" + returnDate + "|" + statusToString(status);
    }

    void fromDatLine(const string& line) {
        auto f = split(line, '|');
        id = stoi(f[0]);
        customerId = stoi(f[1]);
        bookId = stoi(f[2]);
        borrowDate = f[3];
        dueDate = f[4];
        returnDate = f.size() > 5 ? f[5] : "";
        status = f.size() > 6 ? statusFromString(f[6]) : BorrowStatus::DANG_MUON;
    }

    static vector<Borrow> loadFromFile(const string& path) {
        vector<Borrow> result;
        ifstream in(path);
        string line;
        while (getline(in, line)) {
            if (line.empty()) continue;
            Borrow b;
            b.fromDatLine(line);
            result.push_back(b);
        }
        return result;
    }

    static void saveToFile(const string& path, const vector<Borrow>& items) {
        ofstream out(path);
        for (const auto& b : items) out << b.toDatLine() << "\n";
    }
};

// ==================== Repository layer ====================
// Repository<T>: 1 tang trung gian giua file .dat va phan logic ben tren.
// Nam giu danh sach trong bo nho + duong dan file, cung cap CRUD day du
// (add/update/remove/findById). Moi thao tac lam thay doi du lieu deu
// tu dong ghi lai file ngay (auto-save) - dung chung cho Author/Genre/
// Customer/Book vi tat ca deu ke thua Entity.
template <typename T>
class Repository {
    static_assert(is_base_of<Entity, T>::value, "T phai ke thua Entity");
    string path;
    vector<unique_ptr<T>> items;

public:
    explicit Repository(const string& path) : path(path) {
        items = Entity::loadFromFile<T>(path);
    }

    const vector<unique_ptr<T>>& getAll() const { return items; }

    T* findById(int id) const {
        for (const auto& item : items) {
            if (item->getId() == id) return item.get();
        }
        return nullptr;
    }

    int nextId() const {
        int maxId = 0;
        for (const auto& item : items) maxId = max(maxId, item->getId());
        return maxId + 1;
    }

    void save() const { Entity::saveToFile(path, items); }

    T* add(unique_ptr<T> item) {
        items.push_back(move(item));
        save();
        return items.back().get();
    }

    bool update(int id, unique_ptr<T> replacement) {
        for (auto& item : items) {
            if (item->getId() == id) {
                item = move(replacement);
                save();
                return true;
            }
        }
        return false;
    }

    bool remove(int id) {
        for (size_t i = 0; i < items.size(); ++i) {
            if (items[i]->getId() == id) {
                items.erase(items.begin() + i);
                save();
                return true;
            }
        }
        return false;
    }
};

// BorrowRepository: cung mo hinh CRUD nhu Repository<T> o tren, nhung viet
// rieng vi Borrow khong ke thua Entity (khong dung duoc template chung).
class BorrowRepository {
    string path;
    vector<Borrow> items;

public:
    explicit BorrowRepository(const string& path) : path(path) {
        items = Borrow::loadFromFile(path);
    }

    const vector<Borrow>& getAll() const { return items; }

    Borrow* findById(int id) {
        for (auto& item : items) {
            if (item.getId() == id) return &item;
        }
        return nullptr;
    }

    int nextId() const {
        int maxId = 0;
        for (const auto& item : items) maxId = max(maxId, item.getId());
        return maxId + 1;
    }

    void save() const { Borrow::saveToFile(path, items); }

    Borrow* add(const Borrow& b) {
        items.push_back(b);
        save();
        return &items.back();
    }

    bool update(int id, const Borrow& replacement) {
        for (auto& item : items) {
            if (item.getId() == id) {
                item = replacement;
                save();
                return true;
            }
        }
        return false;
    }

    bool remove(int id) {
        for (size_t i = 0; i < items.size(); ++i) {
            if (items[i].getId() == id) {
                items.erase(items.begin() + i);
                save();
                return true;
            }
        }
        return false;
    }
};

// ==================== Service layer ====================
// LibraryService: chua nghiep vu lien quan toi NHIEU repository cung luc -
// dieu ma tung Repository rieng le khong the tu lam vi no chi biet ve 1
// loai du lieu. Vi du: xoa Author phai kiem tra ben Book, muon sach phai
// kiem tra ben Book + Borrow. Day la noi duy nhat "hieu" cac rang buoc
// tham chieu cheo giua cac bang.
class LibraryService {
    Repository<Author>& authors;
    Repository<Genre>& genres;
    Repository<Customer>& customers;
    Repository<Book>& books;
    BorrowRepository& borrows;

public:
    LibraryService(Repository<Author>& authors, Repository<Genre>& genres,
                   Repository<Customer>& customers, Repository<Book>& books,
                   BorrowRepository& borrows)
        : authors(authors), genres(genres), customers(customers),
          books(books), borrows(borrows) {}

    int countActiveBorrows(int bookId) const {
        int count = 0;
        for (const auto& b : borrows.getAll()) {
            if (b.getBookId() == bookId && b.getStatus() == BorrowStatus::DANG_MUON) count++;
        }
        return count;
    }

    int availableCopies(int bookId) const {
        Book* book = books.findById(bookId);
        if (!book) return 0;
        return book->getAmount() - countActiveBorrows(bookId);
    }

    bool isAuthorReferenced(int authorId) const {
        for (const auto& b : books.getAll()) {
            if (b->getAuthorId() == authorId) return true;
        }
        return false;
    }

    bool isGenreReferenced(int genreId) const {
        for (const auto& b : books.getAll()) {
            const auto& ids = b->getGenreIds();
            if (find(ids.begin(), ids.end(), genreId) != ids.end()) return true;
        }
        return false;
    }

    bool isBookReferenced(int bookId) const {
        return countActiveBorrows(bookId) > 0;
    }

    // Xoa co kiem tra rang buoc - tra ve false neu con bi tham chieu,
    // khong xoa "lieu" de tranh id mo coi.
    bool removeAuthor(int id) {
        if (isAuthorReferenced(id)) return false;
        return authors.remove(id);
    }

    bool removeGenre(int id) {
        if (isGenreReferenced(id)) return false;
        return genres.remove(id);
    }

    bool removeBook(int id) {
        if (isBookReferenced(id)) return false;
        return books.remove(id);
    }

    bool removeCustomer(int id) {
        for (const auto& b : borrows.getAll()) {
            if (b.getCustomerId() == id && b.getStatus() == BorrowStatus::DANG_MUON) return false;
        }
        return customers.remove(id);
    }

    bool borrowBook(int customerId, int bookId, const string& borrowDate, const string& dueDate) {
        if (!customers.findById(customerId) || !books.findById(bookId)) return false;
        if (availableCopies(bookId) <= 0) return false;
        borrows.add(Borrow(borrows.nextId(), customerId, bookId, borrowDate, dueDate, "",
                            BorrowStatus::DANG_MUON));
        return true;
    }

    bool returnBook(int borrowId, const string& returnDate) {
        Borrow* b = borrows.findById(borrowId);
        if (!b || b->getStatus() == BorrowStatus::DA_TRA) return false;
        Borrow updated(b->getId(), b->getCustomerId(), b->getBookId(), b->getBorrowDate(),
                       b->getDueDate(), returnDate, BorrowStatus::DA_TRA);
        return borrows.update(borrowId, updated);
    }

    bool isOverdue(const Borrow& b, const string& today) const {
        return b.getStatus() == BorrowStatus::DANG_MUON && today > b.getDueDate();
    }
};

// ---------------- Demo: dung Repository + Service voi du lieu that ----------------
int main() {
    Repository<Author> authorRepo("Data/Author.dat");
    Repository<Genre> genreRepo("Data/Genre.dat");
    Repository<Customer> customerRepo("Data/Customer.dat");
    Repository<Book> bookRepo("Data/Book.dat");
    BorrowRepository borrowRepo("Data/Borrow.dat");
    LibraryService service(authorRepo, genreRepo, customerRepo, bookRepo, borrowRepo);

    cout << "Da nap: " << authorRepo.getAll().size() << " tac gia, "
         << genreRepo.getAll().size() << " the loai, "
         << customerRepo.getAll().size() << " khach hang, "
         << bookRepo.getAll().size() << " sach, "
         << borrowRepo.getAll().size() << " luot muon.\n";

    // --- Them (Create) ---
    Author* newAuthor = authorRepo.add(
        unique_ptr<Author>(new Author(authorRepo.nextId(), "Tac Gia Moi")));
    cout << "\n[Them] Tac gia moi id=" << newAuthor->getId() << "\n";

    // --- Sua (Update): doi ten tac gia vua them ---
    authorRepo.update(newAuthor->getId(),
                       unique_ptr<Author>(new Author(newAuthor->getId(), "Tac Gia Da Sua")));
    cout << "[Sua] Tac gia id=" << newAuthor->getId() << " -> "
         << authorRepo.findById(newAuthor->getId())->toDatLine() << "\n";

    // --- Xoa (Delete) khong bi rang buoc: thanh cong ---
    bool removed = service.removeAuthor(newAuthor->getId());
    cout << "[Xoa] Tac gia id=" << newAuthor->getId() << " (khong bi sach nao dung) -> "
         << (removed ? "thanh cong" : "that bai") << "\n";

    // --- Xoa bi chan vi con rang buoc: lay dung 1 authorId dang duoc 1 sach dung ---
    int referencedAuthorId = bookRepo.getAll().front()->getAuthorId();
    bool blocked = service.removeAuthor(referencedAuthorId);
    cout << "[Xoa] Tac gia id=" << referencedAuthorId << " (dang duoc Book tham chieu) -> "
         << (blocked ? "thanh cong" : "bi chan (dung nhu ky vong)") << "\n";

    // --- Nghiep vu muon/tra sach ---
    int bookId = 1;
    cout << "\nSo luong con lai cua sach id=" << bookId << " truoc khi muon: "
         << service.availableCopies(bookId) << "\n";

    bool borrowed = service.borrowBook(1, bookId, "2026-08-21", "2026-09-04");
    cout << "Khach id=1 muon sach id=" << bookId << " -> " << (borrowed ? "thanh cong" : "that bai")
         << " (con lai: " << service.availableCopies(bookId) << ")\n";

    int lastBorrowId = borrowRepo.getAll().back().getId();
    bool bookRemovedWhileBorrowed = service.removeBook(bookId);
    cout << "Thu xoa sach id=" << bookId << " khi dang co nguoi muon -> "
         << (bookRemovedWhileBorrowed ? "thanh cong" : "bi chan (dung nhu ky vong)") << "\n";

    service.returnBook(lastBorrowId, "2026-08-30");
    cout << "Khach tra sach (borrow id=" << lastBorrowId << ") -> con lai: "
         << service.availableCopies(bookId) << "\n";

    return 0;
}
