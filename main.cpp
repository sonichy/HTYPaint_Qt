#include "mainwindow.h"
#include <QApplication>
#include <QDebug>
#include <QStyleFactory>

int main(int argc, char *argv[])
{
    qSetMessagePattern("[ %{file}: %{line} ] %{message}");
    qDebug() << QStyleFactory::keys();
    QApplication a(argc, argv);
    //QApplication::setStyle(QStyleFactory::create("Fusion"));
    //a.setStyle(QStyleFactory::create("Fusion"));
    MainWindow w;    
    w.show();
    return a.exec();
}