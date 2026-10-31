#include <QApplication>
#include <QDir>
#include <QFile>
#include <QDebug>

#include "globals.h"
#include "MainWindow.h"

Library g_lib;

// Tìm thư mục chứa "Data" bằng cách đi ngược lên từ thư mục chứa file chạy.
// Qt Creator thường chạy ở thư mục build, còn dữ liệu nằm ở thư mục nguồn.
static QString findDataRoot() {
    QString dir = QCoreApplication::applicationDirPath();
    for (int i = 0; i < 8; ++i) {
        if (QDir(dir + "/Data").exists())
            return dir;
        QDir up(dir);
        up.cdUp();
        QString parent = up.absolutePath();
        if (parent == dir)
            break;
        dir = parent;
    }
    return QString();
}

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    QCoreApplication::setApplicationName("He thong quan ly thu vien");

    const QString root = findDataRoot();
    if (!root.isEmpty())
        QDir::setCurrent(root);
    else
        qWarning() << "Khong tim thay thu muc Data (du lieu se khong nap duoc)";

    // Nạp dữ liệu từ các file .dat
    g_lib.load();

    // Tự khóa các khách quá hạn > 5 ngày (giống bản console)
    for (int i = 0; i < (int)g_lib.customers.size(); ++i)
        g_lib.lockOverdue(g_lib.customers[i].getId());

    // Nạp stylesheet
    QFile f(":/styles.qss");
    if (f.open(QIODevice::ReadOnly | QIODevice::Text)) {
        app.setStyleSheet(QString::fromUtf8(f.readAll()));
        f.close();
    }

    MainWindow w;
    w.show();
    return app.exec();
}
