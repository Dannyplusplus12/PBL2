# Tài liệu thành phần — PBL2 (Hệ thống Quản lý Thư viện)

Oct 14, 2026 · @Phúc

> Mục đích: liệt kê gọn **mỗi class có gì** (thuộc tính) và **mỗi hàm làm gì**. Ngôn ngữ: C++20 · Ứng dụng console · Dữ liệu lưu dạng text (mỗi bản ghi 1 dòng, ngăn cách bằng `|`).

## 1. Tổng quan cấu trúc

| File | Vai trò |
| --- | --- |
| `Entity.h` | Lớp cơ sở chứa `id` + `name`, các lớp dữ liệu kế thừa từ đây |
| `Author.h` | Tác giả |
| `Genre.h` | Thể loại |
| `Customer.h` | Khách hàng |
| `Book.h` | Sách |
| `Borrow.h` | Phiếu mượn / trả |
| `Library.h` | Lớp trung tâm: nạp dữ liệu + xử lý nghiệp vụ |
| `utils.h` | Hàm tiện ích dùng chung |
| `DateUtils.h` | Hàm xử lý ngày tháng |
| `mainLib.cpp` | Menu console + toàn bộ giao diện |

Sơ đồ kế thừa: `Author`, `Genre`, `Customer`, `Book` → kế thừa `Entity`. `Borrow` đứng độc lập.

## 2. Entity.h — lớp cơ sở

**Thuộc tính:** `id` (int), `name` (string).

| Hàm | Tác dụng |
| --- | --- |
| `Entity()` / `Entity(id, name)` | Khởi tạo (mặc định `id = 0`) |
| `getId()` / `getName()` | Lấy id / tên |
| `setId(i)` / `setName(n)` | Gán id / tên |
| `toLine()` *(virtual)* | Xuất chuỗi `id\|name` để ghi file |

## 3. Author.h — Tác giả

**Thuộc tính:** kế thừa `id`, `name`.

| Hàm | Tác dụng |
| --- | --- |
| `Author()` / `Author(id, name)` | Khởi tạo |
| `fromLine(line)` *(static)* | Đọc 1 dòng `id\|name` → đối tượng Author |
| `load(path)` *(static)* | Đọc toàn bộ file → `vector<Author>` |
| `save(path, v)` *(static)* | Ghi `vector<Author>` ra file (mỗi đối tượng 1 dòng) |

> **`Genre.h` giống hệt `Author.h`** — chỉ đổi tên class `Author` → `Genre`.

## 4. Customer.h — Khách hàng

**Thuộc tính:** `id`, `name`, `phone` (string), `address` (string), `lock` (bool, `true` khi khách bị khóa).

| Hàm | Tác dụng |
| --- | --- |
| `Customer()` / `Customer(...)` | Khởi tạo (mặc định `lock = false`) |
| `getPhone()` / `getAddress()` | Lấy số điện thoại / địa chỉ |
| `isLocked()` | Trả `true` nếu khách đang bị khóa |
| `setPhone(p)` / `setAddress(a)` | Gán số điện thoại / địa chỉ |
| `setLock(l)` | Khóa / mở khóa khách |
| `toLine()` | Xuất `id\|name\|phone\|address\|lock(0/1)` |
| `fromLine(line)` *(static)* | Đọc dòng → Customer |
| `load(path)` / `save(path, v)` | Đọc / ghi file |

## 5. Book.h — Sách

**Thuộc tính:** `id`, `name`, `authorId` (int), `amount` (int), `genreIds` (vector\<int>).

| Hàm | Tác dụng |
| --- | --- |
| `getAuthorId()` / `getAmount()` / `getGenreIds()` | Lấy mã tác giả / số lượng / danh sách thể loại |
| `setAuthorId()` / `setAmount()` / `setGenreIds()` | Gán các trường tương ứng |
| `toLine()` | Xuất `id\|name\|authorId\|amount\|g1 g2 g3` (thể loại nối bằng dấu cách) |
| `fromLine(line)` *(static)* | Đọc dòng → Book (tách thể loại theo dấu cách) |
| `load(path)` / `save(path, v)` | Đọc / ghi file |

## 6. Borrow.h — Phiếu mượn

**Thuộc tính:** `id`, `customerId`, `bookId`, `borrowDate`, `dueDate`, `returnDate`, `returned` (bool), `renewCount` (int, số lần đã gia hạn).

| Hàm | Tác dụng |
| --- | --- |
| getters / setters | Lấy / gán từng trường (kể cả `getRenewCount()` / `setRenewCount()`) |
| `isReturned()` | Trả `true` nếu phiếu đã trả |
| `toLine()` | Xuất `id\|customerId\|bookId\|borrowDate\|dueDate\|returnDate\|returned(0/1)\|renewCount` |
| `fromLine(line)` *(static)* | Đọc dòng → Borrow |
| `load(path)` / `save(path, v)` | Đọc / ghi file |

## 7. Library.h — lớp trung tâm

**Thuộc tính (public):** `authors`, `genres`, `customers`, `books`, `borrows` (5 vector).

