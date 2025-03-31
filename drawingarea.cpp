// This is our model.
#include "drawingarea.h"

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>
#include <queue>
#include <QTimer>

DrawingArea::DrawingArea(QWidget* parent, int size):
    QWidget(parent), frameVector{}, currFrameIndex(0), size(size),
    dRow{ -1, 0, 1, 0 },
    dCol{ 0, 1, 0, -1 }
{

    QImage frame1 = QImage(size, size, QImage::Format_ARGB32);
    frame1.fill(Qt::white);
    frameVector.push_back(frame1);

    QImage frame2 = QImage(size, size, QImage::Format_ARGB32);
    frame2.fill(Qt::red);
    frameVector.push_back(frame2);

    QImage frame3 = QImage(size, size, QImage::Format_ARGB32);
    frame2.fill(Qt::green);
    frameVector.push_back(frame2);
}

void DrawingArea::setBrushColor(const QColor& color) {
    brushColor = color;
}

void DrawingArea::setUpCanvas() {
    emit imageUpdated(QPixmap::fromImage(frameVector[currFrameIndex]));
    update();
}

void DrawingArea::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton ) {
        drawing = true;
        QPoint pos = convertToRelativeCoordinates(event->pos());

        //drawPixel(pos);

        // call this to activate select tool
        drawMultiplePixels(getAllContiguousPixels(pos.x(), pos.y()));

        emit imageUpdated(QPixmap::fromImage(frameVector[currFrameIndex]));
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
        emit imageUpdated(QPixmap::fromImage(frameVector[currFrameIndex]));
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
    if (isWithinImageBounds(pos, frameVector[currFrameIndex])) {

        QPoint relativePos = convertToRelativeCoordinates(pos);

        // paint in square of pixel
        for (int row = 0 ; row < pixelSize; row ++){
            for (int col = 0 ; col < pixelSize ; col++){

                QPoint p(relativePos.x()+ row , relativePos.y() + col);
                frameVector[currFrameIndex].setPixelColor(p, brushColor);
            }
        }
    }
}

void DrawingArea::drawMultiplePixels(vector<QPoint> contiguousPixels) {
    if (contiguousPixels.empty()){
        //qDebug() << " no pixels found";
        return;
    }

    for(QPoint pos: contiguousPixels){

        if (isWithinImageBounds(pos, frameVector[currFrameIndex])){

            frameVector[currFrameIndex].setPixelColor(pos, brushColor);

        }

    }

    // scale image back to original size
    frameVector[currFrameIndex] = frameVector[currFrameIndex].scaled(size,size, Qt::KeepAspectRatioByExpanding);
}


void DrawingArea::paintEvent(QPaintEvent*) {
    QPainter painter(this);
    // painter.drawImage(120, 20, frame.getImage());
}

// void DrawingArea::setFrame(const QImage& otherFrame){
//     this->frame = otherFrame;
// }

void DrawingArea::setFrameVector(std::vector<QImage>& frameVector){
    this->frameVector = frameVector;
    //frame = frameVector.at(0);
    currFrameIndex = 0;
}

void DrawingArea::updateNextFrame(){
    emit imageUpdated(QPixmap::fromImage(frameVector[currFrameIndex]));
    update();
}

void DrawingArea::onUpdatedFps(int newFps){
    this->fps = newFps;
}

int DrawingArea::getFps(){
    return fps;
}

void DrawingArea::setPixelSize(int size){

    if (size > 1)
        pixelSize = size;
}

std::vector<QImage>& DrawingArea::getFrames(){
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

vector<QPoint> DrawingArea::getAllContiguousPixels(int x, int y){

    vector<QPoint> contiguousPixels = {};

    QColor startColor = frameVector[currFrameIndex].pixelColor(x,y);

    QColor backgroundColor = Qt::white;

    //Rejecting clicks to the background
    if (startColor.operator ==( backgroundColor)){
        //qDebug() << "early exit";
        QPoint p = QPoint(x,y);
        //qDebug() << p << startColor.red() << startColor.blue();
        return contiguousPixels;
    }

    // shrink image to make BFS quicker
    frameVector[currFrameIndex] = frameVector[currFrameIndex].scaled(size/pixelSize, size/pixelSize);

    vector<vector<bool>> visited(size/pixelSize, vector<bool>(size/pixelSize, false));

    qDebug() << frameVector[currFrameIndex].size();

    qDebug() << contiguousPixels;
    return BFS(visited, x/pixelSize, y/pixelSize, startColor);

}

bool DrawingArea::isValid(vector<vector<bool>> visited, int row, int col, QColor& startColor){

    // If cell lies out of bounds
    // should it be row>width?
    if (row < 0 || col < 0 ||
        row >= size/pixelSize || col >= size/pixelSize)
        return false;

    // Invalid cells are already visited or a different color from the original.
    if (visited[row][col]){
        return false;
    }

    if (frameVector[currFrameIndex].pixelColor(row,col).operator != (startColor)){
        return false;
    }

    return true;
}

vector<QPoint> DrawingArea::BFS(vector<vector<bool>> visited, int row, int col, QColor& startColor)
{


    // Stores indices of the pixels
    // Mark the starting pixel as visited
    // and push it into the queue

    std::queue<QPoint> q;
    vector<QPoint> contiguousPixels;

    QPoint p = QPoint(row,col);
    q.push(p);
    contiguousPixels.push_back(p);
    visited[row][col] = true;

    // Iterate through queue

    while (!q.empty())
    {
        QPoint cell = q.front();
        int x = cell.x();
        int y = cell.y();

        q.pop();

        // Go to the adjacent cells
        for(int i = 0; i < 4; i++)
        {
            int adjx = x + dRow[i];
            int adjy = y  + dCol[i];

            if (isValid(visited, adjx, adjy, startColor))
            {
                QPoint p = QPoint(adjx,adjy);
                q.push(p);
                contiguousPixels.push_back(p);
                visited[adjx][adjy] = true;
            }
        }
    }
    return contiguousPixels;
}

void DrawingArea::previewSelected(){
    previewOn = !previewOn;
    previewIndex = 0;

    if(previewOn) {
        previewFrames();
    }
}

void DrawingArea::previewFrames(){
    if(!previewOn || frameVector.empty()) {
        return;
    }

    if(previewIndex >= frameVector.size()) {
        previewIndex = 0;
    }

    QPixmap currPixMap = QPixmap::fromImage(frameVector[previewIndex]);
    QPixmap scaledPixmap = currPixMap.scaled(200, 200,
                                             Qt::KeepAspectRatio,
                                             Qt::SmoothTransformation);
    emit previewUpdated(scaledPixmap);

    previewIndex++;

    QTimer::singleShot(1000 / fps, this, &DrawingArea::previewFrames);
}



