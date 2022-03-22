#include "renderarea.h"
#include <QPaintEvent>
#include <QPainter>

RenderArea::RenderArea(QWidget *parent) :
    QWidget(parent),
    mBackgroundColor(Qt::darkMagenta),  //color const ist in Qt lowercase, bei QColorConstants upper
    mShapeColor(255,255,255),
    mShape(Astroid)                     //interessant: was hier wie eine Funktion aussieht, initialisiert eine Variable (scheint normal in der Initliste)
{
    on_shape_changed();     //Initialisierung der Zeichnung
}

QSize RenderArea::minimumSizeHint() const { //recommended minimum size for the widget
    return QSize(400,300);
}

QSize RenderArea::sizeHint() const {        //return the preferred size of this item.
    return QSize(400,400);
}

void RenderArea::on_shape_changed(){
    switch(mShape){
    case Astroid:
        mScale = 80;
        mIntervalLength = 2 * M_PI;
        mStepCount = 256;
        //setBackgroundColor(Qt::darkRed);          //aus dem beispiel
        break;
    case Cycloid:
        //setBackgroundColor(QColorConstants::DarkGreen);    //meine möglichkeit
        mScale = 8;
        mIntervalLength = 6 * M_PI;
        mStepCount = 128;
        break;
    case HuygensCycloid:
        mScale = 8;
        mIntervalLength = 4 * M_PI;
        mStepCount = 256;
        //setBackgroundColor(QColorConstants::DarkYellow);
        break;
    case HypoCycloid:
        mScale = 30;
        mIntervalLength = 2 * M_PI;
        mStepCount = 256;
        //setBackgroundColor(QColorConstants::DarkBlue);
        break;
    case Line:
        mScale = 100;            //Länge
        mIntervalLength = 1;    //not really neaded
        mStepCount = 128;
        //setBackgroundColor(QColorConstants::DarkBlue);
        break;
    default:                                                //wichtig, default sollte immer gemacht werden

        setBackgroundColor(QColorConstants::DarkMagenta);    //meine möglichkeit
        break;
    }
}

QPointF RenderArea::compute(float t){

    switch(mShape){
    case Astroid:
        return compute_astroid(t);
        break;
    case Cycloid:
        return compute_cycloid(t);
        break;
    case HuygensCycloid:
        return  compute_huygens(t);
        break;
    case HypoCycloid:
        return  compute_hypo(t);
        break;
    case Line:
        return  compute_line(t);
        break;
    default:
        break;
    }
    return QPointF(0,0);
}


QPointF RenderArea::compute_astroid(float t){
    float cos_t = cos(t);
    float sin_t = sin(t);
    float x = 2 * pow(cos_t,3);
    float y = 2 * pow(sin_t,3);

    return QPointF(x,y);
}
QPointF RenderArea::compute_cycloid(float t){
    return QPointF(
                1.5*(t-sin(t)),      //Y -> getauscht
                1.5*(1+cos(t))     //X
    );
}
QPointF RenderArea::compute_huygens(float t){
    return QPointF(
                4*(3*cos(t) - cos(3*t)),    //X
                4*(3*sin(t) - sin(3*t))     //Y
    );
}
QPointF RenderArea::compute_hypo(float t){
    return QPointF(
                1.5*(2*cos(t) + cos(2*t)),  //X
                1.5*(2*sin(t) - sin(2*t))   //Y
    );
}
QPointF RenderArea::compute_line(float t){
    return QPointF( t, t );
}


void RenderArea::paintEvent(QPaintEvent *event)     //wird von Qt aufgerufen wenn nötig, protected+override im .h
{
    Q_UNUSED(event);        //deaktiviert Kompilerwarnung

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    painter.setBrush(mBackgroundColor );
    painter.setPen(mShapeColor);


    //drawing area
    painter.drawRect(this->rect() );
    QPointF center = this->rect().center();     // war im tut kein floatP, ist aber egal, konvertierung erfolgt auch automatisch
    QPointF start = center;
    QPointF fprevPixel = compute(-mIntervalLength) * mScale + center;

    float step = mIntervalLength / mStepCount;              //
    for (float t=-mIntervalLength; t < mIntervalLength; t += step){

        QPointF fpoint = compute(t) * mScale + center;

        //konvertiere Float2D zu Int(Pixel)2D, unnötig
//        QPoint pixel;
//        pixel.setX(fpoint.x() * mScale + center.x() );
//        pixel.setY(fpoint.y() * mScale + center.y() );
//        painter.drawPoint(pixel);

        if(optionCool)painter.drawLine(fpoint, start);        //das war zuerst ein Fehler im Tut, als prevPixel gefehlt hat, grad übernommen
        painter.drawLine(fpoint, fprevPixel);
        fprevPixel = fpoint;

    }
    //painter.drawLine(this->rect().topLeft(), this->rect().bottomRight() );

}









