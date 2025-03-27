#include "drawingarea.h"

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>

DrawingArea::DrawingArea(Frame frame, QWidget* parent)
    : QWidget(parent), frame(frame) {
    setFixedSize(frame.getWidth(), frame.getHeight());
    QPixmap pixmap = QPixmap::fromImage(frame.getImage());
}

void DrawingArea::setBrushColor(const QColor& color) {
    brushColor = color;
}

void DrawingArea::updateCanvas() {
    QPixmap pixmap = QPixmap::fromImage(frame.getImage());
    emit imageUpdated(pixmap);
    update();
}

void DrawingArea::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton ) {
        drawing = true;
        drawPixel(event->pos());
        QPixmap pixmap = QPixmap::fromImage(frame.getImage());
        emit imageUpdated(pixmap);
        update();
    }
}

void DrawingArea::mouseMoveEvent(QMouseEvent* event) {
    if (drawing) {
        drawPixel(event->pos());
        emit imageUpdated(pixmap);
        update();
    }
}

void DrawingArea::mouseReleaseEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        drawing = false;
        update();
    }
}

void DrawingArea::drawPixel(const QPoint& pos) {
    if (pos.x() >= 0 && pos.x() < frame.getImage().width() &&
        pos.y() >= 0 && pos.y() < frame.getImage().height()) {
        frame.getImage().setPixelColor(pos, brushColor);
        update();
    }
}

void DrawingArea::paintEvent(QPaintEvent*) {
    QPainter painter(this);
    painter.drawImage(120, 20, frame.getImage());
    emit imageUpdated(pixmap);
}
