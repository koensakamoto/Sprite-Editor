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
    DrawingArea(Frame frame, QWidget* parent);

    void setBrushColor(const QColor& color);

    void setUpCanvas();

    void setFrame(const Frame& otherFrame);

    void setFrameVector(std::vector<Frame>& frames);

    int getFps();

    /**
     * @return The Frames used to store the QImages DrawingArea is drawing onto.
     */
    std::vector<Frame>& getFrames();

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
    Frame frame;
    std::vector<Frame> frameVector;
    bool drawing;
    QColor brushColor;
    int fps = 10;
    int pixelSize = 40;
    int currFrameIndex;


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
};

#endif // DRAWINGAREA_H
