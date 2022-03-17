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

    enum ShapeType { Astroid, Cycloid, HuygensCycloid, HypoCycloid };

    void setBackgroundcolor(QColor bgColor) {mBackgroundColor = bgColor; }  //setter, inline weil kurz
    QColor backgroundColor() const { return mBackgroundColor; }   //getter, const schützt die Member vor Änderungen

    void setShape (ShapeType shape) { mShape = shape; }
    ShapeType shape() const { return mShape; }

protected:
    void paintEvent(QPaintEvent *event) override;

signals:

private:
    QColor mBackgroundColor;
    QColor mShapeColor;
    ShapeType mShape;

    QPointF compute_astroid(float t);
};

#endif // RENDERAREA_H
