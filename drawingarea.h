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
    DrawingArea(Frame frame, QWidget* parent);

    void setBrushColor(const QColor& color);

    void setUpCanvas();

    void setFrame(Frame otherFrame);

    int getFps();

signals:
    void imageUpdated(const QPixmap &pixmap);

public slots:
    void updateNextFrame();

    void onUpdatedFps(int newFps);

    void setPixelSize(int size);

protected:
    void paintEvent(QPaintEvent*) override;

    void mousePressEvent(QMouseEvent* event) override;

    void mouseMoveEvent(QMouseEvent* event) override;

    void mouseReleaseEvent(QMouseEvent* event) override;

private:
    Frame frame;
    bool drawing;
    QColor brushColor;

    void drawPixel(const QPoint& pos);

    int fps = 10;
    int pixelSize = 20;
};

#endif // DRAWINGAREA_H
