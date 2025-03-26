#include "frame.h"
#include "point2d.h"
#include <queue>
#include <QJsonArray>

using std::vector;

Frame::Frame(int width, int height):image(QImage(width, height, QImage::Format_ARGB32)),
                                    width(width), height(height),
                                    dRow{ -1, 0, 1, 0 },
                                    dCol{ 0, 1, 0, -1 }
                                    {}

Frame::Frame(QImage image): image(image), width(image.width()), height(image.height()),
                            dRow{ -1, 0, 1, 0 },
                            dCol{ 0, 1, 0, -1 }
                            {}


// bool Frame::setPixel(int x, int y, unsigned char r, unsigned char b, unsigned char g, unsigned char a){

//     if (x > width || y > height){
//         return false;
//     }
//     else {
       // QPoint pt = QPoint(x,y);
//         QColor color = QColor()
//         image.setPixelColor(pt,);
//     }

// }

// bool Frame::deletePixel(int x, int y){

//     if (x > width || y > height){
//         return false;
//     }
//     else {
//         grid[x][y].erasePixel();
//         return true;
//     }

// }

Frame Frame::duplicate(){

    QImage copied = this->image.copy(0,0,width,height);
    return Frame(copied);
}

void Frame::resize(int width, int height){

    // int newX = scaleFactor*width;
    // int newY = scaleFactor*height;

    // std::vector<std::vector<Pixel>> newGrid;

    // for (int i = 0; i < newX; i++){
    //     for (int j = 0; j < newY; j++){

    //         newGrid[i][j] = grid[(int)i*scaleFactor][(int)j*scaleFactor];

    //     }
    // }

    // this->grid = newGrid;

    this->image = image.scaled(width,height);
    this->width = width;
    this->height = height;

    sendImage(this->image);
}

void Frame::clear(){
    this->image = QImage(width, height, QImage::Format_ARGB32);
    sendImage(this->image);
}

vector<Point2D> Frame::getAllContiguousPixels(int x, int y){

    vector<Point2D> contiguousPixels;
    vector<vector<bool>> visited;


    QColor startColor = image.pixelColor(x,y);
    QColor backgroundColor = QColor(255,255,255,255);


    //Rejecting clicks to the background
    if (startColor == backgroundColor){
        return contiguousPixels;
    }


    BFS(visited, x, y, contiguousPixels, startColor);

    sendContiguousPixels(contiguousPixels);

    // may become return type void
    return contiguousPixels;

}

bool Frame::isValid(vector<vector<bool>> visited, int row, int col, QColor startColor){

    // If cell lies out of bounds
    // should it be row>width?
    if (row < 0 || col < 0 ||
        row >= width || col >= height)
        return false;

    // Invalid cells are already visited or a different color from the original.
    if (visited[row][col]){
        return false;
    }

    else if (image.pixelColor(row,col).operator != (startColor)){
        return false;
    }

    return true;
}

 void Frame::BFS(vector<vector<bool>> visited, int row, int col, vector<Point2D> contiguousPixels, QColor startColor)
{

    // Stores indices of the matrix cells

    // Mark the starting cell as visited
    // and push it into the queue

    std::queue<Point2D> q;

    Point2D p = Point2D(row,col);
    q.push(p);
    contiguousPixels.push_back(p);
    visited[row][col] = true;

    // Iterate while the queue
    // is not empty

    while (!q.empty())
    {
        Point2D cell = q.front();
        int x = cell.getX();
        int y = cell.getY();

        q.pop();

        // Go to the adjacent cells
        for(int i = 0; i < 4; i++)
        {
            int adjx = x + dRow[i];
            int adjy = y + dCol[i];

            if (isValid(visited, adjx, adjy, startColor))
            {
                Point2D p = Point2D(adjx,adjy);
                q.push(p);
                contiguousPixels.push_back(p);
                visited[adjx][adjy] = true;
            }
        }
    }
}



QJsonObject Frame::frameToQJson(int frameId) {
    QJsonObject frameJson;
    QJsonArray rowsArray;

    int rowLength = grid.size();
    int colLength = grid.at(0).size();

    for (int row = 0; row < rowLength; row++) {
        QJsonArray colsArray;
        for (int col = 0; col < colLength; col++) {
            Pixel currentPixel = grid.at(row).at(col);
            colsArray.append(currentPixel.pixelToQJson());
        }
        rowsArray.append(colsArray);
    }

    const QString frameName = QString("Frame %1").arg(frameId);
    frameJson[frameName] = rowsArray;
    return frameJson;
}



