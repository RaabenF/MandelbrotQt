#include "renderarea.h"
#include <QPaintEvent>
#include <QPainter>

RenderArea::RenderArea(QWidget *parent) :
    QWidget(parent),
      //init list:
    mBackgroundColor(Qt::darkBlue),
    mIntervalLength(1), mPreScale(1), mStepCount(8), optionCool(false)
{
    mPen.setWidth(2);
    mPen.setColor(Qt::white);

//test, geht nicht wg kein append()
//    shapetest[0]={     //Qlist(dynamic array) - vom struct
//                    .id=999,
//                    .name="dfg",    //, function_name;
//                    .sPreScale=1,
//                    .interval=1,     //Length; //8, M_PI;
//                    .steps=1        //Count;
//                };
//    shapetest[0]=paramShape(0,"test",10,M_PI,16);

//    for(int i=0,i<99,i++){
//        shapestore[i]=
//    }
    shapestore.append(paramShape(0,"Astroid",73,M_PI,256) );
    shapestore.append(paramShape(1,"Cycloid",11,6 * M_PI,128) );
    shapestore.append(paramShape(2,"HygensCycloid",35,2*M_PI,256) );
    shapestore.append(paramShape(3,"HypoCycloid",55,M_PI,256) );
    shapestore.append(paramShape(4,"Elipse",100,M_PI,128) );
    shapestore.append(paramShape(5,"Mandala",8,6*M_PI,512) );
    shapestore.append(paramShape(6,"Star",20,3*M_PI,256) );
    shapestore.append(paramShape(7,"Cloud",10,14*M_PI,128) );
    shapestore.append(paramShape(8,"Mandel Brot",10,M_PI,256) );
    shapestore.append(paramShape(9,"tst",30,M_PI,256) );
    //shapestore.append(paramShape(,"",10,M_PI,256) );      //copy me

}

QSize RenderArea::minimumSizeHint() const { //recommended minimum size for the widget
    return QSize(400,300);
}

QSize RenderArea::sizeHint() const {        //return the preferred size of this item.
    return QSize(400,400);
}

RenderArea::ShapeType RenderArea::paramShape(unsigned int id, QString name, float preScale, float interval, int steps ){
    ShapeType sdata = {     //Qlist(dynamic array) - vom struct
         .id=id,
         .name=name,    //, function_name;
         .sPreScale=preScale,
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
        mPreScale = shapestore[row].sPreScale;
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
//        mPreScale = 80;
//        mIntervalLength = M_PI; //2 * M_PI;
//        mStepCount = 256;
//        setBackgroundColor(QColorConstants::DarkYellow);
//        break;

QPointF RenderArea::compute(float t, float * pFloatIter1){

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
        return compute_mandala(t);
        break;
    case 6:
        return compute_star(t);
        break;
    case 7:
        return compute_cloud(t);
        break;
    case 8:
        return compute_mandelb(t, pFloatIter1);
        break;
    default:
        return  compute_line(t);
        break;
    }
    return QPointF(0,0);
}


QPointF RenderArea::compute_astroid(float t){
    float x = 2 * pow(cos(t),3);
    float y = 2 * pow(sin(t),3);

    return QPointF(x,y);
}
QPointF RenderArea::compute_cycloid(float t){
    return QPointF(
                (t-sin(t)),    //X
                (1+cos(t))     //Y
    );
}
QPointF RenderArea::compute_huygens(float t){
    return QPointF(
                (3*cos(t) - cos(3*t)),
                (3*sin(t) - sin(3*t))
    );
}
QPointF RenderArea::compute_hypo(float t){
    return QPointF(
                (2*cos(t) + cos(2*t)),
                (2*sin(t) - sin(2*t))
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
QPointF RenderArea::compute_mandala(float t){
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
QPointF RenderArea::compute_mandelb(float t,  float * pFloatIter1){
    return QPointF( t + sin(t* *pFloatIter1), t + cos(*pFloatIter1) );
}
//return QPointF( t + sin(*pFloatIter1), t + cos(*pFloatIter1) );


void RenderArea::paintEvent(QPaintEvent *event)     //wird von Qt aufgerufen wenn nötig, protected+override im .h
{
    Q_UNUSED(event);        //deaktiviert Kompilerwarnung

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    painter.setBrush(mBackgroundColor );    //brush defines how shapes are filled
    painter.setPen(mPen);   //ehem mShapeColor

    float *pFloatIter1 = new float(1);

    //drawing area
    painter.drawRect(this->rect() );
    QPointF center = this->rect().center();     // war im tut kein floatP, ist aber egal, konvertierung erfolgt auch automatisch
    float step = mIntervalLength / mStepCount;
    float tIntervLength = mIntervalLength + step;
    float tScale = mPreScale * mScale/100;

    if(mShapeIndex == 99){
        QPointF fprevPixel = compute(0, pFloatIter1) * tScale + center;
        for (float t=0; t < tIntervLength; t += step){
            QPointF fpoint = compute(t, pFloatIter1) * tScale + center;
            if(optionCool)painter.drawLine(fpoint, center);        //effekt
            painter.drawLine(fpoint, fprevPixel);
            fprevPixel = fpoint;
            *pFloatIter1 += 1;
        }
    }
    else{
        QPointF fprevPixel = compute(-tIntervLength, pFloatIter1) * tScale + center;    //first point
        for (float t=-tIntervLength; t < tIntervLength; t += step){
            QPointF fpoint = compute(t, pFloatIter1) * tScale + center;
            //konvertiere Float2D zu Int(Pixel)2D, unnötig
        //        QPoint pixel;
        //        pixel.setX(fpoint.x() * tScale + center.x() );
        //        pixel.setY(fpoint.y() * tScale + center.y() );
        //        painter.drawPoint(pixel);
            if(optionCool)painter.drawLine(fpoint, center);        //das war zuerst ein Fehler im Tut, als prevPixel gefehlt hat, grad übernommen
            painter.drawLine(fpoint, fprevPixel);
            fprevPixel = fpoint;
            *pFloatIter1 += 1;
        }
        //painter.drawLine(this->rect().topLeft(), this->rect().bottomRight() );
    }
    delete pFloatIter1;        //unnötig bei Qt?
    //durchläufe interval(256*2)+0+anfang+ende; 515

}









