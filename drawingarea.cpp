// This is our model.
#include "drawingarea.h"

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>

DrawingArea::DrawingArea(Frame frame, QWidget* parent)
    : QWidget(parent), frame(frame) {
    setFixedSize(frame.getWidth(), frame.getHeight());
}

void DrawingArea::setBrushColor(const QColor& color) {
    brushColor = color;
}

void DrawingArea::setUpCanvas() {
    emit imageUpdated(QPixmap::fromImage(frame.getImage()));
    update();
}

void DrawingArea::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton ) {
        drawing = true;
        drawPixel(event->pos());
        emit imageUpdated(QPixmap::fromImage(frame.getImage()));
        update();
    }
}

void DrawingArea::mouseMoveEvent(QMouseEvent* event) {
    if (drawing) {
        drawPixel(event->pos());
        emit imageUpdated(QPixmap::fromImage(frame.getImage()));
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

    }
    frame.getImage().setPixelColor(pos, brushColor);
}

void DrawingArea::paintEvent(QPaintEvent*) {
    QPainter painter(this);
    // painter.drawImage(120, 20, frame.getImage());
}

void DrawingArea::setFrame(Frame otherFrame){
    this->frame = otherFrame;
}

void DrawingArea::updateNextFrame(){
    emit imageUpdated(QPixmap::fromImage(frame.getImage()));
    update();
}

void DrawingArea::onUpdatedFps(int newFps){
    this->fps = newFps;
}

int DrawingArea::getFps(){
    return fps;
}

