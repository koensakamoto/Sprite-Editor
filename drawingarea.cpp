#include "drawingarea.h"

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>

DrawingArea::DrawingArea(Frame* frame, QWidget* parent = nullptr)
    : QWidget(parent), frame(frame) {
    setFixedSize(frame->getWidth(), frame->getHeight());
}

void DrawingArea::paintEvent(QPaintEvent*) override {
    QPainter painter(this);
    for (int y = 0; y < frame->getHeight(); y++) {
        for (int x = 0; x < frame->getWidth(); x++) {
            Pixel& p = frame->getPixel(x, y);
            QColor color(p.r, p.g, p.b, p.a);
            painter.setPen(color);
            painter.drawPoint(x, y);
        }
    }
}

void DrawingArea::mousePressEvent(QMouseEvent* event) override {
    drawPixel(event->pos());
}

void DrawingArea::mouseMoveEvent(QMouseEvent* event) override {
    drawPixel(event->pos());
}

void DrawingArea::drawPixel(const QPoint& pos) {
    if (pos.x() >= 0 && pos.x() < frame->getWidth() &&
        pos.y() >= 0 && pos.y() < frame->getHeight()) {

        frame->getPixel(pos.x(), pos.y()) = Pixel(0, 0, 0); // Draw Black
        update(); // Repaint
    }
}
