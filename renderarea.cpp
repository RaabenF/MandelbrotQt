#include "renderarea.h"
#include <QPaintEvent>
#include <QPainter>
//#include "mainwindow.h"

#include <QThreadPool>
#include <QRunnable>

RenderArea::RenderArea(QWidget *parent) :  QWidget(parent)     //is set with mainwindow ui->setupUi() ?
    ,infm(new std::vector<bool>)    //init list:
    ,mBackgroundColor(Qt::darkBlue)
    ,mShapeColor(255, 255, 255)
    ,mStepCount((8)),mPreScale(1), mIntervalLength(1), optionCool(false)
{
//test, geht nicht wg kein append()
//    shapetest[0]={     //Qlist(dynamic array) - vom struct
//                    .id=999,
//                    .name="dfg",    //, calculation name;
//                    .sPreScale=1,
//                    .interval=1,     //Length; //8, M_PI;
//                    .steps=1        //Count;
//                };
//    shapetest[0]=paramShape(0,"test",10,M_PI,16);

//    for(int i=0,i<99,i++){
//        shapestore[i]=
//    }
    //                          ID,Name     Prescale,Interval,Steps, (x-,y-offs)
    shapestore.append(setShapeparameteres(0,"Astroid",73,M_PI,256) );
    shapestore.append(setShapeparameteres(1,"Cycloid",11,6 * M_PI,128) );
    shapestore.append(setShapeparameteres(2,"HygensCycloid",35,2*M_PI,256) );
    shapestore.append(setShapeparameteres(3,"HypoCycloid",55,M_PI,256) );
    shapestore.append(setShapeparameteres(4,"Elipse",100,M_PI,128) );
    shapestore.append(setShapeparameteres(5,"Mandala",8,6*M_PI,512) );
    shapestore.append(setShapeparameteres(6,"Star",20,3*M_PI,256) );
    shapestore.append(setShapeparameteres(7,"Cloud",10,14*M_PI,128) );
    shapestore.append(setShapeparameteres(8,"Tilde",55,M_PI,256,0,0) );
    shapestore.append(setShapeparameteres(9,"Mandel Brot",1, 3, 64, -200) );   //interval empfohlen: -3..3    steps ist die Auflösung der Berechnung
    shapestore.append(setShapeparameteres(10,"tst",30,M_PI,256) );
    //shapestore.append(paramShape(,"",10,M_PI,256) );      //copy me

    paintarea = new QPixmap(this->size() );    //inherits paintdevice
    dsizebuffer = new QPixmap(paintarea->size()*2 );

    mMaxThreads = QThread::idealThreadCount();
    if(mMaxThreads < 2){    // 1==unknown
        QThreadPool::globalInstance()->setMaxThreadCount(4);    //try 4 or 2 or 1 threads?
        qDebug()<< "Optimum Number of Threads could not be detected";
    }
    qDebug()<<"Optimum Threadnumber on your machine is: " << mMaxThreads;
    mMThrdSqrt = (unsigned int)std::sqrt(mMaxThreads);      //eg sqrt(6)=>2
    mMaxThreads = mMThrdSqrt * mMThrdSqrt;
    mMaxThreads=1;
    QThreadPool::globalInstance()->setMaxThreadCount(mMaxThreads);

    //QThreadPool::globalInstance()->setExpiryTimeout(-1);  //negative=dont destroy new threads
//    for(unsigned int i=0; i<mMaxThreads; i++){
//        QThreadPool::globalInstance()->reserveThread();     //not neccesary?
//    }
    qDebug() << "After Init: " << QThreadPool::globalInstance()->activeThreadCount() << " Threads, expire after: "<<QThreadPool::globalInstance()->expiryTimeout();
    qDebug()<<"initialization of Render Area done";
}
RenderArea::~RenderArea(){
    //delete dsizePainter;  //delete in this order
    delete paintarea;
    delete dsizebuffer;
}

QSize RenderArea::minimumSizeHint() const { //recommended minimum size for the widget
    return QSize(320,240);
}

QSize RenderArea::sizeHint() const {        //return the preferred size of this item.
    return QSize(640,480);
}

void RenderArea::resizeEvent(QResizeEvent *event){
    //called before paintEvent  |   event->oldSize();
    if(!mDrawLine){
        delete paintarea;
        delete dsizebuffer;
        paintarea = new QPixmap(event->size() );
        dsizebuffer = new QPixmap(paintarea->size()*2 );
        updatePixplotOutput();
    }

    event->accept();
}

