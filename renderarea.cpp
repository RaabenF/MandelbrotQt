#include "renderarea.h"
#include <QPaintEvent>
#include <QPainter>
#include "mainwindow.h"

RenderArea::RenderArea(QWidget *parent) :
    QWidget(parent)
      //init list:
    ,mBackgroundColor(Qt::darkBlue)
    ,mShapeColor(255, 255, 255)
    ,mIntervalLength(1), mPreScale(1), mStepCount(8), optionCool(false)
{
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
    //                          ID,Name     Prescale,Interval,Steps
    shapestore.append(paramShape(0,"Astroid",73,M_PI,256) );
    shapestore.append(paramShape(1,"Cycloid",11,6 * M_PI,128) );
    shapestore.append(paramShape(2,"HygensCycloid",35,2*M_PI,256) );
    shapestore.append(paramShape(3,"HypoCycloid",55,M_PI,256) );
    shapestore.append(paramShape(4,"Elipse",100,M_PI,128) );
    shapestore.append(paramShape(5,"Mandala",8,6*M_PI,512) );
    shapestore.append(paramShape(6,"Star",20,3*M_PI,256) );
    shapestore.append(paramShape(7,"Cloud",10,14*M_PI,128) );
    shapestore.append(paramShape(8,"Tilde",55,M_PI,256,0,0) );
    shapestore.append(paramShape(9,"Mandel Brot",1, 3, 4, -100) );   //interval empfohlen: -3..3    steps müsste count(pixel) sein?
    shapestore.append(paramShape(10,"tst",30,M_PI,256) );
    //shapestore.append(paramShape(,"",10,M_PI,256) );      //copy me

    if(this->hasMouseTracking() ){
        this->setMouseTracking(true);
        qDebug("mousetracking on");
    }else qDebug("\nmousetracking not available\n");
}
RenderArea::~RenderArea(){
    delete mappainter;  //delete in this order
    delete shapemap;
}

QSize RenderArea::minimumSizeHint() const { //recommended minimum size for the widget
    return QSize(400,300);
}

QSize RenderArea::sizeHint() const {        //return the preferred size of this item.
    return QSize(400,400);
}

void RenderArea::resizeEvent(QResizeEvent *event){
    //called before paintEvent  |   event->oldSize();
    if(!mDrawLine){
        delete mappainter;  //delete in this order
        delete shapemap;
        shapemap = new QPixmap(event->size() );
        mappainter = new QPainter(shapemap);
        plotDrawer(this->mappainter);
    }
}

void RenderArea::mousePressEvent(QMouseEvent *event){
    if (event->button() == Qt::LeftButton) {
        mMouseOldPos = event->pos();   //position().toPoint();
    }
}
void RenderArea::mouseMoveEvent(QMouseEvent *event){
    if (!mMouseOldPos.isNull() ){   //event->button() == Qt::LeftButton) {
        mTempMove -= event->pos() - mMouseOldPos;
        if (mTempMove.manhattanLength() > 2){   //movement treshold
            if(!mDrawLine){
                plotDrawer(this->mappainter);
                update();
            }
        }
        mMouseOldPos = event->pos();    //globalPosition();
    }
}
void RenderArea::mouseReleaseEvent(QMouseEvent *event){

}

RenderArea::ShapeType RenderArea::paramShape(unsigned int id, QString name, float preScale, float interval, int steps, float Xoffset, float Yoffset ){
    ShapeType sdata = {     //Qlist(dynamic array) - vom struct
         .id=id,
         .name=name,            //, function_name;
         .prescale=preScale,
         .interval=interval,    //Length; //8, M_PI;
         .steps=steps,           //Count;
         .Xoffset=Xoffset,
         .Yoffset=Yoffset
    };
    ShapeList.append(name);
    return sdata;
}
//    default:                                                //wichtig, default sollte immer gemacht werden
//        mPreScale = 80;
//        mIntervalLength = M_PI; //2 * M_PI;
//        mStepCount = 256;
//        setBackgroundColor(QColorConstants::DarkYellow);
//        break;


