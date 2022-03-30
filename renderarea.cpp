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
    shapestore.append(paramShape(8,"Tilde",55,M_PI,256) );
    shapestore.append(paramShape(9,"Mandel Brot",10, 3,256) );   //interval empfohlen: -3..3/y=i=-2..2
    shapestore.append(paramShape(10,"tst",30,M_PI,256) );
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
         .name=name,            //, function_name;
         .prescale=preScale,
         .interval=interval,    //Length; //8, M_PI;
         .steps=steps           //Count;
    };
    //alt (convenience) menüliste:
    ShapeList.append(name);

    return sdata;
}

unsigned int RenderArea::setShape (unsigned int row){
    if (shapestore.length() > row ){    //length starts@ 1, row @ 0
        if(row == shapestore[row].id) mShapeIndex = row;              //or id
        else{
            qDebug("shapestore ID failure");
            mShapeIndex = 0;
        }
        mPreScale = shapestore[mShapeIndex].prescale;
        mIntervalLength = shapestore[mShapeIndex].interval;
        mStepCount = shapestore[mShapeIndex].steps;
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

unsigned int RenderArea::getShapeIDbyName(QString name){
    for (int i=0; shapestore.length() > i; i++ ){    //length starts@ 1, row @ 0
        if(!shapestore[i].name.compare(name,Qt::CaseInsensitive) ) {
            return shapestore[i].id;
        }
    }
    return 0;
}


QPointF RenderArea::compute(float t, float * pFloatIter1, std::complex<double> *compval){

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
        return compute_tilde(t, pFloatIter1);
        break;
    case 9:
        return compute_mandelb(t, pFloatIter1, compval);
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
QPointF RenderArea::compute_tilde(float t,  float * pFloatIter1){
    return QPointF( t + sin(t), 0.5*t + cos(*pFloatIter1) );
}
QPointF RenderArea::compute_mandelb(float t,  float * pFloatIter1, std::complex<double> *lastCval){
    std::complex<double> Xval(t,1);      //include <complex>

    //*lastCval = std::complex<double>(t, 1);     //equals the Complex Number real=t * 1imag
    // x1 =  (x0)² + C
    for(int i=0; i<1; i++){
        //*lastCval = ( *lastCval * *lastCval );
        Xval *= Xval;
        *lastCval *= Xval;  //this is  +C
    }

    QPointF endv( t, lastCval->real() );    //, lastCval->imag() );
    //qDebug() << endv.x() << endv.y();
    return endv;
}

//return QPointF( t + sin(*pFloatIter1), t + cos(*pFloatIter1) );


void RenderArea::paintEvent(QPaintEvent *event)     //wird von Qt aufgerufen wenn nötig, protected+override im .h
{
    Q_UNUSED(event);        //deaktiviert Kompilerwarnung

    QPainter painter(this);

    bool drawLine = true;   //default true
    if(mShapeIndex == getShapeIDbyName("mandel brot") ){
        drawLine = false;
        painter.setRenderHint(QPainter::Antialiasing, false);
        mPen.setWidth(1);
        mPen.setColor(Qt::blue);
    }
    else {
        painter.setRenderHint(QPainter::Antialiasing, true);
        mPen.setWidth(2);
        mPen.setColor(Qt::white);
    }
    painter.setBrush(mBackgroundColor );    //brush defines how shapes are filled
    painter.setPen(mPen);   //ehem mShapeColor


    //drawing area
    painter.drawRect(this->rect() );
    QPointF center = this->rect().center();     // war im tut kein floatP, ist aber egal, konvertierung erfolgt auch automatisch
    float step = mIntervalLength / mStepCount;
    float tIntervLength = mIntervalLength + step;
    float tScale = mPreScale * mScale/100;

    float *pFloatIter1 = new float(0);
    //QPointF *lastFV = new QPointF(0,0);
    std::complex<double> *lastCval = new std::complex<double>(0,0);      //include <complex>

    //painter.drawLine(this->rect().topLeft(), this->rect().bottomRight() );
    //for (float t=0; t < tIntervLength; t += step){


    float t2=-tIntervLength;
    if (drawLine){
        t2= tIntervLength - step;
    }
    else{
        t2= t2*2/3;
    }
    QPointF fprevPixel = compute(-tIntervLength, &t2, lastCval) * tScale + center;    //first point
    while(t2 < tIntervLength){
        for (float t=-tIntervLength; t < tIntervLength; t += step){
            QPointF fpoint = compute(t, &t2, lastCval) * tScale + center;


            if(drawLine){
                if(optionCool)painter.drawLine(fpoint, center);        //das war zuerst ein Fehler im Tut, als prevPixel gefehlt hat, grad übernommen
                painter.drawLine(fpoint, fprevPixel);
                fprevPixel = fpoint;
            }
            else{   //konvertiere Float2D zu Int(Pixel)2D, unnötig
                //        QPoint pixel;
                //        pixel.setX(fpoint.x() * tScale + center.x() );
                //        pixel.setY(fpoint.y() * tScale + center.y() );
                painter.drawPoint(fpoint);   //pixel);
            }
            //*lastFV
            *pFloatIter1 += 1;
        }
        t2 += step;
    }

    delete pFloatIter1;
    //delete lastFV;        //unnötig bei Qt?
    delete lastCval;
    //durchläufe for() interval(256*2)+0+anfang+ende; 515
    //GESAMT DURCHLÄUFE 5, evtl wegen oversampling?
}









