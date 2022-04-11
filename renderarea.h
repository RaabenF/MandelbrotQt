#ifndef RENDERAREA_H
#define RENDERAREA_H

#include <QWidget>
#include <QPen>
#include <complex>
#include <QPixmap>

class RenderArea : public QWidget
{
    Q_OBJECT
public:
    explicit RenderArea(QWidget *parent = nullptr);

    QSize minimumSizeHint() const override; // Q_DECL_OVERRIDE; im Tutorial ist deprecated
    QSize sizeHint() const override;

    void setBackgroundColor(QColor color) {mBackgroundColor = color; updatePixmap(); update(); }  //setter, inline weil kurz
    QColor backgroundColor() const { return mBackgroundColor; }             //getter, const schützt die Member vor Änderungen

    void setShapeColor(QColor color) {mPen.setColor(color); updatePixmap(); update(); }  //setter, inline weil kurz
    QColor ShapeColor() const { return mPen.color(); }             //getter, const schützt die Member vor Änderungen

    unsigned int getShapeIDbyName(QString name);

    unsigned int setShape (unsigned int row);
    unsigned int getActShapeID() const { return mShapeIndex; }

    //unsigned int setShape (QString query);

    void setInterval(float value) { mIntervalLength = value; updatePixmap(); update(); }
    float Interval() const { return mIntervalLength; }

    void setScale(int scale) { mScale = scale; updatePixmap(); update(); }     //int->float is ok da nur ganze werte
    float scale() const { return mScale; }

    void setStepCount(int count) {mStepCount = count; updatePixmap(); update(); }
    int stepCount () const {return mStepCount; }

    void setCool(bool Cool) { optionCool = Cool; updatePixmap(); update(); }
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
    QPoint mMouseOldPos = QPoint(0,0), mTempMove;
    float mXoffset=0, mYoffset=0;               //valid till setShape()


signals:

private:
    QList<ShapeType> shapestore;     //dynamische Qliste des structs, kann wie c array verwendet werden
    QPixmap *shapemap;
    QPainter *mappainter;

    QColor mBackgroundColor;
    QColor mShapeColor;
    QPen mPen;

    unsigned int mShapeIndex=0;
    float mIntervalLength, mPreScale;
    int mStepCount, mScale=100;
    bool optionCool, mDrawLine;

    void updatePixmap();

    QPointF compute(float x);           //dispatcher based on type
    QPointF compute(float x,  float y);
    QPoint compute(int x, int y);

    QPointF compute_astroid(float x);
    QPointF compute_cycloid(float x);
    QPointF compute_huygens(float x);
    QPointF compute_hypo(float x);
    QPointF compute_line(float x);
    QPointF compute_circle(float x);
    QPointF compute_elipse(float x);
    QPointF compute_mandala(float x);
    QPointF compute_star(float x);
    QPointF compute_cloud(float x);
    QPointF compute_tilde(float x);
    QPointF compute_mandelb(float x,  float y);
    QPoint compute_mandelb(int x,  int y);

    void lineDrawer(float step, float tIntervLength, float scale, QPointF center, QPainter &painter);
    void plotDrawer(QPainter *painter);
};

#endif // RENDERAREA_H
