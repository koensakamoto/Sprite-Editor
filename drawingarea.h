#ifndef DRAWINGAREA_H
#define DRAWINGAREA_H

#include <QWidget>

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <vector>

using std::vector;

/**
 * @brief The DrawingArea class defines the area in the MainWindow that allows for drawing. This class keeps track of the mouse and tools
 * so that the user can correctly interact with the program.
 * This is our Model.
 */
class DrawingArea : public QWidget {
    Q_OBJECT

public:
    enum class PaintTool {
        PEN,
        PAINTBUCKET,
        SELECT,
        ERASER
    };

    /**
     * @brief This is the constructor for our drawing area that takes a parent Widget and a size as parameters.
     * @param parent - The parent Widget that will be the MainWindow.
     * @param size - Size of the drawing area.
     */
    DrawingArea(QWidget* parent, int size);

    /**
     * @brief Sets the brush color of the color tools.
     * @param color - Color that will be set.
     */
    void setBrushColor(const QColor& color);

    /**
     * @brief Sets up the canvas when MainWindow is run.
     */
    void setUpCanvas();

    /**
     * @brief Sets the frame vector when loading a file.
     * @param frames - Vector of frames that will be be loaded.
     */
    void setFrameVector(std::vector<QImage>& frames);

    /**
     * @brief Gets the FPS of the preview windows.
     * @return - The FPS of the preview windows as an integer.
     */
    int getFps();

    /**
     * @return The Frames used to store the QImages DrawingArea is drawing onto.
     */
    std::vector<QImage>& getFrames();

signals:
    /**
     * @brief Signal that is emitted when the image is updated.
     * @param pixmap - Passes along the updated pixmap to be put into the QLabel in MainWindow.
     */
    void imageUpdated(const QPixmap &pixmap);

    void previewUpdated(const QPixmap& pixmap);

public slots:
    void updateNextFrame();

    void onUpdatedFps(int newFps);

    void setPixelSize(int size);

    void previewSelected();

    void updateCurrentFrame(int index);
    void addFrame(int index, int copyIndex);
    void deleteFrame(int index);

    void mirrorHorizontally();
    void mirrorVertically();

    void setCurrentTool(DrawingArea::PaintTool tool);

protected:

    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;

private:
    PaintTool currentTool;

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

    bool previewOn = false;

    void previewFrames();

    int previewIndex = 0;


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
