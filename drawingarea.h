#ifndef DRAWINGAREA_H
#define DRAWINGAREA_H

#include <QWidget>

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include "frame.h"

class DrawingArea : public QWidget {
    Q_OBJECT

public:
    DrawingArea(QWidget* parent, int size);

    void setBrushColor(const QColor& color);

    void setUpCanvas();

    // void setFrame(const Frame& otherFrame);

    void setFrameVector(std::vector<QImage>& frames);

    int getFps();

    /**
     * @return The Frames used to store the QImages DrawingArea is drawing onto.
     */
    std::vector<QImage>& getFrames();

signals:
    void imageUpdated(const QPixmap &pixmap);

public slots:
    void updateNextFrame();

    void onUpdatedFps(int newFps);

    void setPixelSize(int size);

protected:
    void paintEvent(QPaintEvent*) override;

    void mousePressEvent(QMouseEvent* event) override;

    void mouseMoveEvent(QMouseEvent* event) override;

    void mouseReleaseEvent(QMouseEvent* event) override;

private:

    std::vector<QImage> frameVector;

    bool drawing;

    QColor brushColor;

    int fps = 10;

    int size;

    int pixelSize = 100;

    int currFrameIndex = 0;

    // Used for BFS
    int dRow[4];
    int dCol[4];

    void drawPixel(const QPoint& pos);


    /**
     * @brief drawMultiplePixels Draws all pixels at pixelSize from a vector of QPoints.
     * Each QPoint represents the (x,y) of the top left corner of the pixel to be drawn.
     * @param contiguousPixels
     */
    void drawMultiplePixels(std::vector<QPoint> contiguousPixels);

    /**
     * isWithinImageBounds Returns true if the QPoint representing the click cooridinates is
     * within the bounds of the QImage img. Returns false if the click is out of image bounds.
     */
    bool isWithinImageBounds(QPoint clickPos, QImage img);

    /**
     * Turns a QPoint p from screen coordinates to relative coordinates within the DrawingFrame's image.
     * (0,0) is the top left corner of the DrawingFrame's image.
     */
    QPoint convertToRelativeCoordinates(QPoint p);

    /**
     * @brief getAllContiguousPixels returns a list of Point2D objects. Each point is an x and y
     * coordinate for a pixel that is continous with the original pixel from x and y.
     * @param x x-coordinate of the pixel selected.
     * @param y y-coordinate of the pixel selected.
     * @return
     */
    vector<QPoint> getAllContiguousPixels(int x, int y);

    // Helper method for BFS of grid.
    bool isValid(vector<vector<bool>> visited, int row, int col, QColor& startColor);

    //BFS used to getAllContiguousPixels()
    vector<QPoint> BFS(vector<vector<bool>> visited, int row, int col, QColor& startColor);
};

#endif // DRAWINGAREA_H
