#ifndef FRAME_H
#define FRAME_H

#include <vector>
#include <QObject>
#include <QJsonObject>
#include <QImage>
#include <point2d.h>

using std::vector;

class Frame
{
private:

    QImage image;
    int width;
    int height;

    // Used for BFS
    int dRow[4];
    int dCol[4];

    // Helper method for BFS of grid.
    bool isValid(vector<vector<bool>> visited, int row, int col, QColor startColor);

    //BFS used to getAllContiguousPixels()
    void BFS(vector<vector<bool>> visited, int row, int col, vector<Point2D> contiguousPixels, QColor startColor);

public:

    /**
     * @brief Frame Creates a Frame of the specified dimensions. Units are in Pixels.
     * @param x Width of frame in Pixels.
     * @param y Length of frame in Pixels.
     */
    Frame(int x, int y);

    /**
     * @brief Frame Constructs a Frame from a QImage instance.
     */
    Frame(QImage image);

    // /**
    //  * @brief setPixel
    //  * @param x
    //  * @param y
    //  * @param r
    //  * @param b
    //  * @param g
    //  * @param a
    //  * @return True if pixel was sucessfully changed, false otherwise.
    //  */
    // bool setPixel(int x, int y, unsigned char r, unsigned char b, unsigned char g, unsigned char a);

    // /**
    //  * @brief deletePixel Pixel is "deleted" from the grid array. Pixel will not be shown.
    //  * @param x
    //  * @param y
    //  * @return
    //  */
    // bool deletePixel(int x, int y);

    /**
     * @brief resize
     * @param x width of the new frame
     * @param y height of the new frame;
     */
    void resize(int x, int y);

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
     * @brief getAllContiguousPixels returns a list of Point2D objects. Each point is an x and y
     * coordinate for a pixel that is continous with the original pixel from x and y.
     * @param x x-coordinate of the pixel selected.
     * @param y y-coordinate of the pixel selected.
     * @return
     */
    vector<Point2D> getAllContiguousPixels(int x, int y);

    /**
    * @brief Converts frame data to a QJsonObject.
    * @return A QJsonObject with the frame's data.
    */
    QJsonObject frameToQJson(int frameId);
public slots:
    /**
     * @brief getImage from model
     * @return A QImage representing the grid of pixels of an image.
     */
    void recieveAndUpdateImage(QImage);


signals:
    void sendImage(QImage image);
    void sendContiguousPixels(vector<Point2D>);
};

#endif // FRAME_H
