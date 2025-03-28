#include "frame.h"
#include "point2d.h"
#include <queue>
#include <QJsonArray>

using std::vector;

Frame::Frame(int width, int height):image(QImage(width, height, QImage::Format_ARGB32)),
                                    width(width), height(height),
                                    dRow{ -1, 0, 1, 0 },
                                    dCol{ 0, 1, 0, -1 }
    {
    image.fill(Qt::white);
    }

Frame::Frame(QImage image): image(image), width(image.width()), height(image.height()),
                            dRow{ -1, 0, 1, 0 },
                            dCol{ 0, 1, 0, -1 }
    {
    image.fill(Qt::white);
    }


Frame Frame::duplicate(){

    QImage copied = this->image.copy(0,0,width,height);
    return Frame(copied);
}

int Frame::getHeight() {
    return height;
}

int Frame::getWidth() {
    return width;
}

QImage& Frame::getImage() {
    return image;
}

void Frame::resize(int width, int height){

    this->image = image.scaled(width,height);
    this->width = width;
    this->height = height;

    // Frame::sendImage(this->image);
}

void Frame::clear(){
    this->image = QImage(width, height, QImage::Format_ARGB32);
    // Frame::sendImage(this->image);
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

    // Frame::sendContiguousPixels(contiguousPixels);

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

// Point2D Frame::scaleFromImage(int x, int y){

//     int nextX = x/pixelSize;
//     int nextY = y/pixelSize;


// }

