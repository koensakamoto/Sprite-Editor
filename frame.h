#ifndef FRAME_H
#define FRAME_H

#include <vector>
#include <QObject>
#include <pixel.h>
#include <QJsonObject>

class Frame
{
private:

    int width = 5;
    int height = 5;
    std::vector<std::vector<Pixel>> grid;

    // Helper method for BFS of grid.
    bool isValid(int row, int col);

    //BFS used to getAllContiguousPixels()
    void BFS(std::vector<std::vector<bool>> visited, int row, int col);

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

    /**
    * @brief Converts frame data to a QJsonObject.
    * @return A QJsonObject with the frame's data.
    */
    QJsonObject frameToQJson();
};

#endif // FRAME_H
