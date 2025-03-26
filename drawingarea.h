#ifndef DRAWINGAREA_H
#define DRAWINGAREA_H

#include <QWidget>

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include "frame.h"

class DrawingArea : public QWidget {
    Q_OBJECT

public:
    DrawingArea(Frame* frame, QWidget* parent = nullptr);

protected:
    void paintEvent(QPaintEvent*) override;

    void mousePressEvent(QMouseEvent* event) override;

    void mouseMoveEvent(QMouseEvent* event) override;

private:
    Frame* frame;

    void drawPixel(const QPoint& pos);
};

#endif // DRAWINGAREA_H
