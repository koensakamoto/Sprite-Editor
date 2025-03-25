#include "pixel.h"

Pixel::Pixel(): red(0), green(0), blue(0), alpha(0), displayed(false) {}

Pixel::Pixel(unsigned char r, unsigned char g, unsigned char b,  unsigned char a):red(r), green(g), blue(b), alpha(a), displayed(true) {}

void Pixel::erasePixel(){
    red = 0;
    green = 0;
    blue = 0;
    alpha = 0;
    displayed = false;

}

void Pixel::setPixel(unsigned char r, unsigned char g, unsigned char b,  unsigned char a){
    red = r;
    blue = b;
    green = g;
    alpha = a;
    displayed = true;

}

bool Pixel::isDisplayed(){
    return displayed;
}

QJsonObject Pixel::pixelToQJson() {
    QJsonObject pixelJson;
    pixelJson["red"] = this->red;
    pixelJson["blue"] = this->blue;
    pixelJson["green"] = this->green;
    pixelJson["alpha"] = this->alpha;
    pixelJson["displayed"] = this->displayed;

    return pixelJson;
}
