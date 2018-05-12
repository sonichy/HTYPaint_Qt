#include "mainwindow.h"
#include <QApplication>
#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QTextCodec>
#endif

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qSetMessagePattern("[ %{file}: %{line} ] %{message}");

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
    QTextCodec *tc = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForTr(tc);
    QTextCodec::setCodecForLocale(QTextCodec::codecForLocale());
    QTextCodec::setCodecForCStrings(QTextCodec::codecForLocale());
#endif

    MainWindow w;
    w.show();
    return a.exec();
}