void RenderArea::mousePressEvent(QMouseEvent *event){
    if (event->button() == Qt::LeftButton) {
        mMousePos = event->pos();   //position().toPoint();
    }
    mMouseLB=true;//event->ignore(); doesnt work
}

void RenderArea::mouseMoveEvent(QMouseEvent *event){
    //drag while clicking (mousebutton event doesnt work here)
    if ( !mMousePos.isNull() && mMouseLB ){
        mtMouseMove -= event->pos() - mMousePos;
        if (mtMouseMove.manhattanLength() > 3){   //movement treshold
            if(!mDrawLine){
                updatePixplotOutput();
                update();
            }
        }
    }
    mMousePos = event->pos();    //rel to widget or: globalPosition();
    event->accept();
}

void RenderArea::mouseReleaseEvent(QMouseEvent *event){
    Q_UNUSED(event);
    mMouseLB = false;
}

void RenderArea::mouseDoubleClickEvent(QMouseEvent *event){
    if (event->button() == Qt::LeftButton){
        RenderArea::zoom(1);
        event->accept();
    }
}

void RenderArea::wheelEvent(QWheelEvent *event){
    //QPoint numPixels = event->pixelDelta();
    //    if (!numPixels.isNull()) {
    //        scrollWithPixels(numPixels);
    //    } else
    int steps = event->angleDelta().y() / 120;   //one mousewheel-scroll is 15 degree long
    RenderArea::zoom(steps);
    event->accept();
}

void RenderArea::zoom(int steps){
    //redraw with value stored whith last mouse event (scroll to cursor)
    QPoint pos = QCursor::pos() - this->window()->pos();
    const float zoomscale = 1.1;
    //if(this->underMouse() ){
    if(!mDrawLine){
        if (steps>0) {
            if(mScaleSteps)setStepCount(mStepCount * (zoomscale) );
            mtMouseMove -= (this->rect().center() - pos);
            setScale(mScale*2 );
        }else if(steps<0){
            if(mScaleSteps)setStepCount(mStepCount / (zoomscale) );
            mtMouseMove += (this->rect().center() - pos)/2;
            setScale(mScale/2 );
        }
    }
    emit this->valueChanged();  //mainwindow updates ui then
    update();
}

//is called on (resize), mouse events (move, zoom), or value changes:
void RenderArea::updatePixplotOutput(){

    //todo: dispatch partial pixmaps in here:
    if(false){//!dsizebuffer->isNull() ){//mScale==100)
        //copy=deepcopy of part of pixmap
        int tWidth = dsizebuffer->width(), tHeight = dsizebuffer->height();
        QRect trect = dsizebuffer->rect();
        //set start of rect, changes size
        trect.setX(tWidth/4);
        trect.setY(tHeight/4);
        trect.setSize(paintarea->size() );
        *paintarea = dsizebuffer->copy(trect );
        //*paintarea = dsizebuffer->scaled(this->size(),Qt::KeepAspectRatioByExpanding,Qt::SmoothTransformation);
    }else{
        *paintarea = dsizebuffer->scaled(this->size(),Qt::KeepAspectRatioByExpanding,Qt::SmoothTransformation);
    }
}

