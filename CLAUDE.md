# PBL2 — Hệ thống Quản lý Thư viện (C++)

## Build

```bash
g++ -std=c++11 main.cpp -o main.exe
```

Hoặc chạy `build.bat` cho tiện.

## Cấu trúc file

```
PBL2/
├── main.cpp        # entry point + toàn bộ console UI
├── Library.h       # class Library: load/find/borrow/return/remove
├── Entity.h        # class Entity: base (id, name) + virtual toLine()
├── Author.h        # Author   : Entity (id, name)
├── Genre.h         # Genre    : Entity (id, name)
├── Customer.h      # Customer : Entity (id, name, phone, address)
├── Book.h          # Book     : Entity (id, name, authorId, amount, genreIds)
├── Borrow.h        # Borrow            (id, customerId, bookId, dates, returned)
├── utils.h         # split, nextId, getStr, getInt, clr, pause, header
└── Data/
    ├── Author.dat
    ├── Genre.dat
    ├── Customer.dat
    ├── Book.dat
    └── Borrow.dat
```

## Kiến trúc

Một tầng duy nhất — không có Repository hay Service layer:

- `Author`, `Genre`, `Customer`, `Book` kế thừa từ `Entity` (chứa `id` + `name`). `Borrow` là bản ghi mượn/trả nên đứng độc lập.
- Mỗi class tự đảm nhiệm serialize/deserialize (`toLine` / `fromLine`) và đọc/ghi file (`load` / `save`).
- `Library` gom tất cả vector dữ liệu, xử lý logic nghiệp vụ liên bảng (kiểm tra ràng buộc khi xóa, tính số bản còn lại, mượn/trả sách).
- `main.cpp` chứa vòng lặp menu và tất cả hàm UI.

## Format dữ liệu

| File          | Format                                          |
|---------------|-------------------------------------------------|
| Author.dat    | `id\|name`                                      |
| Genre.dat     | `id\|name`                                      |
| Customer.dat  | `id\|name\|phone\|address`                      |
| Book.dat      | `id\|name\|authorId\|amount\|g1 g2 g3`          |
| Borrow.dat    | `id\|customerId\|bookId\|borrowDate\|dueDate\|returnDate\|returned(0/1)` |

## Lưu ý

- Compiler tối thiểu: **GNU C++11** (`-std=c++11`)
- Xóa Author/Genre/Book/Customer đều có kiểm tra ràng buộc tham chiếu chéo.
- `app.cpp` là file cũ (single-file architecture), có thể xóa.