unsigned int RenderArea::setShape (unsigned int row){
    if (shapestore.length() > row ){    //length starts@ 1, row @ 0
        if(row != shapestore[row].id){
            qDebug("shapestore ID failure");
            mShapeIndex = 0;
        }
        mShapeIndex = row;        //setter
        mPreScale = shapestore[mShapeIndex].prescale;
        mIntervalLength = shapestore[mShapeIndex].interval;
        mStepCount = shapestore[mShapeIndex].steps;
        mTempMove = QPoint(0,0);
        mXoffset=0, mYoffset=0;
    }
    else{
        qDebug() << "shapelist index out of range, no menu";
        return shapestore.length();                             //return failure
    }
    if(mShapeIndex >= getShapeIDbyName("mandel brot") ){
        mDrawLine = false;

        shapemap = new QPixmap(this->size() );    //inherits paintdevice
        mappainter = new QPainter(shapemap);
    } else mDrawLine = true;
    repaint();
    return 0;   //return success
}

unsigned int RenderArea::getShapeIDbyName(QString name){
    for (int i=0; shapestore.length() > i; i++ ){    //length starts@ 1, row @ 0
        if(!shapestore[i].name.compare(name,Qt::CaseInsensitive) ) {
            return shapestore[i].id;
        }
    }
    return 0;   //0 is failure or standartvalue
}

void RenderArea::updatePixmap(){
    if(mShapeIndex >= getShapeIDbyName("mandel brot") ){
        plotDrawer(this->mappainter);       //repaint
    }
}


QPointF RenderArea::compute(float x){
    switch(mShapeIndex){
    case 0:
        return compute_astroid(x);
        break;
    case 1:
        return compute_cycloid(x);
        break;
    case 2:
        return  compute_huygens(x);
        break;
    case 3:
        return  compute_hypo(x);
        break;
    case 4:
        return compute_elipse(x);
        break;
    case 5:
        return compute_mandala(x);
        break;
    case 6:
        return compute_star(x);
        break;
    case 7:
        return compute_cloud(x);
        break;
    case 8:
        return compute_tilde(x);
        break;
    default:
        return  compute_line(x);
        break;
    }
    return QPointF(0,0);
}
QPointF RenderArea::compute(float x, float y){
    switch(mShapeIndex){
    case 9:
        return compute_mandelb(x, y);
        break;
    default:
        return  compute_line(x);
        break;
    }
    return QPointF(0,0);
}
QPoint RenderArea::compute(int x, int y){
    switch(mShapeIndex){
    case 9:
        return compute_mandelb(x, y);
        break;
    default:
        return QPoint(0,0);
        break;
    }
    return QPoint(0,0);
}

QPointF RenderArea::compute_astroid(float x){
    float xout = 2 * pow(cos(x),3);
    float yout = 2 * pow(sin(x),3);

    return QPointF(xout,yout);
}
QPointF RenderArea::compute_cycloid(float x){
    return QPointF(
                (x-sin(x)),    //X
                (1+cos(x))     //Y
    );
}
QPointF RenderArea::compute_huygens(float x){
    return QPointF(
                (3*cos(x) - cos(3*x)),
                (3*sin(x) - sin(3*x))
    );
}
QPointF RenderArea::compute_hypo(float x){
    return QPointF(
                (2*cos(x) + cos(2*x)),
                (2*sin(x) - sin(2*x))
    );
}
QPointF RenderArea::compute_line(float x){
    return QPointF( x, x );
}
QPointF RenderArea::compute_circle(float x){
    return QPointF( cos(x), sin(x) );
}
QPointF RenderArea::compute_elipse(float x){
    return QPointF( 2*cos(x), sin(x) );
}
QPointF RenderArea::compute_mandala(float x){
    return QPointF( 11.0*cos(x) - 6.0*cos(11.0/6.0*x),      //ohne komma rechnet er tatsächlich in INT!!! trotz cos
                    11.0*sin(x) - 6.0*sin(11.0/6.0*x) );
}
QPointF RenderArea::compute_star(float x){
    const float R=5, r=3, d=5;
    return QPointF( (R-r)*cos(x) + d*cos(x*((R-r)/r) ),
                    (R-r)*sin(x) - d*sin(x*((R-r)/r) ) );
}
QPointF RenderArea::compute_cloud(float x){
    float a=14, b=1, sign=-1;
    float xout = (a-b) * cos(x*b/a) + sign*b*cos(x* (a+b) /a);
    float yout = (a-b) * sin(x*b/a) + sign*b*sin(x* (a+b) /a);
    return QPointF( xout, yout );
}
//X-Y plots
QPointF RenderArea::compute_tilde(float x){
    return QPointF( x + sin(x), 0.5*x + cos(100*x) );
}
QPointF RenderArea::compute_mandelb(float x,  float y){  //, std::complex<double> *lastXval){
    //*lastXval = std::complex<double>(x, y);     //equals the Complex Number real=t * 1imag        #include <complex>
    std::complex<float> Xvar(0,0);
    std::complex<float> Cvar(x,y);      //i think float is sufficient

    // X(i) = (X0)² + C
    for(int i=0; i<16; i++){
        Xvar = Xvar * Xvar;     //Xval = std::pow(Xval,2);
        Xvar += Cvar;           //X+C
        //if(Xvar.real() > 0xFFFFFF){
        if(std::isinf( Xvar.real()) ){          //break at infinity
            QPointF endv( Xvar.real(), 0  );    //return Complex Value in fpoint
            return endv;
        }
    }
    QPointF endv( Xvar.real(), Xvar.imag()  );    //return Complex Value in fpoint
    return endv;
}

