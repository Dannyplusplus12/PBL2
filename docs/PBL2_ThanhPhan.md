# Tài liệu thành phần — PBL2 (Hệ thống Quản lý Thư viện)

> Mục đích: liệt kê gọn **mỗi class có gì** (thuộc tính) và **mỗi hàm làm gì**.
> Ngôn ngữ: C++11 · Ứng dụng console · Dữ liệu lưu dạng text (mỗi bản ghi 1 dòng, ngăn cách bằng `|`).

## 1. Tổng quan cấu trúc

| File | Vai trò |
|------|---------|
| `Entity.h` | Lớp cơ sở chứa `id` + `name`, các lớp dữ liệu kế thừa từ đây |
| `Author.h` | Tác giả |
| `Genre.h` | Thể loại |
| `Customer.h` | Khách hàng |
| `Book.h` | Sách |
| `Borrow.h` | Phiếu mượn / trả |
| `Library.h` | Lớp trung tâm: nạp dữ liệu + xử lý nghiệp vụ |
| `utils.h` | Hàm tiện ích dùng chung |
| `main.cpp` | Menu console + toàn bộ giao diện |

Sơ đồ kế thừa: `Author`, `Genre`, `Customer`, `Book` → kế thừa `Entity`. `Borrow` đứng độc lập.

---

## 2. Entity.h — lớp cơ sở

**Thuộc tính:** `id` (int), `name` (string).

| Hàm | Tác dụng |
|-----|----------|
| `Entity()` / `Entity(id, name)` | Khởi tạo (mặc định `id = 0`) |
| `getId()` / `getName()` | Lấy id / tên |
| `setId(i)` / `setName(n)` | Gán id / tên |
| `toLine()` *(virtual)* | Xuất chuỗi `id|name` để ghi file |

---

## 3. Author.h — Tác giả

**Thuộc tính:** kế thừa `id`, `name`.

| Hàm | Tác dụng |
|-----|----------|
| `Author()` / `Author(id, name)` | Khởi tạo |
| `fromLine(line)` *(static)* | Đọc 1 dòng `id|name` → đối tượng Author |
| `load(path)` *(static)* | Đọc toàn bộ file → `vector<Author>` |
| `save(path, v)` *(static)* | Ghi `vector<Author>` ra file (mỗi đối tượng 1 dòng) |

> **`Genre.h` giống hệt `Author.h`** — chỉ đổi tên class `Author` → `Genre`.

---

## 4. Customer.h — Khách hàng

**Thuộc tính:** `id`, `name`, `phone` (string), `address` (string).

| Hàm | Tác dụng |
|-----|----------|
| `Customer()` / `Customer(...)` | Khởi tạo |
| `getPhone()` / `getAddress()` | Lấy số điện thoại / địa chỉ |
| `setPhone(p)` / `setAddress(a)` | Gán số điện thoại / địa chỉ |
| `toLine()` | Xuất `id|name|phone|address` |
| `fromLine(line)` *(static)* | Đọc dòng → Customer |
| `load(path)` / `save(path, v)` | Đọc / ghi file |

---

## 5. Book.h — Sách

**Thuộc tính:** `id`, `name`, `authorId` (int), `amount` (int), `genreIds` (vector<int>).

| Hàm | Tác dụng |
|-----|----------|
| `getAuthorId()` / `getAmount()` / `getGenreIds()` | Lấy mã tác giả / số lượng / danh sách thể loại |
| `setAuthorId()` / `setAmount()` / `setGenreIds()` | Gán các trường tương ứng |
| `toLine()` | Xuất `id|name|authorId|amount|g1 g2 g3` (thể loại nối bằng dấu cách) |
| `fromLine(line)` *(static)* | Đọc dòng → Book (tách thể loại theo dấu cách) |
| `load(path)` / `save(path, v)` | Đọc / ghi file |

---

## 6. Borrow.h — Phiếu mượn

**Thuộc tính:** `id`, `customerId`, `bookId`, `borrowDate`, `dueDate`, `returnDate`, `returned` (bool).

