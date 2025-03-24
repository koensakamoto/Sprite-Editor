#ifndef PIXEL_H
#define PIXEL_H

#include <QObject>

class Pixel
{

private:

    // red, green and blue values ranging from 0 to 255.
    unsigned char red;
    unsigned char green;
    unsigned char blue;

    // transparency
    unsigned char alpha;

    // If true, the pixel should be displayed, if false, the pixel is not displayed.
    bool displayed;

public:

    /**
     * @brief Pixel Default constructor.
     */
    Pixel();

    /**
     * @brief Pixel creates a Pixel object with the specified RGBA values.
     * @param r Value from 0-255 representing the red value of a pixel.
     * @param g Value from 0-255 representing the green value of a pixel.
     * @param b Value from 0-255 representing the blue value of a pixel.
     * @param a Value from 0-255 representing the alpha value of a pixel.
     */
    Pixel(unsigned char r, unsigned char g, unsigned char b, unsigned char a);

    /**
     * @brief setPixel Sets the RGBA values of a pixel, and isColored to true.
     * @param r Value from 0-255 representing the red value of a pixel.
     * @param g Value from 0-255 representing the green value of a pixel.
     * @param b Value from 0-255 representing the blue value of a pixel.
     * @param a Value from 0-255 representing the alpha value of a pixel.
     */
    void setPixel(unsigned char r, unsigned char g, unsigned char b,  unsigned char a);

    /**
     * @brief erasePixel Sets isColored to false. Deletes previous color information by
     * setting RGBA to 0.
     */
    void erasePixel();

    /**
     * @return The value is the isDisplayed flag. True if the pixel is displayed.
     */
    bool isDisplayed();

};

#endif // PIXEL_H
