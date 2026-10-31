#pragma once
#include <QString>
#include "Library.h"

// Đối tượng Library dùng chung cho toàn bộ GUI (định nghĩa trong main.cpp)
extern Library g_lib;

// Chuyển đổi std::string <-> QString (dữ liệu là UTF-8)
inline QString qs(const std::string& s) { return QString::fromStdString(s); }
inline std::string ss(const QString& s) { return s.toStdString(); }