| Hàm | Tác dụng |
|-----|----------|
| getters / setters | Lấy / gán từng trường |
| `isReturned()` | Trả `true` nếu phiếu đã trả |
| `toLine()` | Xuất `id|customerId|bookId|borrowDate|dueDate|returnDate|returned(0/1)` |
| `fromLine(line)` *(static)* | Đọc dòng → Borrow |
| `load(path)` / `save(path, v)` | Đọc / ghi file |

---

## 7. Library.h — lớp trung tâm

**Thuộc tính (public):** `authors`, `genres`, `customers`, `books`, `borrows` (5 vector).

| Hàm | Tác dụng |
|-----|----------|
| `load()` | Nạp cả 5 vector từ các file `.dat` (gọi 1 lần khi khởi động) |
| `findAuthor(id)` / `findGenre(id)` / `findCustomer(id)` / `findBook(id)` / `findBorrow(id)` | Tìm theo id, trả con trỏ đến phần tử hoặc `NULL` |
| `available(bookId)` | Số bản còn mượn được = `amount` − số phiếu đang mượn (chưa trả) |
| `borrow(customerId, bookId, borrowDate, dueDate)` | Tạo phiếu mượn (kiểm tra khách/sách tồn tại + còn bản) |
| `returnBook(borrowId, returnDate)` | Đánh dấu phiếu đã trả + ghi ngày trả |
| `removeAuthor(id)` | Xóa tác giả (từ chối nếu sách đang tham chiếu tới) |
| `removeGenre(id)` | Xóa thể loại (từ chối nếu sách đang dùng) |
| `removeBook(id)` | Xóa sách (từ chối nếu đang có người mượn) |
| `removeCustomer(id)` | Xóa khách (từ chối nếu còn sách chưa trả) |

---

## 8. utils.h — hàm tiện ích

| Hàm | Tác dụng |
|-----|----------|
| `split(s, d)` | Tách chuỗi theo ký tự `d` → `vector<string>` |
| `nextId(v)` | Trả `max(id) + 1` để sinh id mới |
| `getStr(p)` | In prompt `p` rồi đọc 1 dòng chuỗi từ bàn phím |
| `getInt(p)` | Đọc số nguyên (lặp lại cho đến khi nhập hợp lệ) |
| `clr()` | Xóa màn hình console (`system("cls")`) |
| `pause()` | In "Enter để tiếp tục..." rồi chờ nhấn Enter |
| `header(t)` | In tiêu đề dạng `=== t ===` |

---

## 9. main.cpp — giao diện

| Hàm | Tác dụng |
|-----|----------|
| `printSach(b)` | In 1 dòng sách (id, tên, tác giả, số lượng, còn lại, thể loại) |
| `printBorrow(bw)` | In 1 dòng phiếu mượn (id, khách, sách, ngày mượn, hạn trả, trạng thái) |
| `menuTacGia()` | Menu tác giả: danh sách / thêm / sửa / xóa |
| `menuTheLoai()` | Menu thể loại: danh sách / thêm / sửa / xóa |
| `menuKhach()` | Menu khách hàng: danh sách / thêm / sửa / xóa |
| `menuSach()` | Menu sách: danh sách / thêm / sửa / xóa / tìm kiếm |
| `menuMuonTra()` | Menu mượn/trả: đang mượn / lịch sử / mượn / trả |
| `main()` | Nạp dữ liệu rồi chạy vòng lặp menu chính |

---

## 10. Đường dẫn file & format dữ liệu

Đường dẫn khai báo trong `Library.h`, tương đối từ thư mục chạy chương trình.

| File | Format mỗi dòng |
|------|-----------------|
| `Data/Author.dat` | `id|name` |
| `Data/Genre.dat` | `id|name` |
| `Data/Customer.dat` | `id|name|phone|address` |
| `Data/Book.dat` | `id|name|authorId|amount|g1 g2 g3` |
| `Data/Borrow.dat` | `id|customerId|bookId|borrowDate|dueDate|returnDate|returned` |
