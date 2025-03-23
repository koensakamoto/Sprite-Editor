#include "frame.h"
#include "pixel.h"

// Frame::Frame(): width(0), height(0){}

Frame::Frame(int width, int height): width(width), height(height){
    this->grid = Pixel[width][height];
}


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
    }

}

Frame Frame::duplicate(){

    Pixel newGrid[width][height];

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

    Pixel newGrid[newX][newY];

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
