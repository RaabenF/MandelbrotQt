#ifndef RENDERAREA_H
#define RENDERAREA_H

#include <QWidget>
#include <QPen>

class RenderArea : public QWidget
{
    Q_OBJECT
public:
    explicit RenderArea(QWidget *parent = nullptr);

    QSize minimumSizeHint() const override; // Q_DECL_OVERRIDE; im Tutorial ist deprecated
    QSize sizeHint() const override;

    void setBackgroundColor(QColor color) {mBackgroundColor = color; repaint();}  //setter, inline weil kurz
    QColor backgroundColor() const { return mBackgroundColor; }             //getter, const schützt die Member vor Änderungen

    void setShapeColor(QColor color) {mPen.setColor(color); repaint();}  //setter, inline weil kurz
    QColor ShapeColor() const { return mPen.color(); }             //getter, const schützt die Member vor Änderungen

    int setShape (int row);

    void setInterval(float value) { mIntervalLength = value; repaint(); }
    float Interval() const { return mIntervalLength; }

    void setScale(int scale) { mScale = scale; repaint(); }     //int->float is ok da nur ganze werte
    float scale() const { return mScale; }

    void setStepCount(int count) {mStepCount = count; repaint(); }
    int stepCount () const {return mStepCount; }

    void setCool(bool Cool) { optionCool = Cool; repaint(); }
    bool Cool() const { return optionCool; }

    typedef struct name_scale_interval_steps_id{      //tag optional
        unsigned int id;
        QString name;//, function_name;
        float sPreScale, interval;//Length; //8, M_PI;
        int steps;//Count;
    }ShapeType;

    ShapeType paramShape(
        unsigned int id,
        QString name,
        float sPreScale,
        float interval,
        int steps
    );

    //QList<ShapeType> shapetest;

    QStringList ShapeList;      //menu entries


protected:
    void paintEvent(QPaintEvent *event) override;

signals:

private:
    QList<ShapeType> shapestore;     //dynamische Qliste des structs, kann wie c array verwendet werden

    QColor mBackgroundColor;
    QPen mPen;

    float mIntervalLength, mPreScale;
    int mStepCount, mScale=100;
    unsigned int mShapeIndex=0;
    bool optionCool;

    QPointF compute(float t,  float * pFloatIter1);           //dispatcher based on type

    QPointF compute_astroid(float t);
    QPointF compute_cycloid(float t);
    QPointF compute_huygens(float t);
    QPointF compute_hypo(float t);
    QPointF compute_line(float t);
    QPointF compute_circle(float t);
    QPointF compute_elipse(float t);
    QPointF compute_mandala(float t);
    QPointF compute_star(float t);
    QPointF compute_cloud(float t);
    QPointF compute_mandelb(float t,  float * pFloatIter1);
};

#endif // RENDERAREA_H
