#include "mainwindow.h"

#include <QApplication>
#include <QScreen>

#include <QCommandLineParser>
//#include <QCommandLineOption>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QCommandLineParser parser;
    parser.setApplicationDescription("Qt Mandelbrot Example");
    parser.addHelpOption();
    parser.addVersionOption();

    MainWindow w;

    if(false){
        //QApplication inherits QGuiApplication
        qDebug() << "Number of screens:" << QGuiApplication::screens().size();
        qDebug() << "Primary screen:" << QGuiApplication::primaryScreen()->name();
        foreach (QScreen *screen, QGuiApplication::screens() ) {
            qDebug() << "manufacturer: " << screen->manufacturer() << "model: " << screen->model() << "serialNumber: " << screen->serialNumber();
            qDebug() << "Information for screen:" << screen->name();
            qDebug() << "  Available geometry:" << screen->availableGeometry().x() << screen->availableGeometry().y() << screen->availableGeometry().width() << "x" << screen->availableGeometry().height();
            qDebug() << "  Available size:" << screen->availableSize().width() << "x" << screen->availableSize().height();
            qDebug() << "  Available virtual geometry:" << screen->availableVirtualGeometry().x() << screen->availableVirtualGeometry().y() << screen->availableVirtualGeometry().width() << "x" << screen->availableVirtualGeometry().height();
            qDebug() << "  Available virtual size:" << screen->availableVirtualSize().width() << "x" << screen->availableVirtualSize().height();
            qDebug() << "  Depth:" << screen->depth() << "bits";
            qDebug() << "  devicePixelRatio:" << screen->devicePixelRatio();
            qDebug() << "  Geometry:" << screen->geometry().x() << screen->geometry().y() << screen->geometry().width() << "x" << screen->geometry().height();
            qDebug() << "  Logical DPI:" << screen->logicalDotsPerInch();
            qDebug() << "  Logical DPI X:" << screen->logicalDotsPerInchX();
            qDebug() << "  Logical DPI Y:" << screen->logicalDotsPerInchY();
            //qDebug() << "  Orientation:" << Orientation(screen->orientation());
            qDebug() << "  Orientation:" << screen->orientation();
            //nativeOrientation
            qDebug() << "  Physical DPI:" << screen->physicalDotsPerInch();
            qDebug() << "  Physical DPI X:" << screen->physicalDotsPerInchX();
            qDebug() << "  Physical DPI Y:" << screen->physicalDotsPerInchY();
            qDebug() << "  Physical size:" << screen->physicalSize().width() << "x" << screen->physicalSize().height() << "mm";
            //qDebug() << "  Primary orientation:" << Orientation(screen->primaryOrientation());
            qDebug() << "  Primary orientation:" << screen->primaryOrientation();
            qDebug() << "  Refresh rate:" << screen->refreshRate() << "Hz";
            qDebug() << "  Size:" << screen->size().width() << "x" << screen->size().height();
            qDebug() << "  Virtual geometry:" << screen->virtualGeometry().x() << screen->virtualGeometry().y() << screen->virtualGeometry().width() << "x" << screen->virtualGeometry().height();
            qDebug() << "  Virtual size:" << screen->virtualSize().width() << "x" << screen->virtualSize().height();
        }
    }

    //https://doc.qt.io/qt-6/qt.html#WindowType-enum
    //w.setWindowFlag(Qt::WindowTitleHint);
    //w.setWindowFlag(Qt::WindowStaysOnTopHint);      //helpfull while debugging
    //w.setWindowFlag(Qt::FramelessWindowHint);   //for this->setAttribute(Qt::WA_TranslucentBackground);

    w.show();
    return a.exec();
}




//// Helper function to return display orientation as a string.
//QString Orientation(Qt::ScreenOrientation orientation)
//{
//    switch (orientation) {
//        case Qt::PrimaryOrientation           : return "Primary";
//        case Qt::LandscapeOrientation         : return "Landscape";
//        case Qt::PortraitOrientation          : return "Portrait";
//        case Qt::InvertedLandscapeOrientation : return "Inverted landscape";
//        case Qt::InvertedPortraitOrientation  : return "Inverted portrait";
//        default                               : return "Unknown";
//    }
//}