RenderArea::ShapeType RenderArea::setShapeparameteres(unsigned int id, QString name, float preScale, float interval, int steps, float Xoffset, float Yoffset ){
    ShapeType sdata = {     //Qlist(dynamic array) - vom struct
         .id=id,
         .name=name,            //, calculation name;
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
        mXoffset = shapestore[mShapeIndex].Xoffset;
        mYoffset = shapestore[mShapeIndex].Yoffset;
        mScale=100; //uncomment to keep zoom on change
        //optionCool=false;
        mDrawLine=false;

        mXoffset = 0;
        mYoffset = 0;
        mtMouseMove = QPoint(0,0);
    }
    else{
        qDebug() << "shapelist index out of range, no menu";
        return shapestore.length();                             //return failure
    }
    //init plotter specific stuff - start painting per pixel mode
    if(mShapeIndex >= getShapeIDbyName("mandel brot") ){
        infm->reserve(this->height()*this->width() );
        mDrawLine = false;

        QSize tsize = this->dsizebuffer->size();// paintarea->size();
        if(mWindowstartet)startThreads(tsize );
        else{ startThreads(tsize );
            mWindowstartet = true;
        }
    }else{     //start drawing mode
        infm->clear();
        mDrawLine = true;
    }
    //emit this->valueChanged();    //not needed till now
    return 0;   //return success
}
//only call in setShape so far, with paintarea
void RenderArea::startThreads(QSize mapsize){
    //delete threads first
    if( QThreadPool::globalInstance()->activeThreadCount() == 0){
        mThreads = 0;
        mCalcTasks.clear();
    }else{
        QThreadPool::globalInstance()->clear();//only if autodelete
        //2nd and last try:
        if( QThreadPool::globalInstance()->activeThreadCount() == 0){
            mThreads = 0;
            mCalcTasks.clear();
            qDebug() << "could not delete all threads";
        }
    }
    if(mTaskdone)
    while(mThreads < mMaxThreads){
        //Interval enables calculating fixed-ratio square parts of the whole picture
        float tIntervStart = -mIntervalLength;
        float tIntervEnd = tIntervStart + mIntervalLength*2/mMThrdSqrt;   //mMThrdSqrt == number of threads per one side of the pixmap
        calcTask* tskptr=nullptr;

        tskptr = setupRenderthread(&mapsize, tIntervStart, tIntervEnd);
        if(tskptr!=nullptr){
            mCalcTasks.append(tskptr);    //sets up the Thread's Parameters as well
            QThreadPool::globalInstance()->start(tskptr);
        }
        else qDebug() << "thread not found";
        ++mThreads;
    }
    qDebug() << "Active Threads: " << QThreadPool::globalInstance()->activeThreadCount();
}

unsigned int RenderArea::getShapeIDbyName(QString name){
    for (int i=0; shapestore.length() > i; i++ ){    //length starts@ 1, row @ 0
        if(!shapestore[i].name.compare(name,Qt::CaseInsensitive) ) {
            return shapestore[i].id;
        }
    }
    return 0;   //else return standartvalue
}

void RenderArea::calcTaskDone(QPixmap resultmap){
    *dsizebuffer = resultmap;
    //*paintarea = dsizebuffer->copy(trect );
    *paintarea = dsizebuffer->scaled(this->size(),Qt::KeepAspectRatioByExpanding,Qt::SmoothTransformation);
    mTaskdone=true;
    updatePixplotOutput();
    update();
}
//Interval enables calculating fixed-ratio square parts of the whole picture
calcTask*  RenderArea::setupRenderthread(QSize *mapsize, float intervalStart, float intervalEnd){
    if(mShapeIndex >= getShapeIDbyName("mandel brot") ){
        int tWidth = mapsize->width(), tHeight = mapsize->height();
        infm->resize(tWidth*tHeight);

        const float xInterval = (intervalEnd - intervalStart)/2;
        const float yInterval = xInterval * tHeight/tWidth;
        float tScale = mPreScale * mScale/100;      //preSc is set per Shape, mScale-slider:0..100..1000  => 0..1..10 *mPreScale mandel=1..100

        int tXoffset = shapestore[this->mShapeIndex].Xoffset;// + mMove.x();   //lifetime of mouse-move is reset with setShape(), works well
        int tYoffset = shapestore[this->mShapeIndex].Yoffset;

        const QPointF step = QPointF(xInterval/tWidth / tScale,  // *step scales a pix value to Interval-Units
                                     yInterval/tHeight / tScale);    //bigger scale -> smaller steps

        const float tInitScale = mPreScale * 100/100;      //preSc is set per Shape, mScale-slider:0..100..1000  => 0..1..10 *mPreScale mandel=1..100
        // 1. set the offset from shapestore for the point-of-view at beginning
        float xStartOffset = xInterval/tWidth /tInitScale * tXoffset;   //viewport offset at init scale (1) =initStep*offset   -> 100px=-2/3  relative to 0,0
        float yStartOffset = yInterval/tHeight /tInitScale * tYoffset;

        // 2. this 2nd offset is added to the point-of-view, the step component represents the actual scale which is added in permanently
        mXoffset += (mtMouseMove.x() * step.x() );
        mYoffset += (mtMouseMove.y() * step.y() );
        mtMouseMove = QPoint(0,0);  //move recieved

        yStartOffset += mYoffset;
        xStartOffset += mXoffset;

        // 3. add the remaining half of the area-in-sight to the offsets, that equates to the final starting point
        QPointF startpnt = QPointF(xStartOffset - xInterval/2 /tScale,
                                     yStartOffset - yInterval/2 /tScale);
        calcTask* threadpointer=nullptr;
        if(false){//start thread
            threadpointer = new calcTask(this, startpnt, step, *mapsize, mShapeIndex, mStepCount );
            QThreadPool::globalInstance()->start(threadpointer );        //start() adds thread to queue, starts while running
        }else{//for DEBUG:
            QPainter *dspaint = new QPainter(dsizebuffer);
            calcTask::plotDrawer(dspaint, mShapeIndex, startpnt,step,dsizebuffer->size(),mStepCount );
            this->calcTaskDone(*dsizebuffer);
            delete dspaint;
        }
        return threadpointer;
    }
    return nullptr;
}

