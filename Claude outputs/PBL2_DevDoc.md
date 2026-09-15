# Tài liệu kỹ thuật — Dự án PBL2 (Hệ thống Quản lý Thư viện)

> Ngôn ngữ: C++11 · Compiler: GCC 4.9.2 (MinGW) · Build: `g++ -std=c++11 -o app.exe main.cpp`

---

## Mục lục

1. [utils.h — Tiện ích dùng chung](#1-utilsh)
2. [Author.h — Tác giả](#2-authorh)
3. [Genre.h — Thể loại](#3-genreh)
4. [Customer.h — Khách hàng](#4-customerh)
5. [Book.h — Sách](#5-bookh)
6. [Borrow.h — Phiếu mượn](#6-borrowh)
7. [Library.h — Thư viện (lớp trung tâm)](#7-libraryh)
8. [Hằng số đường dẫn file](#8-hằng-số-đường-dẫn-file)
9. [Format dữ liệu file .dat](#9-format-dữ-liệu-file-dat)

---

## 1. utils.h

File tiện ích dùng chung, được `#include` trong mọi file khác.

### Hàm `split`

```cpp
vector<string> split(const string& s, char d)
```

| Tham số | Kiểu | Mô tả |
|---------|------|--------|
| `s` | `const string&` | Chuỗi cần tách |
| `d` | `char` | Ký tự phân cách |

**Trả về:** `vector<string>` — danh sách các phần sau khi tách.

**Ví dụ:**
```cpp
split("1|Nguyen Van A", '|')
// → ["1", "Nguyen Van A"]
```

---

### Hàm `nextId` *(template)*

```cpp
template<typename T>
int nextId(const vector<T>& v)
```

| Tham số | Kiểu | Mô tả |
|---------|------|--------|
| `v` | `const vector<T>&` | Vector bất kỳ, yêu cầu `T` có trường `int id` |

**Trả về:** `int` — giá trị `max(id) + 1`. Trả về `1` nếu vector rỗng.

**Ví dụ:**
```cpp
nextId(lib.authors)  // → ID tiếp theo cho Author
```

---

### Hàm `getStr`

```cpp
string getStr(const string& p)
```

| Tham số | Kiểu | Mô tả |
|---------|------|--------|
| `p` | `const string&` | Chuỗi prompt hiển thị trước khi chờ nhập |

**Trả về:** `string` — dòng văn bản người dùng nhập từ `stdin`.

---

### Hàm `getInt`

```cpp
int getInt(const string& p)
```

| Tham số | Kiểu | Mô tả |
|---------|------|--------|
| `p` | `const string&` | Chuỗi prompt |

**Trả về:** `int` — số nguyên hợp lệ. Vòng lặp vô hạn cho đến khi nhập đúng.

---

### Hàm `clr`

```cpp
void clr()
```

Xóa màn hình console. Gọi `system("cls")` (Windows).

---

### Hàm `pause`

```cpp
void pause()
```

In `[Enter de tiep tuc...]` rồi chờ người dùng nhấn Enter.

---

### Hàm `header`

```cpp
void header(const string& t)
```

| Tham số | Kiểu | Mô tả |
|---------|------|--------|
| `t` | `const string&` | Tiêu đề cần in |

In ra định dạng: `=== <t> ===`

---

## 2. Author.h

### Struct `Author`

| Trường | Kiểu | Mặc định | Mô tả |
|--------|------|----------|--------|
| `id` | `int` | `0` | Mã định danh, duy nhất |
| `name` | `string` | `""` | Tên tác giả |

### Phương thức

#### `toLine`

```cpp
string toLine() const
```

**Trả về:** `string` — dòng văn bản theo format `id|name` để ghi file.

---

#### `fromLine` *(static)*

```cpp
static Author fromLine(const string& line)
```

| Tham số | Kiểu | Mô tả |
|---------|------|--------|
| `line` | `const string&` | Một dòng từ file `Author.dat` |

**Trả về:** `Author` — đối tượng được khôi phục từ dòng văn bản.

---

#### `load` *(static)*

```cpp
static vector<Author> load(const string& path)
```

| Tham số | Kiểu | Mô tả |
|---------|------|--------|
| `path` | `const string&` | Đường dẫn đến file `Author.dat` |

**Trả về:** `vector<Author>` — toàn bộ danh sách tác giả. Trả về vector rỗng nếu file không tồn tại.

---

#### `save` *(static)*

```cpp
static void save(const string& path, const vector<Author>& v)
```

| Tham số | Kiểu | Mô tả |
|---------|------|--------|
| `path` | `const string&` | Đường dẫn file đích |
| `v` | `const vector<Author>&` | Danh sách cần ghi |

Ghi đè toàn bộ file. Mỗi Author chiếm một dòng.

---

## 3. Genre.h

### Struct `Genre`

| Trường | Kiểu | Mặc định | Mô tả |
|--------|------|----------|--------|
| `id` | `int` | `0` | Mã định danh, duy nhất |
| `name` | `string` | `""` | Tên thể loại |

### Phương thức

Tương tự `Author`: `toLine()`, `fromLine(line)`, `load(path)`, `save(path, v)` — cùng chữ ký, cùng logic, thay `Author` bằng `Genre`.

---

## 4. Customer.h

### Struct `Customer`

| Trường | Kiểu | Mặc định | Mô tả |
|--------|------|----------|--------|
| `id` | `int` | `0` | Mã định danh, duy nhất |
| `name` | `string` | `""` | Họ tên đầy đủ |
| `phone` | `string` | `""` | Số điện thoại |
| `address` | `string` | `""` | Địa chỉ |

### Phương thức

#### `toLine`

```cpp
string toLine() const
```

**Trả về:** `string` — format `id|name|phone|address`.

---

#### `fromLine` *(static)*

```cpp
static Customer fromLine(const string& line)
```

Tách theo `|`. Trường `phone` và `address` lấy an toàn bằng kiểm tra `f.size()` để tránh lỗi khi dữ liệu thiếu cột.

---

`load(path)` và `save(path, v)` — cùng pattern với `Author`.

---

## 5. Book.h

### Struct `Book`

| Trường | Kiểu | Mặc định | Mô tả |
|--------|------|----------|--------|
| `id` | `int` | `0` | Mã định danh, duy nhất |
| `name` | `string` | `""` | Tựa đề sách |
| `authorId` | `int` | `0` | Khóa ngoại → `Author.id` |
| `amount` | `int` | `0` | Tổng số bản có trong thư viện |
| `genreIds` | `vector<int>` | `{}` | Danh sách khóa ngoại → `Genre.id` |

### Phương thức

#### `toLine`

```cpp
string toLine() const
```

**Trả về:** `string` — format `id|name|authorId|amount|g1 g2 g3`  
Phần `genreIds` được nối bằng dấu cách. Nếu không có thể loại, cột cuối là chuỗi rỗng.

---

#### `fromLine` *(static)*

```cpp
static Book fromLine(const string& line)
```

Tách `|` lấy 5 cột. Cột thứ 5 (index 4) tiếp tục tách theo `' '` để lấy `genreIds`. Bỏ qua chuỗi rỗng khi tách.

---

`load(path)` và `save(path, v)` — cùng pattern với `Author`.

---

## 6. Borrow.h

### Struct `Borrow`

| Trường | Kiểu | Mặc định | Mô tả |
|--------|------|----------|--------|
| `id` | `int` | `0` | Mã phiếu mượn, duy nhất |
| `customerId` | `int` | `0` | Khóa ngoại → `Customer.id` |
| `bookId` | `int` | `0` | Khóa ngoại → `Book.id` |
| `borrowDate` | `string` | `""` | Ngày mượn, format `YYYY-MM-DD` |
| `dueDate` | `string` | `""` | Hạn trả, format `YYYY-MM-DD` |
| `returnDate` | `string` | `""` | Ngày trả thực tế (rỗng nếu chưa trả) |
| `returned` | `bool` | `false` | `true` = đã trả |

### Phương thức

#### `toLine`

```cpp
string toLine() const
```

**Trả về:** `string` — format `id|customerId|bookId|borrowDate|dueDate|returnDate|returned`  
`returned` lưu là `"1"` (đã trả) hoặc `"0"` (chưa trả).

---

#### `fromLine` *(static)*

```cpp
static Borrow fromLine(const string& line)
```

Tách 7 cột. `returned = (f.size() > 6 && f[6] == "1")`.

---

`load(path)` và `save(path, v)` — cùng pattern với `Author`.

---

## 7. Library.h

Lớp trung tâm. Nắm giữ toàn bộ dữ liệu trong bộ nhớ và xử lý logic nghiệp vụ.

### Thuộc tính (public)

| Trường | Kiểu | Mô tả |
|--------|------|--------|
| `authors` | `vector<Author>` | Danh sách tác giả |
| `genres` | `vector<Genre>` | Danh sách thể loại |
| `customers` | `vector<Customer>` | Danh sách khách hàng |
| `books` | `vector<Book>` | Danh sách sách |
| `borrows` | `vector<Borrow>` | Danh sách phiếu mượn |

---

### Phương thức

#### `load`

```cpp
void load()
```

Nạp tất cả 5 vector từ file `.dat` tương ứng. Gọi một lần khi khởi động.

---

#### `findAuthor`

```cpp
Author* findAuthor(int id)
```

| Tham số | Kiểu | Mô tả |
|---------|------|--------|
| `id` | `int` | Mã tác giả cần tìm |

**Trả về:** `Author*` — con trỏ đến phần tử trong `authors`, hoặc `NULL` nếu không tìm thấy.

> **Lưu ý:** Con trỏ bị vô hiệu sau khi `authors` thay đổi kích thước (push_back / erase).

---

#### `findGenre`

```cpp
Genre* findGenre(int id)
```

Tương tự `findAuthor`, tìm trong `genres`.

---

#### `findCustomer`

```cpp
Customer* findCustomer(int id)
```

Tìm trong `customers`.

---

#### `findBook`

```cpp
Book* findBook(int id)
```

Tìm trong `books`.

---

#### `findBorrow`

```cpp
Borrow* findBorrow(int id)
```

Tìm trong `borrows`.

---

#### `available`

```cpp
int available(int bookId)
```

| Tham số | Kiểu | Mô tả |
|---------|------|--------|
| `bookId` | `int` | Mã sách cần kiểm tra |

**Trả về:** `int` — số bản còn có thể cho mượn.  
Công thức: `Book.amount − (số Borrow có bookId == bookId và returned == false)`.  
Trả về `0` nếu `bookId` không tồn tại.

---

#### `borrow`

```cpp
bool borrow(int customerId, int bookId,
            const string& borrowDate, const string& dueDate)
```

| Tham số | Kiểu | Mô tả |
|---------|------|--------|
| `customerId` | `int` | Mã khách hàng |
| `bookId` | `int` | Mã sách |
| `borrowDate` | `const string&` | Ngày mượn (`YYYY-MM-DD`) |
| `dueDate` | `const string&` | Hạn trả (`YYYY-MM-DD`) |

**Trả về:** `bool`
- `false` nếu khách hoặc sách không tồn tại, hoặc không còn bản để mượn.
- `true` nếu tạo phiếu thành công. Phiếu mới được `push_back` vào `borrows` và ghi file ngay.

---

#### `returnBook`

```cpp
bool returnBook(int borrowId, const string& returnDate)
```

| Tham số | Kiểu | Mô tả |
|---------|------|--------|
| `borrowId` | `int` | Mã phiếu mượn |
| `returnDate` | `const string&` | Ngày trả thực tế (`YYYY-MM-DD`) |

**Trả về:** `bool`
- `false` nếu phiếu không tồn tại hoặc đã được trả trước đó.
- `true` nếu đánh dấu `returned = true`, ghi `returnDate`, lưu file.

---

#### `removeAuthor`

```cpp
bool removeAuthor(int id)
```

**Điều kiện:** Từ chối xóa (`return false`) nếu tồn tại bất kỳ `Book` nào có `authorId == id`.  
**Trả về:** `true` nếu xóa thành công và ghi file.

---

#### `removeGenre`

```cpp
bool removeGenre(int id)
```

**Điều kiện:** Từ chối nếu tồn tại `Book` nào có `id` trong `genreIds`.  
**Trả về:** `true` nếu thành công.

---

#### `removeBook`

```cpp
bool removeBook(int id)
```

**Điều kiện:** Từ chối nếu có `Borrow` nào có `bookId == id` và `returned == false`.  
**Trả về:** `true` nếu thành công.

---

#### `removeCustomer`

```cpp
bool removeCustomer(int id)
```

**Điều kiện:** Từ chối nếu có `Borrow` nào có `customerId == id` và `returned == false`.  
**Trả về:** `true` nếu thành công.

---

## 8. Hằng số đường dẫn file

Khai báo trong `Library.h`, đường dẫn tương đối từ thư mục chạy chương trình:

```cpp
const string PATH_AUTHOR   = "Data/Author.dat";
const string PATH_GENRE    = "Data/Genre.dat";
const string PATH_CUSTOMER = "Data/Customer.dat";
const string PATH_BOOK     = "Data/Book.dat";
const string PATH_BORROW   = "Data/Borrow.dat";
```

---

## 9. Format dữ liệu file .dat

Mỗi bản ghi là một dòng. Các trường cách nhau bằng `|`. Không có dòng header.

| File | Format |
|------|--------|
| `Author.dat` | `id\|name` |
| `Genre.dat` | `id\|name` |
| `Customer.dat` | `id\|name\|phone\|address` |
| `Book.dat` | `id\|name\|authorId\|amount\|g1 g2 g3` |
| `Borrow.dat` | `id\|customerId\|bookId\|borrowDate\|dueDate\|returnDate\|returned` |

**Ví dụ `Book.dat`:**
```
1|Toi Thay Hoa Vang Tren Co Xanh|1|36|2 12
2|Dat Rung Phuong Nam|2|39|8 9
```

**Ví dụ `Borrow.dat`:**
```
1|3|2|2024-01-10|2024-01-24||0
2|1|5|2024-01-11|2024-01-25|2024-01-20|1
```

---

*Tài liệu này được tạo tự động từ mã nguồn dự án PBL2.*