| Hàm | Tác dụng |
| --- | --- |
| `load()` | Nạp cả 5 vector từ các file `.dat` (gọi 1 lần khi khởi động) |
| `findAuthor(id)` / `findGenre(id)` / `findCustomer(id)` / `findBook(id)` / `findBorrow(id)` | Tìm theo id, trả con trỏ đến phần tử hoặc `NULL` |
| `authorExists(name, excludeId)` / `genreExists(name, excludeId)` | Kiểm tra trùng tên (không phân biệt hoa thường, bỏ khoảng trắng thừa); `excludeId` để bỏ qua chính bản ghi đang sửa |
| `bookExists(name, authorId, excludeId)` | Sách bị coi là trùng khi cùng tên và cùng tác giả |
| `available(bookId)` | Số bản còn mượn được = `amount` − số phiếu đang mượn (chưa trả) |
| `canBorrow(customerId)` | Trả `true` nếu khách đang mượn dưới 5 cuốn chưa trả |
| `lockCustomer(customerId)` / `unlockCustomer(customerId)` | Khóa / mở khóa khách rồi lưu file; trả `false` nếu không tìm thấy khách |
| `lockOverdue(customerId)` | Khóa khách nếu có phiếu chưa trả quá hạn trên 5 ngày |
| `borrow(customerId, bookId, daysBorrowed)` | Tạo phiếu mượn: ngày mượn là hôm nay, hạn trả = hôm nay + `daysBorrowed` (1–60 ngày). Từ chối nếu khách/sách không tồn tại, khách bị khóa, hết bản hoặc khách đã mượn đủ 5 cuốn |
| `returnBook(borrowId, returnDate)` | Đánh dấu phiếu đã trả + ghi ngày trả |
| `Renew(borrowId, extraDay)` | Gia hạn thêm `extraDay` ngày, tối đa 1 lần mỗi phiếu. Từ chối nếu phiếu đã trả, khách bị khóa hoặc phiếu quá hạn trên 5 ngày |
| `removeAuthor(id)` | Xóa tác giả (từ chối nếu sách đang tham chiếu tới) |
| `removeGenre(id)` | Xóa thể loại (từ chối nếu sách đang dùng) |
| `removeBook(id)` | Xóa sách (từ chối nếu đang có người mượn) |
| `removeCustomer(id)` | Xóa khách (từ chối nếu còn sách chưa trả) |

## 8. utils.h — hàm tiện ích

| Hàm | Tác dụng |
| --- | --- |
| `split(s, d)` | Tách chuỗi theo ký tự `d` → `vector<string>` |
| `trim(s)` | Bỏ khoảng trắng ở hai đầu chuỗi |
| `normalize(s)` | Bỏ khoảng trắng thừa và đổi sang chữ thường, dùng để so sánh trùng tên |
| `nextId(v)` | Trả `max(id) + 1` để sinh id mới |
| `getStr(p)` | In prompt `p` rồi đọc 1 dòng chuỗi từ bàn phím |
| `getInt(p)` | Đọc số nguyên (lặp lại cho đến khi nhập hợp lệ) |
| `clr()` | Xóa màn hình console (`system("cls")`, chỉ chạy trên Windows) |
| `pause()` | In "Enter để tiếp tục..." rồi chờ nhấn Enter |
| `header(t)` | In tiêu đề dạng `=== t ===` |

## 9. DateUtils.h — hàm xử lý ngày

Các hàm nằm trong `namespace DateUtils`, ngày viết dạng `YYYY-MM-DD`.

| Hàm | Tác dụng |
| --- | --- |
| `parseDate(date)` | Đổi chuỗi ngày thành kiểu ngày của C++20 |
| `today()` | Ngày hiện tại theo giờ Việt Nam (UTC+7), dạng chuỗi |
| `daysBetween(d1, d2)` | Số ngày từ `d1` đến `d2` |
| `daysUntil(dueDate)` | Số ngày còn lại từ hôm nay đến `dueDate` (âm nếu đã qua) |
| `isOverdue(dueDate)` | Trả `true` nếu `dueDate` đã qua |
| `overdueDays(dueDate)` | Số ngày quá hạn (0 nếu chưa quá hạn) |
| `addDays(date, n)` | Cộng thêm `n` ngày vào `date`, trả về chuỗi ngày mới |

## 10. mainLib.cpp — giao diện

| Hàm | Tác dụng |
| --- | --- |
| `printSach(b)` | In 1 dòng sách (id, tên, tác giả, số lượng, còn lại, thể loại) |
| `printBorrow(bw)` | In 1 dòng phiếu mượn (id, khách, sách, ngày mượn, hạn trả, trạng thái: đang mượn / quá hạn n ngày / đã trả, kèm số lần gia hạn) |
| `menuTacGia()` | Menu tác giả: danh sách / thêm (kiểm tra trùng tên) / sửa / xóa |
| `menuTheLoai()` | Menu thể loại: danh sách / thêm (kiểm tra trùng tên) / sửa / xóa |
| `menuKhach()` | Menu khách hàng: danh sách (có trạng thái khóa) / thêm / sửa / xóa / khóa / mở khóa |
| `menuSach()` | Menu sách: danh sách / thêm (kiểm tra tác giả tồn tại, trùng sách, số lượng không âm) / sửa / xóa / tìm kiếm |
| `menuMuonTra()` | Menu mượn/trả: đang mượn / lịch sử / mượn / trả / gia hạn |
| `main()` | Nạp dữ liệu, tự khóa các khách quá hạn trên 5 ngày, rồi chạy vòng lặp menu chính |

## 11. Đường dẫn file & format dữ liệu

Đường dẫn khai báo trong `Library.h`, tương đối từ thư mục chạy chương trình.

| File | Format mỗi dòng |
| --- | --- |
| `Data/Author.dat` | `id\|name` |
| `Data/Genre.dat` | `id\|name` |
| `Data/Customer.dat` | `id\|name\|phone\|address\|lock(0/1)` |
| `Data/Book.dat` | `id\|name\|authorId\|amount\|g1 g2 g3` |
| `Data/Borrow.dat` | `id\|customerId\|bookId\|borrowDate\|dueDate\|returnDate\|returned(0/1)\|renewCount` |
