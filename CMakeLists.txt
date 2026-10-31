cmake_minimum_required(VERSION 3.16)

project(PBL2 VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

qt_standard_project_setup()

qt_add_executable(PBL2
    frontend/main.cpp
    frontend/globals.h
    frontend/MainWindow.h
    frontend/MainWindow.cpp
    frontend/MainWindow.ui
    frontend/DashboardPage.h
    frontend/DashboardPage.cpp
    frontend/DashboardPage.ui
    frontend/BorrowReturnPage.h
    frontend/BorrowReturnPage.cpp
    frontend/BorrowReturnPage.ui
    frontend/ManagementPage.h
    frontend/ManagementPage.cpp
    frontend/ManagementPage.ui
    frontend/BookEntryDialog.h
    frontend/BookEntryDialog.cpp
    frontend/BookEntryDialog.ui
    frontend/BorrowCartDialog.h
    frontend/BorrowCartDialog.cpp
    frontend/BorrowCartDialog.ui
    frontend/NameDialog.h
    frontend/NameDialog.cpp
    frontend/NameDialog.ui
    frontend/resources.qrc
    models/Entity.h
    models/Author.h
    models/Genre.h
    models/Customer.h
    models/Book.h
    models/Borrow.h
    models/List.h
    backend/Library.h
    backend/utils.h
    backend/DateUtils.h
)

target_include_directories(PBL2 PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/models
    ${CMAKE_CURRENT_SOURCE_DIR}/backend
    ${CMAKE_CURRENT_SOURCE_DIR}/frontend
)

target_link_libraries(PBL2 PRIVATE Qt6::Widgets)

# Ứng dụng GUI (không mở cửa sổ console)
set_target_properties(PBL2 PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)