QPoint RenderArea::compute_mandelb(int x,  int y){  //only float double and longdouble are guaranteed in std::complex
    float a=x, b=y;
    a /=100; b/=100;
    std::complex<float> Xvar(0,0);
    std::complex<float> Cvar(a,b);

    // X(i) = (X0)² + C
    for(int i=0; i<mStepCount; i++){
        Xvar = Xvar * Xvar;     //Xval = std::pow(Xval,2);
        Xvar += Cvar;           //X+C
        if(std::isinf( Xvar.real()) ){          //break at infinity
            QPoint endv( Xvar.real(), 0  );    //return Complex Value in fpoint
            return endv;
        }
    }
    QPoint endv( Xvar.real(), Xvar.imag()  );    //return Complex Value in fpoint
    return endv;
}

//return QPointF( t + sin(*pFloatIter1), t + cos(*pFloatIter1) );


void RenderArea::paintEvent(QPaintEvent *event)     //wird von Qt aufgerufen wenn nötig, protected+override im .h
{
    Q_UNUSED(event);        //deaktiviert Kompilerwarnung

    QPainter painter(this);
    painter.setBrush(mBackgroundColor );    //brush defines how shapes are filled

    //setShape(foo);    //verboten!!! calls repaint()-> rekursiv

    //std::complex<double> *complVal = new std::complex<double>(1,1);      //include <complex>

    //painter.drawLine(this->rect().topLeft(), this->rect().bottomRight() );
    //for (float t=0; t < tIntervLength; t += step){

    if (mDrawLine){
        //drawing area:
        painter.drawRect(this->rect() );
        QPointF center = this->rect().center();     // war im tut kein floatP, ist aber egal, konvertierung erfolgt auch automatisch
        float step = mIntervalLength / mStepCount;
        float tIntervLength = mIntervalLength + step;
        float tScale = mPreScale * mScale/100;      //preSc is set per Shape, mScale-slider:0..100..1000  => 0..1..10 *mPreScale mandel=1..100

        painter.setRenderHint(QPainter::Antialiasing, true);
        lineDrawer(step, tIntervLength, tScale, center, painter);
    }
    else{
        painter.setRenderHint(QPainter::Antialiasing, false);
        painter.setRenderHint(QPainter::TextAntialiasing, false);
        painter.setRenderHint(QPainter::SmoothPixmapTransform, false);
        painter.setRenderHint(QPainter::VerticalSubpixelPositioning, false);
        painter.setRenderHint(QPainter::LosslessImageRendering, false);

        painter.setPen(Qt::black);
        painter.drawPixmap(this->rect(), *shapemap, shapemap->rect() );
    }



    //durchläufe for() interval(256*2)+0+anfang+ende; 515
    //GESAMT DURCHLÄUFE 5, evtl wegen oversampling? -> ohne antialiasing 4
}

