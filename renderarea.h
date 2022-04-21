#ifndef RENDERAREA_H
#define RENDERAREA_H

#include <QWidget>
#include <QPen>
#include <complex>
#include <QPixmap>
#include <QRunnable>

class RenderArea : public QWidget   //RenderArea ist ein Objekt in UI
{
    Q_OBJECT
    friend class calcTask;

public:
    explicit RenderArea(QWidget *parent = nullptr);
    ~RenderArea();

    QSize minimumSizeHint() const override; // Q_DECL_OVERRIDE; im Tutorial ist deprecated
    QSize sizeHint() const override;

    void setBackgroundColor(QColor color) {mBackgroundColor = color; updateOutput(); update(); }  //setter, inline weil kurz
    QColor backgroundColor() const { return mBackgroundColor; }             //getter, const schützt die Member vor Änderungen

    void setShapeColor(QColor color) {mPen.setColor(color); updateOutput(); update(); }  //setter, inline weil kurz
    QColor ShapeColor() const { return mPen.color(); }             //getter, const schützt die Member vor Änderungen

    unsigned int getShapeIDbyName(QString name);

    unsigned int setShape (unsigned int row);
    unsigned int getShapeIndexID() const { return mShapeIndex; }

    //unsigned int setShape (QString query);

    void setInterval(float value) { mIntervalLength = value; updateOutput(); update(); }
    float Interval() const { return mIntervalLength; }

    void setScale(int scale) { mScale = scale; updateOutput(); update(); }     //int->float is ok da nur ganze werte
    float scale() const { return mScale; }

    void setStepCount(qint64 count) {*mStepCount = count; updateOutput(); update(); }
    int stepCount() const {return *mStepCount; }

    void setCool(bool Cool) { optionCool = Cool; updateOutput(); update(); }
    bool Cool() const { return optionCool; }

    typedef struct id_name_scale_interval_steps{      //tag optional
        unsigned int id;
        QString name;               //, function_name;
        float prescale, interval;   //Length; //8, M_PI;
        int steps;                  //Count;
        float Xoffset, Yoffset;
    }ShapeType;

    ShapeType paramShape(
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
    QList<ShapeType> shapestore;     //dynamische Qliste des structs, kann wie c array verwendet werden
    QPixmap *paintarea, *dsizebuffer;
    QPainter *mappainter;
    std::vector<bool> *infm = nullptr;     //pixelmask infinitymap

    QColor mBackgroundColor;
    QColor mShapeColor;
    QPen mPen;

    unsigned int mShapeIndex;
    float mPreScale, mIntervalLength;
    int *mStepCount=nullptr;
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
    void updateOutput();
    void updatePixmap(QPixmap *targetmap, float intervalStart, float intervalEnd);

    //void plotDrawer(QPainter *painter, QPointF startpnt, QPointF step, QSize targetsize,std::vector<bool> *infm, int *mStepCount);
};

class calcTask : public QRunnable
{
    //Q_OBJECT
public:
      //parent not optional-> no nullptr
    explicit calcTask(RenderArea *parent, QPainter *painter, QPointF startpnt, QPointF step,  QSize targetsize);//, std::vector<bool> *infm, int *StepCount)

    void run() override;

    static QPointF compute2(float x,  float y, int *StepCount, unsigned int ShapeIndex);           //dispatcher based on type
    static QPointF compute_mandelb(float x,  float y, int *StepCount);
    void plotDrawer(QPainter *painter, unsigned int ShapeIndex, QPointF startpnt, QPointF step, QSize targetsize,std::vector<bool> *infm, int *StepCount);

};

#endif // RENDERAREA_H
