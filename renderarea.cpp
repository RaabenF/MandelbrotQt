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

QPointF RenderArea::compute_astroid(float t){
    float cos_t = cos(t);
    float sin_t = sin(t);
    float x = 2 * pow(cos_t,3);
    float y = 2 * pow(sin_t,3);

    return QPointF(x,y);
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
    default:                                                //wichtig, sollte immer gemacht werden
        setBackgroundcolor(QColorConstants::DarkMagenta);
        break;

    }

    painter.setBrush(mBackgroundColor );
    painter.setPen(mShapeColor);


    //drawing area
    painter.drawRect(this->rect() );

    QPoint center = this->rect().center();
    int stepCount = 256;
    float scale = 80;

    float intervalLength = 2 * M_PI;
    float step = intervalLength / stepCount;
    for (float t=0; t < intervalLength; t += step){
        QPointF fpoint = compute_astroid(t);

        QPoint pixel;
        pixel.setX(fpoint.x() * scale + center.x() );
        pixel.setY(fpoint.y() * scale + center.y() );

        painter.drawPoint(pixel);
    }
    //painter.drawLine(this->rect().topLeft(), this->rect().bottomRight() );

}