void RenderArea::lineDrawer(float step, float tIntervLength, float tScale, QPointF center, QPainter &painter){
    mPen.setWidth(2);
    mPen.setColor(Qt::white);        // wegmachen hier aus paint??
    painter.setPen(mPen);   //draw with the pen

    QPointF fprevPixel = compute(-tIntervLength) * tScale + center;    //first point

    for (float x= -tIntervLength; x < tIntervLength; x += step){    //drawing center focused is probably safest way
        //draws a line between actual and previous point
        QPointF fpoint = compute(x) * tScale + center;
        //konvertiere Float2D zu Int(Pixel)2D, unnötig
        //        QPoint pixel;
        //        pixel.setX(fpoint.x() * tScale + center.x() );
        //        pixel.setY(fpoint.y() * tScale + center.y() );
        if(optionCool)painter.drawLine(fpoint, center);        //das war zuerst ein Fehler im Tut, als prevPixel gefehlt hat, grad übernommen
        painter.drawLine(fpoint, fprevPixel);
        fprevPixel = fpoint;
    }//X-loop
}

void RenderArea::plotDrawer(QPainter *painter){

    float tIntervLength = mIntervalLength;
    float tScale = mPreScale * mScale/100;      //preSc is set per Shape, mScale-slider:0..100..1000  => 0..1..10 *mPreScale mandel=1..100

    int tXoffset = shapestore[this->mShapeIndex].Xoffset;// + mMove.x();   //lifetime of move is set with setShape(), that works well
    int tYoffset = shapestore[this->mShapeIndex].Yoffset;

    int tWidth = this->width();
    int tHeight = this->height();
    QPoint tArea = QPoint(this->width(), this->height() );

    const float yInterval = tIntervLength * tHeight/tWidth;
    const QPointF step = QPointF(tIntervLength/tWidth / tScale,        // *step scales a pix value to Interval-Units
                                 yInterval/tHeight / tScale);    //bigger scale -> smaller steps

    const float tInitScale = mPreScale * 100/100;      //preSc is set per Shape, mScale-slider:0..100..1000  => 0..1..10 *mPreScale mandel=1..100
    // 1. set the offset from shapestore for the point-of-view at beginning
    float xStartOffset = tIntervLength/tWidth /tInitScale * tXoffset;   //viewport offset at init scale (1) =initStep*offset   -> 100px=-2/3  relative to 0,0
    float yStartOffset = yInterval/tHeight /tInitScale * tYoffset;


    // 2. this 2nd offset is added to the point-of-view, the step component represents the actual scale which is added in permanently
    mXoffset += (mTempMove.x() * step.x() );
    mYoffset += (mTempMove.y() * step.y() );
    mTempMove = QPoint(0,0);

    float x = xStartOffset + mXoffset;
    // 3. add the remaining half of the area-in-sight to the offsets, that equates to the final starting point
    x = x - tIntervLength/2 /tScale;

    if(true){      //float calculation
        for(int w= 0; w < tWidth; w++){
            //y = -1.5;   //debug

            float y = yStartOffset + mYoffset;
            y = y - yInterval/2 /tScale;

            for(int h= 0; h < tHeight; h++){

                // (x,y)Plot function was added here (for the mandelbrot set)
                qreal result = compute(x, y).x();
                quint64 atest = (quint64)result;     //(quint64)
                //we need the upper 3 bytes:
                atest = atest >> ((sizeof(qreal)-3)*8 );        // -inf => 0x800000
                //char32_t ctest = atest & 0xFFFFFF;

                painter->setPen(atest );

                QPointF fpoint(w, h);       //area starts at (0,0)
                painter->drawPoint(fpoint);
                y += step.y();

            }//X-loop
            x += step.x();

        }//Y-loop
    } else {        //int calculation test
        for(int w= 0; w < tWidth; w++){
            for(int h= 0; h < tHeight; h++){
                // (x,y)Plot function was added here (for the mandelbrot set)
                painter->setPen(compute(w-tWidth/2, h-tHeight/2).x() );        //pass int values to complex-float

                QPointF fpoint(w, h);       //area starts at (0,0)
                painter->drawPoint(fpoint);
            }//X-loop
        }//Y-loop
    }
}





