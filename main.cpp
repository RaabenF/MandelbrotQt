#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    //https://doc.qt.io/qt-6/qt.html#WindowType-enum
    //w.setWindowFlag(Qt::WindowTitleHint);
    w.setWindowFlag(Qt::WindowStaysOnTopHint);      //helpfull while debugging
    //w.setWindowFlag(Qt::FramelessWindowHint);   //for this->setAttribute(Qt::WA_TranslucentBackground);

    w.show();
    return a.exec();
}
