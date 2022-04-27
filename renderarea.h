#ifndef RENDERAREA_H
#define RENDERAREA_H

#include <QWidget>
#include <QPen>
#include <complex>
#include <QPixmap>
#include <QRunnable>
#include <QMutex>


class RenderArea;

class calcTask : public QRunnable
{//  Q_OBJECT - runnable is a strange glue class from thread to conccurrent, qobject doesnt seem to work here
    friend class RenderArea;
public:
      //parent not optional-> no nullptr
    explicit calcTask(RenderArea *parent, QPointF startpnt, QPointF stepsize, QSize targetsize, unsigned int ShapeIndex, unsigned int mStepCount);//, std::vector<bool> *infm, int *StepCount)
    ~calcTask();

    void run() override;    //arguments must stay blank for override, therefore parameters are stored as members
    //void setThreadParams(QPointF startpnt, QPointF stepsize, QSize targetsize);


protected:

private:
    RenderArea *parent;
    QMutex mutex;

    QPixmap *thrdmap;
    QPainter *thrdpainter;

    QPointF tStartpnt;
    QPointF tStepsize;
    QSize tTargetsize;

    unsigned int mShapeIndex, mStepCount;

//    QPointF startpnt=QPointF(0,0);
//    QPointF step=QPointF(0.1,0.1);
//    QSize targetsize=QSize(320,240);

    static QPointF compute2(float x,  float y, int StepCount, unsigned int ShapeIndex);           //dispatcher based on type
    static QPointF compute_mandelb(float x,  float y, int StepCount);
    static void plotDrawer(QPainter *painter, unsigned int ShapeIndex, QPointF startpnt, QPointF step, QSize targetsize, int StepCount);

};



class RenderArea : public QWidget   //RenderArea ist ein Objekt in UI
{
    Q_OBJECT
    friend class calcTask;

public:
    explicit RenderArea(QWidget *parent = nullptr);
    ~RenderArea();

    QSize minimumSizeHint() const override; // Q_DECL_OVERRIDE; im Tutorial ist deprecated
    QSize sizeHint() const override;

    void setBackgroundColor(QColor color) {mBackgroundColor = color; updatePixplotOutput(); update(); }  //setter, inline weil kurz
    QColor backgroundColor() const { return mBackgroundColor; }             //getter, const schützt die Member vor Änderungen

    void setShapeColor(QColor color) {mPen.setColor(color); updatePixplotOutput(); update(); }  //setter, inline weil kurz
    QColor ShapeColor() const { return mPen.color(); }             //getter, const schützt die Member vor Änderungen

    unsigned int getShapeIDbyName(QString name);

    unsigned int setShape (unsigned int row);
    unsigned int getShapeIndexID() const { return mShapeIndex; }

    //unsigned int setShape (QString query);

    void setInterval(float value) { mIntervalLength = value; updatePixplotOutput(); update(); }
    float Interval() const { return mIntervalLength; }

    void setScale(int scale) { mScale = scale; updatePixplotOutput(); update(); }     //int->float is ok da nur ganze werte
    float scale() const { return mScale; }

    void setStepCount(unsigned int count) {mStepCount = count; updatePixplotOutput(); update(); }
    int stepCount() const {return mStepCount; }

    void setCool(bool Cool) { optionCool = Cool; updatePixplotOutput(); update(); }
    bool Cool() const { return optionCool; }

    typedef struct id_name_scale_interval_steps{      //tag optional
        unsigned int id;
        QString name;               //, calculation name;
        float prescale, interval;   //Length; //8, M_PI;
        int steps;                  //Count;
        float Xoffset, Yoffset;
    }ShapeType;

    ShapeType setShapeparameteres(  //function
        unsigned int id,
        QString name,
        float sPreScale,
        float interval,
        int steps,
        float Xoffset=0, float Yoffset=0    //default argument here causes casual usage
    );

    //QList<ShapeType> shapetest;

    QStringList ShapeList;      //menu entries

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
#if QT_CONFIG(wheelevent)
    void wheelEvent(QWheelEvent *event) override;
#endif
    void zoom(int steps);
    QPoint mMousePos = QPoint(0,0), mtMouseMove;
    float mXoffset=0, mYoffset=0;               //valid till setShape()


signals:
    void valueChanged();

private:
    QList<calcTask*> mCalcTasks;
    bool mTaskdone=true, mWindowstarted = false;
    unsigned int mMaxThreads = 1, mMThrdSqrt = 0, mThreads = 0;
    QList<ShapeType> shapestore;     //dynamische Qliste des structs, kann wie c array verwendet werden
    QPixmap *paintarea, *dsizebuffer, *dsbufferold;
    std::vector<bool> *infm = nullptr;     //pixelmask infinitymap, do Qlist instead?

    QColor mBackgroundColor;
    QColor mShapeColor;
    QPen mPen;

    unsigned int mShapeIndex, mStepCount;
    float mPreScale, mIntervalLength;
    qint64 mScale=100;    //maybe convert to exponential, longlongint. type is guaranteed 64-bit on all platforms supported by Qt

    bool optionCool, mDrawLine;
    bool mMouseLB=false;
    const bool mScaleSteps = true;

    static QPointF compute(float x, unsigned int ShapeIndex);           //dispatcher based on type

    static QPointF compute_astroid(float x);
    static QPointF compute_cycloid(float x);
    static QPointF compute_huygens(float x);
    static QPointF compute_hypo(float x);
    static QPointF compute_line(float x);
    static QPointF compute_circle(float x);
    static QPointF compute_elipse(float x);
    static QPointF compute_mandala(float x);
    static QPointF compute_star(float x);
    static QPointF compute_cloud(float x);
    static QPointF compute_tilde(float x);

    void lineDrawer(float step, float tIntervLength, float scale, QPointF center, QPainter &painter);
    void updatePixplotOutput();
    void calcTaskDone(QPixmap resultmap);
    void calcTaskDone(QPixmap *resultmap);

    void startThreads(QSize mapsize);
    calcTask* setupRenderthread(QSize *mapsize, float intervalStart, float intervalEnd);


    //void plotDrawer(QPainter *painter, QPointF startpnt, QPointF step, QSize targetsize,std::vector<bool> *infm, int mStepCount);
};

#endif // RENDERAREA_H
