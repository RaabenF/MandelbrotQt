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

    enum ShapeType { Astroid, Cycloid, HuygensCycloid, HypoCycloid, Line };

    void setBackgroundcolor(QColor bgColor) {mBackgroundColor = bgColor; repaint();}  //setter, inline weil kurz
    QColor backgroundColor() const { return mBackgroundColor; }             //getter, const schützt die Member vor Änderungen

    void setShape (ShapeType shape) { mShape = shape; on_shape_changed(); repaint(); }
    ShapeType shape() const { return mShape; }

    void setInterval(int value) { mIntervalLength = value; repaint(); }
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
    ShapeType mShape;
    bool optionCool=true;

    float mIntervalLength;
    float mScale;
    int mStepCount = 256;

    QPointF compute(float t);           //dispatcher based on type
    QPointF compute_astroid(float t);
    QPointF compute_cycloid(float t);
    QPointF compute_huygens(float t);
    QPointF compute_hypo(float t);
    QPointF compute_line(float t);
    void on_shape_changed();
};

#endif // RENDERAREA_H
