#ifndef FRAME_H
#define FRAME_H

#include <QObject>
#include <pixel.h>

class Frame
{
private:

    int width;
    int height;
    Pixel grid[][];

public:


    /**
     * @brief Frame Creates a Frame of the specified dimensions. Units are in Pixels.
     * @param x Width of frame in Pixels.
     * @param y Length of frame in Pixels.
     */
    Frame(int x, int y);

    /**
     * @brief setPixel
     * @param x
     * @param y
     * @param r
     * @param b
     * @param g
     * @param a
     * @return True if pixel was sucessfully changed, false otherwise.
     */
    bool setPixel(int x, int y, unsigned char r, unsigned char b, unsigned char g, unsigned char a);

    /**
     * @brief deletePixel Pixel is "deleted" from the grid array. Pixel will not be shown.
     * @param x
     * @param y
     * @return
     */
    bool deletePixel(int x, int y);

    /**
     * @brief resize
     * @param x width of the new frame
     * @param y height of the new frame;
     */
    void resize(double scaleFactor);

    /**
     * @brief duplicate
     * @return A Frame object that is a copy of the original.
     */
    Frame duplicate();

    /**
     * @brief clear Erases every Pixel in the frame.
     */
    void clear();

    /**
     * @brief getAllContiguousPixels
     * @param x x-coordinate of the pixel selected.
     * @param y y-coordinate of the pixel selected.
     * @return
     */
    std::vector<Pixel> getAllContiguousPixels(int x, int y);
};

#endif // FRAME_H
