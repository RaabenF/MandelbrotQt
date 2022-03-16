#include "renderarea.h"
#include <QPaintEvent>
#include <QPainter>

RenderArea::RenderArea(QWidget *parent) :
    QWidget(parent),
    mBackgroundColor(Qt::darkMagenta),  //color const ist in Qt lowercase, bei QColorConstants upper
    mShapeColor(255,255,255),
    mShape(Astroid)                     //interessant: was hier wie eine Funktion aussieht, initialisiert eine Variable (scheint normal in der Initliste)
{
}

QSize RenderArea::minimumSizeHint() const { //recommended minimum size for the widget
    return QSize(100,100);
}

QSize RenderArea::sizeHint() const {        //return the preferred size of this item.
    return QSize(400,200);
}

void RenderArea::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);        //deaktiviert Kompilerwarnung

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    switch(mShape){
    case Astroid:
        setBackgroundcolor(Qt::darkRed);          //aus dem beispiel
        break;
    case Cycloid:
        setBackgroundcolor(QColorConstants::DarkGreen);    //meine möglichkeit
        break;
    case HuygensCycloid:
        setBackgroundcolor(QColorConstants::DarkYellow);
        break;
    case HypoCycloid:
        setBackgroundcolor(QColorConstants::DarkBlue);
        break;
    default:            //wichtig
        setBackgroundcolor(QColorConstants::DarkMagenta);
        break;

    }

    painter.setBrush(mBackgroundColor );
    painter.setPen(mShapeColor);

    //drawing area
    painter.drawRect(this->rect() );
    painter.drawLine(this->rect().topLeft(), this->rect().bottomRight() );

}
