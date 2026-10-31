cmake_minimum_required(VERSION 3.16)

project(PBL2 VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

qt_standard_project_setup()

qt_add_executable(PBL2
    main.cpp
    globals.h
    MainWindow.h
    MainWindow.cpp
    MainWindow.ui
    DashboardPage.h
    DashboardPage.cpp
    DashboardPage.ui
    BorrowReturnPage.h
    BorrowReturnPage.cpp
    BorrowReturnPage.ui
    ManagementPage.h
    ManagementPage.cpp
    ManagementPage.ui
    BookEntryDialog.h
    BookEntryDialog.cpp
    BookEntryDialog.ui
    BorrowCartDialog.h
    BorrowCartDialog.cpp
    BorrowCartDialog.ui
    NameDialog.h
    NameDialog.cpp
    NameDialog.ui
    resources.qrc
)

target_link_libraries(PBL2 PRIVATE Qt6::Widgets)

# Ứng dụng GUI (không mở cửa sổ console)
set_target_properties(PBL2 PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)
