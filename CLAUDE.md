# PBL2 — Hệ thống Quản lý Thư viện (C++)

## Build

```bash
g++ -std=c++11 -o app.exe main.cpp
```

## Cấu trúc file

```
PBL2/
├── main.cpp        # entry point + toàn bộ console UI
├── Library.h       # class Library: load/find/borrow/return/remove
├── Author.h        # struct Author   (id, name)
├── Genre.h         # struct Genre    (id, name)
├── Customer.h      # struct Customer (id, name, phone, address)
├── Book.h          # struct Book     (id, name, authorId, amount, genreIds)
├── Borrow.h        # struct Borrow   (id, customerId, bookId, dates, returned)
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

- Mỗi struct tự đảm nhiệm serialize/deserialize (`toLine` / `fromLine`) và đọc/ghi file (`load` / `save`).
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
