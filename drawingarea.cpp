// This is our model.
#include "drawingarea.h"

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>
#include <queue>
#include <QTimer>

DrawingArea::DrawingArea(QWidget* parent, int size):
    QWidget(parent), frameVector{}, currFrameIndex{}, size(size),
    dRow{ -1, 0, 1, 0 },
    dCol{ 0, 1, 0, -1 },
    currentTool(PaintTool::PEN)
{

    QImage frame1 = QImage(size, size, QImage::Format_ARGB32);
    frame1.fill(Qt::white);
    frameVector.push_back(frame1);
}

void DrawingArea::setBrushColor(const QColor& color) {
    brushColor = color;
}

void DrawingArea::setUpCanvas() {
    emit imageUpdated(QPixmap::fromImage(frameVector[currFrameIndex]));
    update();
}

void DrawingArea::resetCanvas(int newSize, int newPixelSize) {
    // Clear existing frames
    frameVector.clear();

    // Clear all undo/redo history
    undoStacks.clear();
    redoStacks.clear();

    // Update size
    size = newSize;
    pixelSize = newPixelSize;

    // Create new blank frame
    QImage frame1 = QImage(size, size, QImage::Format_ARGB32);
    frame1.fill(Qt::white);
    frameVector.push_back(frame1);

    // Reset to first frame
    currFrameIndex = 0;

    // Update display
    emit imageUpdated(QPixmap::fromImage(frameVector[currFrameIndex]));
    update();
}

void DrawingArea::setCurrentTool(DrawingArea::PaintTool tool){
    currentTool = tool;
}

