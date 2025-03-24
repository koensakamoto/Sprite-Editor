#include "frame.h"
#include "pixel.h"
#include <queue>

// Frame::Frame(): width(0), height(0){}

Frame::Frame(int width, int height): width(width), height(height), grid{}{}


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

    std::vector<std::vector<Pixel>> newGrid;;

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
    BFS(visited,x,y);



}


bool Frame::isValid(bool vis[][], int row, int col)
{

    // If cell lies out of bounds
    // should it be row>width?
    if (row < 0 || col < 0 ||
        row >= width || col >= height)
        return false;

    // If cell is already visited or not colored

    if (vis[row][col]){
        return false;
    }
    else if (!grid[row][col].isColored()){
        return false;
    }


    // Otherwise
    return true;
}


 void BFS(bool visited[][], int row, int col)
{

    // Stores indices of the matrix cells

    // add point2d class
    std::queue<Point2D> q = new std::queue();

    //Queue<pair > q = new LinkedList<>();

    // Mark the starting cell as visited
    // and push it into the queue
    q.add(new pair(row, col));
    vis[row][col] = true;

    // Iterate while the queue
    // is not empty
    while (!q.isEmpty())
    {
        pair cell = q.peek();
        int x = cell.first;
        int y = cell.second;

        System.out.print(grid[x][y] + " ");

        q.remove();

        // Go to the adjacent cells
        for(int i = 0; i < 4; i++)
        {
            int adjx = x + dRow[i];
            int adjy = y + dCol[i];

            if (isValid(vis, adjx, adjy))
            {
                q.add(new pair(adjx, adjy));
                vis[adjx][adjy] = true;
            }
        }
    }
}
