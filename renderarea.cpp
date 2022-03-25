#include "renderarea.h"
#include <QPaintEvent>
#include <QPainter>

RenderArea::RenderArea(QWidget *parent) :
    QWidget(parent),
    mIntervalLength(1), mScale(1), mStepCount(8), optionCool(false)  //init
{
    setBackgroundColor(Qt::darkCyan);   //hier oder in init liste, egal
    mPen.setWidth(2);
    mPen.setColor(Qt::white);

//test, geht nicht wg kein append()
//    shapetest[0]={     //Qlist(dynamic array) - vom struct
//                    .id=999,
//                    .name="dfg",    //, function_name;
//                    .scale=1,
//                    .interval=1,     //Length; //8, M_PI;
//                    .steps=1        //Count;
//                };
//    shapetest[0]=paramShape(0,"test",10,M_PI,16);

//    for(int i=0,i<99,i++){
//        shapestore[i]=
//    }
    shapestore.append(paramShape(0,"Astroid",80,M_PI,256) );
    shapestore.append(paramShape(1,"Cycloid",10,6 * M_PI,128) );
    shapestore.append(paramShape(2,"HygensCycloid",32,2*M_PI,256) );
    shapestore.append(paramShape(3,"HypoCycloid",60,M_PI,256) );
    shapestore.append(paramShape(4,"Elipse",80,M_PI,128) );
    shapestore.append(paramShape(5,"Fancy",9,6*M_PI,512) );
    shapestore.append(paramShape(6,"Star",25,3*M_PI,256) );
    shapestore.append(paramShape(7,"Cloud",10,14*M_PI,128) );
    shapestore.append(paramShape(8,"tst",10,M_PI,256) );
    //shapestore.append(paramShape(,"",10,M_PI,256) );

}

QSize RenderArea::minimumSizeHint() const { //recommended minimum size for the widget
    return QSize(400,300);
}

QSize RenderArea::sizeHint() const {        //return the preferred size of this item.
    return QSize(400,400);
}

RenderArea::ShapeType RenderArea::paramShape(unsigned int id, QString name, float scale, float interval, int steps ){
    ShapeType sdata = {     //Qlist(dynamic array) - vom struct
         .id=id,
         .name=name,    //, function_name;
         .scale=scale,
         .interval=interval,     //Length; //8, M_PI;
         .steps=steps        //Count;
    };
    //alt (convenience) menüliste:
    ShapeList.append(name);

    return sdata;
}

int RenderArea::setShape (int row){
    if (shapestore.length()> row ){     //length starts@ 1, row @ 0
        mShapeIndex = row;              //or id
        mScale = shapestore[row].scale;
        mIntervalLength = shapestore[row].interval;
        mStepCount = shapestore[row].steps;
        //setBackgroundColor(QColorConstants::DarkYellow);
    }
    else{
        qDebug() << "shapelist index out of range, no menu";
        return shapestore.length();
    }
    repaint();
    return 0;
}

//    default:                                                //wichtig, default sollte immer gemacht werden
//        mScale = 80;
//        mIntervalLength = M_PI; //2 * M_PI;
//        mStepCount = 256;
//        setBackgroundColor(QColorConstants::DarkYellow);
//        break;

QPointF RenderArea::compute(float t){

    switch(mShapeIndex){
    case 0:
        return compute_astroid(t);
        break;
    case 1:
        return compute_cycloid(t);
        break;
    case 2:
        return  compute_huygens(t);
        break;
    case 3:
        return  compute_hypo(t);
        break;
    case 4:
        return compute_elipse(t);
        break;
    case 5:
        return compute_fancy(t);
        break;
    case 6:
        return compute_star(t);
        break;
    case 7:
        return compute_cloud(t);
        break;
    default:
        return  compute_line(t);
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
                (t-sin(t)),    //Y -> getauscht
                (1+cos(t))     //X
    );
}
QPointF RenderArea::compute_huygens(float t){
    return QPointF(
                (3*cos(t) - cos(3*t)),    //X
                (3*sin(t) - sin(3*t))     //Y
    );
}
QPointF RenderArea::compute_hypo(float t){
    return QPointF(
                (2*cos(t) + cos(2*t)),  //X
                (2*sin(t) - sin(2*t))   //Y
    );
}
QPointF RenderArea::compute_line(float t){
    return QPointF( t, t );
}
QPointF RenderArea::compute_circle(float t){
    return QPointF( cos(t), sin(t) );
}
QPointF RenderArea::compute_elipse(float t){
    return QPointF( 2*cos(t), sin(t) );
}
QPointF RenderArea::compute_fancy(float t){
    return QPointF( 11.0*cos(t) - 6.0*cos(11.0/6.0*t),      //ohne komma rechnet er tatsächlich in INT!!! trotz cos
                    11.0*sin(t) - 6.0*sin(11.0/6.0*t) );
}
QPointF RenderArea::compute_star(float t){
    const float R=5, r=3, d=5;
    return QPointF( (R-r)*cos(t) + d*cos(t*((R-r)/r) ),
                    (R-r)*sin(t) - d*sin(t*((R-r)/r) ) );
}
QPointF RenderArea::compute_cloud(float t){
    float a=14, b=1, sign=-1;
    float x = (a-b) * cos(t*b/a) + sign*b*cos(t* (a+b) /a);
    float y = (a-b) * sin(t*b/a) + sign*b*sin(t* (a+b) /a);
    return QPointF( x, y );
}

void RenderArea::paintEvent(QPaintEvent *event)     //wird von Qt aufgerufen wenn nötig, protected+override im .h
{
    Q_UNUSED(event);        //deaktiviert Kompilerwarnung

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    painter.setBrush(mBackgroundColor );
    painter.setPen(mPen);   //ehem mShapeColor


    //drawing area
    painter.drawRect(this->rect() );
    QPointF center = this->rect().center();     // war im tut kein floatP, ist aber egal, konvertierung erfolgt auch automatisch
    QPointF start = center;
    float step = mIntervalLength / mStepCount;
    float tempInterval = mIntervalLength + step;

    if(mShapeIndex == 99){      //mandel=test
        QPointF fprevPixel = compute(0) * mScale + center;
        for (float t=0; t < tempInterval; t += step){
            QPointF fpoint = compute(t) * mScale + center;
            if(optionCool)painter.drawLine(fpoint, start);        //effekt
            painter.drawLine(fpoint, fprevPixel);
            fprevPixel = fpoint;
        }
    }
    else{
        QPointF fprevPixel = compute(-tempInterval) * mScale + center;
        for (float t=-tempInterval; t < tempInterval + step; t += step){
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
}