void RenderArea::paintEvent(QPaintEvent *event)     //wird von Qt aufgerufen wenn nötig, protected+override im .h
{
    Q_UNUSED(event);        //deaktiviert Kompilerwarnung

    QPainter rAreaPainter(this);
    rAreaPainter.setBrush(mBackgroundColor );    //brush defines how shapes are filled

    //setShape(foo);    //verboten!!! calls repaint()-> rekursiv
    //std::complex<double> *complVal = new std::complex<double>(1,1);      //include <complex>

    if (mDrawLine){
        //drawing area:
        rAreaPainter.drawRect(this->rect() );
        QPointF center = this->rect().center();     // war im tut kein floatP, ist aber egal, konvertierung erfolgt auch automatisch
        float step = mIntervalLength / mStepCount;
        float tIntervLength = mIntervalLength + step;
        float tScale = mPreScale * mScale/100;      //preSc is set per Shape, mScale-slider:0..100..1000  => 0..1..10 *mPreScale mandel=1..100

        rAreaPainter.setRenderHint(QPainter::Antialiasing, true);
        lineDrawer(step, tIntervLength, tScale, center, rAreaPainter);
    }
    else{       //start plot drawing process
        rAreaPainter.setRenderHint(QPainter::Antialiasing, false);
        rAreaPainter.setRenderHint(QPainter::TextAntialiasing, false);
        rAreaPainter.setRenderHint(QPainter::SmoothPixmapTransform, false);
        rAreaPainter.setRenderHint(QPainter::VerticalSubpixelPositioning, false);
        rAreaPainter.setRenderHint(QPainter::LosslessImageRendering, false);
        rAreaPainter.setPen(Qt::black);

        //draw paintarea buffer to screen:
        rAreaPainter.drawPixmap(this->rect(), *paintarea, paintarea->rect() );
        if(! (this->rect()==paintarea->rect()) )qDebug() << "wrong pixmap size";
    }
    //durchläufe for() interval(256*2)+0+anfang+ende; 515
    //GESAMT DURCHLÄUFE 5, evtl wegen oversampling? -> ohne antialiasing 4
}


