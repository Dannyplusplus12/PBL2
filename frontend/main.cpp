#include <QApplication>
#include <QDir>
#include <QFile>
#include <QDebug>
#include <QPalette>
#include <QStyleHints>
#include <QColor>

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

    // Ép toàn bộ app chạy theme SÁNG, bất chấp Windows có đang bật Dark mode hay không.
    QApplication::styleHints()->setColorScheme(Qt::ColorScheme::Light);
    app.setStyle(QStringLiteral("Fusion"));
    {
        QPalette p;
        p.setColor(QPalette::Window,            QColor(248, 250, 252));
        p.setColor(QPalette::WindowText,        QColor(15, 23, 42));
        p.setColor(QPalette::Base,              QColor(255, 255, 255));
        p.setColor(QPalette::AlternateBase,     QColor(248, 250, 252));
        p.setColor(QPalette::ToolTipBase,       QColor(255, 255, 255));
        p.setColor(QPalette::ToolTipText,       QColor(15, 23, 42));
        p.setColor(QPalette::Text,              QColor(15, 23, 42));
        p.setColor(QPalette::Button,            QColor(255, 255, 255));
        p.setColor(QPalette::ButtonText,        QColor(15, 23, 42));
        p.setColor(QPalette::BrightText,        QColor(239, 68, 68));
        p.setColor(QPalette::Link,              QColor(99, 102, 241));
        p.setColor(QPalette::Highlight,         QColor(99, 102, 241));
        p.setColor(QPalette::HighlightedText,   QColor(255, 255, 255));
        p.setColor(QPalette::PlaceholderText,   QColor(148, 163, 184));
        p.setColor(QPalette::Disabled, QPalette::Text,        QColor(148, 163, 184));
        p.setColor(QPalette::Disabled, QPalette::ButtonText,  QColor(148, 163, 184));
        app.setPalette(p);
    }

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
