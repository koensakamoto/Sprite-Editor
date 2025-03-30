// This is our model.
#include "drawingarea.h"

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>

DrawingArea::DrawingArea(Frame frame, QWidget* parent)
    : QWidget(parent), frame(frame), frameVector{}, currFrameIndex(0) {
    setFixedSize(frame.getWidth(), frame.getHeight());
    frameVector.push_back(frame);
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
        // QImage img = frame.getImage();
        // QColor c = img.pixelColor(1, 1);
        // qDebug() << "r" << c.red() << "g" << c.green();
        // frameVector[currFrameIndex] = frame;
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
        frameVector[currFrameIndex] = frame;
        update();
    }
}

void DrawingArea::drawPixel(const QPoint& pos) {
    if (isWithinImageBounds(pos, frame.getImage())) {

        QPoint relativePos = convertToRelativeCoordinates(pos);

        // paint in square of pixel
        for (int row = 0 ; row < pixelSize; row ++){
            for (int col = 0 ; col < pixelSize ; col++){

                QPoint p(relativePos.x()+ row , relativePos.y() + col);
                frame.getImage().setPixelColor(p, brushColor);
            }
        }
    }
}

void DrawingArea::drawMultiplePixels(vector<QPoint> contiguousPixels) {
    if (contiguousPixels.empty()){
        qDebug() << " no pixels found";
    }

    for(QPoint pos: contiguousPixels){

        if (isWithinImageBounds(pos, frame.getImage())){

            QPoint relativePos = convertToRelativeCoordinates(pos);

            // int relativeClickX = std::round(x / pixelSize) * pixelSize;
            // int relativeClickY = std::round(y / pixelSize) * pixelSize;

            // paint in square of pixel
            for (int row = 0 ; row < pixelSize; row ++){
                for (int col = 0 ; col < pixelSize ; col++){

                    QPoint p(relativePos.x() + row , relativePos.y() + col);
                    frame.getImage().setPixelColor(p, brushColor);
                }
            }
            qDebug()<< relativePos.x() + " " + relativePos.y();
        }

    }
}


void DrawingArea::paintEvent(QPaintEvent*) {
    QPainter painter(this);
    // painter.drawImage(120, 20, frame.getImage());
}

void DrawingArea::setFrame(const Frame& otherFrame){
    this->frame = otherFrame;
}

void DrawingArea::setFrameVector(std::vector<Frame>& frameVector){
    this->frameVector = frameVector;
    frame = frameVector.at(0);
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

void DrawingArea::setPixelSize(int size){

    if (size > 1){
    pixelSize = size;
    frame.setPixelSize(pixelSize);
    }
}

std::vector<Frame>& DrawingArea::getFrames(){
    return frameVector;
}

bool DrawingArea::isWithinImageBounds(QPoint clickPos, QImage img){

    if (clickPos.x() >= 0 && clickPos.x() < img.width() && clickPos.y() >= 0 && clickPos.y() < img.height()) {

        return true;
    }
    return false;
}

QPoint DrawingArea::convertToRelativeCoordinates(QPoint p){

    int relativeX = std::round(p.x() / pixelSize) * pixelSize;
    int relativeY = std::round(p.y() / pixelSize) * pixelSize;

    return QPoint(relativeX,relativeY);
}