QPointF RenderArea::compute(float x, unsigned int ShapeIndex){
    switch(ShapeIndex){
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

//return QPointF( t + sin(*pFloatIter1), t + cos(*pFloatIter1) );

void RenderArea::lineDrawer(float step, float tIntervLength, float tScale, QPointF center, QPainter &temppainter){
    mPen.setWidth(2);
    mPen.setColor(Qt::white);        // wegmachen hier aus paint??
    temppainter.setPen(mPen);   //draw with the pen

    QPointF fprevPixel = compute(-tIntervLength, mShapeIndex) * tScale + center;    //first point

    for (float x= -tIntervLength; x < tIntervLength; x += step){    //drawing center focused is probably safest way
        //draws a line between actual and previous point
        QPointF fpoint = compute(x, mShapeIndex) * tScale + center;
        //konvertiere Float2D zu Int(Pixel)2D, unnötig
        //        QPoint pixel;
        //        pixel.setX(fpoint.x() * tScale + center.x() );
        //        pixel.setY(fpoint.y() * tScale + center.y() );
        if(optionCool)temppainter.drawLine(fpoint, center);        //das war zuerst ein Fehler im Tut, als prevPixel gefehlt hat, grad übernommen
        temppainter.drawLine(fpoint, fprevPixel);
        fprevPixel = fpoint;
    }//X-loop
}








//plotts the given startPoint + steps*targetsize to a Paintdevice, which is tied to a thread:
void calcTask::plotDrawer(QPainter *painter, unsigned int ShapeIndex, QPointF startpnt, QPointF step,  QSize targetsize, int StepCount){
    int pixwidth = targetsize.width(), pixheight = targetsize.height();
    float ystart = startpnt.y();
    int pixcounter=0;

    //two for-loops count every pixel: w=>x, h=>y, simultanious x & y count up for every pixel, in step-length of the interval (of the drawing function => compute() )
    for(int h= 0; h < pixheight; h++){
        //x is reset in every row:
        float xrun = startpnt.x();

        for(int w= 0; w < pixwidth; w++){
            QPointF result = QPointF(0,0);

            //if(!infm.at(pixcounter) )   //check if pixel in bitmap is black
                result = compute2(xrun, ystart, StepCount, ShapeIndex);

            //when inf is not reached (==0) paint bit-mask black=0
            if(result.x() != 0){
                painter->setPen(Qt::black);
                //infm->at(pixcounter) = 1;
            }else{
            //iterations of the fractal scaled to color#   //modulo is useless, overflow does the same mostly.
            // use two complement colors is best. more iterations then advance contrast and detail
                //float i = result.y() / ((float)StepCount) * 0xffffff;    //option psycho
                //char r = uiter>>16 & 0xff, g = uiter>>8 & 0xff ,b = uiter & 0xff;    //option psycho
                unsigned int uiter = result.y() / StepCount * 0x2ff;    //standart: *255 or 0xff
                char r = 0, g = uiter>>1 & 0xff ,b = uiter & 0xff;

                if(false){   //option crisp
                    float fRC = result.x();  //the abs-length of Complex causes some bright pixels
                    int iRC = fRC*100;       //*100...500 for abs, absolute length is always positive
                    r = iRC;
                }
                QRgb color = qRgb( r, g, b);    //255,R,G,B

                painter->setPen(color);
                //infm->at(pixcounter) = 0;
            }
            //always:
            QPointF fpoint(w, h);       //area starts at (0,0)
            painter->drawPoint(fpoint);
            //infpainter->drawPoint(fpoint);

            xrun += step.x();
            pixcounter++;
        }//X-loop

        ystart += step.y();
    }//Y-loop
}



QPointF calcTask::compute2(float x, float y, int StepCount, unsigned int ShapeIndex){
    switch(ShapeIndex){
    case 9:
        return calcTask::compute_mandelb(x, y, StepCount);
        break;
    default:
        //return  RenderArea::compute_line(x);
        break;
    }
    return QPointF(0,0);
}

QPointF calcTask::compute_mandelb(float x,  float y, int StepCount){
    std::complex<double> Xvar(0,0);      //equals the Complex Number real=t * 1imag        #include <complex>
    std::complex<double> Cvar(x,y);      //float looks nice, too

    // X(i) = (X0)² + C
    for(int i=0; i < StepCount; i++){
        Xvar = Xvar * Xvar;     // Xval² = std::pow(Xval,2);
        Xvar += Cvar;           // X+C

        if(std::isinf( Xvar.real()) ){          //break at infinity
            QPointF endv( 0, i  );    //return number of iterations
            return endv;
        }
    }
    //std::abs(Xvar) std::arg(Xvar)     arg=angular=phase angle, abs=total length of C
    QPointF endv( std::abs(Xvar), StepCount);    //return Complex Value, and Iterations, in fpoint
    return endv;
}

calcTask::calcTask(RenderArea *parent, QPointF startpnt, QPointF stepsize, QSize targetsize, unsigned int ShapeIndex, unsigned int StepCount):
  parent(parent)
  //, std::vector<bool> *infm, int StepCount)
  ,tStartpnt(startpnt)
  ,tStepsize(stepsize)
  ,tTargetsize(targetsize)
  ,mShapeIndex(ShapeIndex)
  ,mStepCount(StepCount)
{
    QMutexLocker locker(&mutex);
    //mutex.lock();
    this->parent->mTaskdone=false;      //lock sth

    //mutex.unlock();
    //we need one painter per thread
    thrdmap = new QPixmap(targetsize);
    thrdpainter = new QPainter(thrdmap);//targetmap);
}

calcTask::~calcTask(){
    mutex.lock();
//    //thrdpainter->end();
    delete thrdpainter;
//    //condition.wakeOne();    //Wakes one thread waiting on THAT wait condition
    mutex.unlock();
}

void calcTask::run(){
    qDebug() << "Hello world from thread" << QThread::currentThread();
    this->plotDrawer(thrdpainter,   //only call
                   mShapeIndex,
                   tStartpnt,
                   tStepsize,
                   tTargetsize,
                   mStepCount);
    this->parent->calcTaskDone(*thrdmap);
}