void DrawingArea::mousePressEvent(QMouseEvent *event){
    QPoint pos = QPoint(event->pos());

    // Map widget coordinates to image coordinates
    if (this->width() != size || this->height() != size) {
        float scaleX = (float)size / this->width();
        float scaleY = (float)size / this->height();
        pos = QPoint(pos.x() * scaleX, pos.y() * scaleY);
    }

    if (event->button() == Qt::LeftButton){
        // Save state before starting to draw
        saveState();

        drawing = true;
        switch(currentTool){

        case PaintTool::PEN:
            drawing = true;
            drawPixel(pos);
            break;

        case PaintTool::ERASER:
            brushColor = Qt::white;
            drawPixel(pos);
            break;

        case PaintTool::PAINTBUCKET:
            drawMultiplePixels(getAllContiguousPixels(pos.x(), pos.y()));
            break;
        }
    }
    emit imageUpdated(QPixmap::fromImage(frameVector[currFrameIndex]));
    update();
}
void DrawingArea::mouseMoveEvent(QMouseEvent *event){
    QPoint pos = QPoint(event->pos());

    // Map widget coordinates to image coordinates
    if (this->width() != size || this->height() != size) {
        float scaleX = (float)size / this->width();
        float scaleY = (float)size / this->height();
        pos = QPoint(pos.x() * scaleX, pos.y() * scaleY);
    }

    if (drawing){
        switch(currentTool){

        case PaintTool::PEN:
            drawing = true;
            drawPixel(pos);
            break;

        case PaintTool::ERASER:
            brushColor = Qt::white;
            drawPixel(pos);
            break;

        case PaintTool::PAINTBUCKET:
            break;
        }
    }
    emit imageUpdated(QPixmap::fromImage(frameVector[currFrameIndex]));
    update();
}
void DrawingArea::mouseReleaseEvent(QMouseEvent *event){
    if (event->button() == Qt::LeftButton){
        drawing = false;
    }
    update();
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
        qDebug() << " no pixels found";
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

void DrawingArea::mirrorHorizontally(){
    saveState();
    frameVector[currFrameIndex] = frameVector[currFrameIndex].mirrored(true, false);
    emit imageUpdated(QPixmap::fromImage(frameVector[currFrameIndex]));
    update();
}
void DrawingArea::mirrorVertically(){
    saveState();
    frameVector[currFrameIndex] = frameVector[currFrameIndex].mirrored(false,true);
    emit imageUpdated(QPixmap::fromImage(frameVector[currFrameIndex]));
    update();
}

void DrawingArea::setFrameVector(std::vector<QImage>& frameVector){
    this->frameVector = frameVector;
    //frame = frameVector.at(0);
    currFrameIndex = 0;

    // Clear undo/redo history when loading new frames
    undoStacks.clear();
    redoStacks.clear();
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

    // Paint bucket: Don't fill if clicking on same color as brush (no-op)
    if (startColor == brushColor && currentTool == PaintTool::PAINTBUCKET){
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


    // if (currentTool == PaintTool::PAINTBUCKET){
    //     if (frameVector[currFrameIndex].pixelColor(row,col).operator != (startColor)){
    //         return false;
    //     }
    // }

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

// void DrawingArea::previewTrueSizeSelected(){
//     // previewOn = !previewOn;
//     previewIndex = 0;

//     if(previewOn) {
//         previewFramesTrueSize();
//     }
// }

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

int DrawingArea::getSize(){
    return size;
}
int DrawingArea::getPixelSize(){
    return pixelSize;
}

// void DrawingArea::previewFramesTrueSize(){
//     if(!previewOn || frameVector.empty()) {
//         return;
//     }

//     if(previewIndex >= frameVector.size()) {
//         previewIndex = 0;
//     }

//     QPixmap currPixMap = QPixmap::fromImage(frameVector[previewIndex]);

//     int size = frameVector[previewIndex].height();

//     int scaledSize = size/pixelSize;
//     QPixmap scaledPixmap = currPixMap.scaled(scaledSize, scaledSize,
//                                              Qt::KeepAspectRatio,
//                                              Qt::SmoothTransformation);
//     emit previewUpdated(scaledPixmap);


//     previewIndex++;

//     //QTimer::singleShot(1000 / fps, this, &DrawingArea::previewFramesTrueSize);

// }
void DrawingArea::addFrame(int newPixelSize, int copyIndex){

    // if index is -1 create new QImage
    QImage newFrame;

    if(copyIndex == -1){
        newFrame = QImage(size, size, QImage::Format_ARGB32);
        newFrame.fill(Qt::white);
    }else {
        QImage copyImage = frameVector.at(copyIndex);
        newFrame = copyImage;
    }
    frameVector.push_back(newFrame);



}

void DrawingArea::deleteFrame(int index) {
    if (index >= 0 && index < (int)frameVector.size()) {
        frameVector.erase(frameVector.begin() + index);

        // Remove undo/redo history for deleted frame
        undoStacks.erase(index);
        redoStacks.erase(index);

        // Shift all frame indices down for frames after the deleted one
        std::map<int, std::vector<QImage>> newUndoStacks;
        std::map<int, std::vector<QImage>> newRedoStacks;

        for (auto& pair : undoStacks) {
            int frameIndex = pair.first;
            if (frameIndex > index) {
                newUndoStacks[frameIndex - 1] = pair.second;
            } else if (frameIndex < index) {
                newUndoStacks[frameIndex] = pair.second;
            }
        }

        for (auto& pair : redoStacks) {
            int frameIndex = pair.first;
            if (frameIndex > index) {
                newRedoStacks[frameIndex - 1] = pair.second;
            } else if (frameIndex < index) {
                newRedoStacks[frameIndex] = pair.second;
            }
        }

        undoStacks = newUndoStacks;
        redoStacks = newRedoStacks;
    }
    updateNextFrame();
}

void DrawingArea::updateCurrentFrame(int index){
    currFrameIndex = index;
    // Don't clear stacks - each frame keeps its own history
    updateNextFrame();
}

void DrawingArea::saveState() {
    // Save current frame state to undo stack for this specific frame
    undoStacks[currFrameIndex].push_back(frameVector[currFrameIndex].copy());

    // Limit undo history to MAX_HISTORY
    if (undoStacks[currFrameIndex].size() > MAX_HISTORY) {
        undoStacks[currFrameIndex].erase(undoStacks[currFrameIndex].begin());
    }

    // Clear redo stack for this frame when new action is performed
    redoStacks[currFrameIndex].clear();
}

void DrawingArea::undo() {
    if (!canUndo()) return;

    // Save current state to redo stack for this frame
    redoStacks[currFrameIndex].push_back(frameVector[currFrameIndex].copy());

    // Restore previous state from undo stack for this frame
    frameVector[currFrameIndex] = undoStacks[currFrameIndex].back().copy();
    undoStacks[currFrameIndex].pop_back();

    // Update display
    emit imageUpdated(QPixmap::fromImage(frameVector[currFrameIndex]));
    update();
}

void DrawingArea::redo() {
    if (!canRedo()) return;

    // Save current state to undo stack for this frame
    undoStacks[currFrameIndex].push_back(frameVector[currFrameIndex].copy());

    // Restore state from redo stack for this frame
    frameVector[currFrameIndex] = redoStacks[currFrameIndex].back().copy();
    redoStacks[currFrameIndex].pop_back();

    // Update display
    emit imageUpdated(QPixmap::fromImage(frameVector[currFrameIndex]));
    update();
}

bool DrawingArea::canUndo() const {
    // Check if current frame has undo history
    auto it = undoStacks.find(currFrameIndex);
    return it != undoStacks.end() && !it->second.empty();
}

bool DrawingArea::canRedo() const {
    // Check if current frame has redo history
    auto it = redoStacks.find(currFrameIndex);
    return it != redoStacks.end() && !it->second.empty();
}



