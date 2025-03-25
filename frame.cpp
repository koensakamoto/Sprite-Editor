#include "frame.h"
#include "pixel.h"
#include "point2d.h"
#include <queue>

// Frame::Frame(): width(0), height(0){}

Frame::Frame(int width, int height): width(width), height(height), grid{},
                                    dRow{ -1, 0, 1, 0 }, dCol{ 0, 1, 0, -1 }{}


bool Frame::setPixel(int x, int y, unsigned char r, unsigned char b, unsigned char g, unsigned char a){

    if (x > width || y > height){
        return false;
    }
    else {
        grid[x][y].setPixel(r, g, b, a);
        return true;
    }

}

bool Frame::deletePixel(int x, int y){

    if (x > width || y > height){
        return false;
    }
    else {
        grid[x][y].erasePixel();
        return true;
    }

}

Frame Frame::duplicate(){

    std::vector<std::vector<Pixel>> newGrid;

    for (int i  = 0; i < width; i++){
         for (int j = 0; j < height; j++){
            newGrid[i][j] = grid[i][j];
         }
    }

    Frame newFrame(width,height);
    newFrame.grid = newGrid;

    return newFrame;
}

void Frame::resize(double scaleFactor){

    int newX = scaleFactor*width;
    int newY = scaleFactor*height;

    std::vector<std::vector<Pixel>> newGrid;

    for (int i = 0; i < newX; i++){
        for (int j = 0; j < newY; j++){

            newGrid[i][j] = grid[(int)i*scaleFactor][(int)j*scaleFactor];

        }
    }

    this->grid = newGrid;
}

void Frame::clear(){
    this->grid = {};
}

std::vector<Pixel> Frame::getAllContiguousPixels(int x, int y){

    bool visited[width][height];

    std::vector<Pixel> contiguousPixels;

    BFS(visited, x, y, contiguousPixels);

    return contiguousPixels;

}

bool Frame::isValid(bool visited[][], int row, int col){

    // If cell lies out of bounds
    // should it be row>width?
    if (row < 0 || col < 0 ||
        row >= width || col >= height)
        return false;

    // If cell is already visited or not colored

    if (visited[row][col]){
        return false;
    }
    else if (!grid[row][col].isDisplayed()){
        return false;
    }

    // Otherwise
    return true;
}

 void Frame::BFS(bool visited[][], int row, int col)
{

    // Stores indices of the matrix cells

    // Mark the starting cell as visited
    // and push it into the queue

    std::queue<Point2D> q;

    Point2D p = Point2D(row,col);
    q.push(p);
    visited[row][col] = true;

    // Iterate while the queue
    // is not empty

    while (!q.empty())
    {
        Point2D cell = q.front();
        int x = cell.getX();
        int y = cell.getY();

        //System.out.print(grid[x][y] + " ");

        q.pop();

        // Go to the adjacent cells
        for(int i = 0; i < 4; i++)
        {
            int adjx = x + dRow[i];
            int adjy = y + dCol[i];

            if (isValid(visited, adjx, adjy))
            {
                Point2D p = Point2D(adjx,adjy);
                q.push(p);
                visited[adjx][adjy] = true;
            }
        }
    }
}


QJsonObject Frame::frameToQJson(){
        QJsonObject frameJson;
        int rowLength = (int)grid.size();
        int colLength = (int)grid.at(0).size();
        for(int row = 0 ; row < rowLength; row++ ){

            for(int col = 0; col < colLength; col++ ){
                const QString key = QString("(%1, %2)").arg(row).arg(col);
                Pixel currentPixel = this->grid.at(row).at(col);
                frameJson[key] = currentPixel.pixelToQJson();
            }

        }

        return frameJson;
    }


