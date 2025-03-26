#include "drawingarea.h"

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>

DrawingArea::DrawingArea(Frame* frame, QWidget* parent)
    : QWidget(parent), frame(frame) {
    setFixedSize(frame->getWidth(), frame->getHeight());
}

void DrawingArea::setBrushColor(const QColor& color) {
    brushColor = color;
}

void DrawingArea::updateCanvas() {
    update();
}

void DrawingArea::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton ) {
        drawing = true;
        drawPixel(event->pos());
    }
}

void DrawingArea::mouseMoveEvent(QMouseEvent* event) {
    if (drawing) {
        drawPixel(event->pos());
    }
}

void DrawingArea::mouseReleaseEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        drawing = false;
    }
}

void DrawingArea::drawPixel(const QPoint& pos) {
    if (!frame) return;

    if (pos.x() >= 0 && pos.x() < frame->getImage().width() &&
        pos.y() >= 0 && pos.y() < frame->getImage().height()) {
        frame->getImage().setPixelColor(pos, brushColor);
        update(); // Refresh the drawing
    }
}
