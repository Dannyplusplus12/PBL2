# PBL2 — Hệ thống Quản lý Thư viện (Qt 6)

Ứng dụng chuyển từ giao diện console sang **Qt 6 Widgets**, dùng **CMake**.

## Cấu trúc
- Tầng nghiệp vụ (giữ nguyên, không đổi model): `Entity.h`, `Author.h`, `Genre.h`, `Customer.h`, `Book.h`, `Borrow.h`, `List.h`, `DateUtils.h`, `Library.h`.
- Giao diện Qt (mới): `MainWindow`, `DashboardPage` (Tổng quan), `BorrowReturnPage` (Mượn trả), `ManagementPage` (Quản lý) + các dialog `BookEntryDialog`, `BorrowCartDialog`, `NameDialog`.
- Mỗi màn hình có file `.ui` tương ứng → mở trong **Qt Creator → Design** để kéo-thả chỉnh sửa.

## Cách mở & chạy (Qt Creator)
1. Mở **Qt Creator** → *File → Open File or Project…* → chọn `CMakeLists.txt`.
2. Chọn **Kit** là Qt `6.12.0` MinGW 64-bit (hoặc kit Qt6 bất kỳ có sẵn).
3. Bấm **Run** (Ctrl+R).

> Đường dẫn dữ liệu `Data/*.dat` được tự định vị trong `main.cpp` (đi ngược lên từ thư mục chứa file chạy). Nếu đổi cấu trúc thư mục, có thể đặt *Projects → Run → Working directory* về `d:\Dev\PBL2`.

## Build bằng dòng lệnh (MinGW)
```powershell
cmake -S . -B build -G Ninja `
  -DCMAKE_PREFIX_PATH=C:/Qt/6.12.0/mingw_64 `
  -DCMAKE_CXX_COMPILER=C:/Qt/Tools/mingw1310_64/bin/g++.exe `
  -DCMAKE_MAKE_PROGRAM=C:/Qt/Tools/Ninja/ninja.exe
cmake --build build
```

## 3 màn hình chính
| Màn hình | Chức năng |
|---|---|
| **Tổng quan** | 4 card thống kê; bấm card "Quá hạn" để xem danh sách khách quá hạn + sách quá hạn |
| **Mượn trả** | Danh sách khách (tìm theo tên/SĐT, khách quá hạn nền đỏ) + chi tiết khách + trả/mượn sách (giỏ hàng) + thêm/sửa/xóa/khóa khách |
| **Quản lý** | Tab Sách (nhập/xuất kho thông minh, sửa, xóa), Tác giả, Thể loại |

