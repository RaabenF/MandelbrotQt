#ifndef RENDERAREA_H
#define RENDERAREA_H

#include <QWidget>

class RenderArea : public QWidget
{
    Q_OBJECT
public:
    explicit RenderArea(QWidget *parent = nullptr);

    QSize minimumSizeHint() const override; // Q_DECL_OVERRIDE; im Tutorial ist deprecated
    QSize sizeHint() const override;

    enum ShapeType :int { Astroid, Cycloid, HuygensCycloid, HypoCycloid, Line, Circle, Elipse, Mandel };    //enum typ int
    ShapeType EnumOfIndex(int i) { return static_cast<ShapeType>(i); }

    QStringList ShapeList;

    void setBackgroundColor(QColor color) {mBackgroundColor = color; repaint();}  //setter, inline weil kurz
    QColor backgroundColor() const { return mBackgroundColor; }             //getter, const schützt die Member vor Änderungen

    void setShapeColor(QColor color) {mShapeColor = color; repaint();}  //setter, inline weil kurz
    QColor ShapeColor() const { return mShapeColor; }             //getter, const schützt die Member vor Änderungen

    void setShape (ShapeType shape);
    //void setShape (QString shapename);    //geht so nicht
    void setShape (int row);

    void setInterval(float value) { mIntervalLength = value; repaint(); }
    float Interval() const { return mIntervalLength; }

    void setScale(int scale) { mScale = scale; repaint(); }     //int->float is ok da nur ganze werte
    float scale() const { return mScale; }

    void setStepCount(int count) {mStepCount = count; repaint(); }
    int stepCount () const {return mStepCount; }

    void setCool(bool Cool) { optionCool = Cool; repaint(); }
    bool Cool() const { return optionCool; }

protected:
    void paintEvent(QPaintEvent *event) override;

signals:

private:
    QColor mBackgroundColor;
    QColor mShapeColor;
    ShapeType mShape;   //nicht mehr verwendet
    int mShapeIndex;
    bool optionCool=true;

    float mIntervalLength, mScale;
    float step, tempInterval;
    int mStepCount = 256;

    QPointF compute(float t);           //dispatcher based on type
    QPointF compute_astroid(float t);
    QPointF compute_cycloid(float t);
    QPointF compute_huygens(float t);
    QPointF compute_hypo(float t);
    QPointF compute_line(float t);
    QPointF compute_circle(float t);
    QPointF compute_elipse(float t);
    QPointF compute_mandel(float t);
    void on_shape_changed();
};

#endif // RENDERAREA_H
