#include "pixel.h"

Pixel::Pixel(): red(0), green(0), blue(0), alpha(0), colored(false) {}

void Pixel::erasePixel(){
    red = 0;
    green = 0;
    blue = 0;
    alpha = 0;
    colored = false;

}

void Pixel::setPixel(unsigned char r, unsigned char g, unsigned char b,  unsigned char a){
    red = r;
    blue = b;
    green = g;
    alpha = a;
    colored = true;

}

bool Pixel::isColored(){
    return colored;
}
