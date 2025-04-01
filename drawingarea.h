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

    int getSize();

    int getPixelSize();

signals:
    /**
     * @brief Signal that is emitted when the image is updated.
     * @param pixmap - Passes along the updated pixmap to be put into the drawing area QLabel in MainWindow.
     */
    void imageUpdated(const QPixmap &pixmap);

    /**
     * @brief Signal that is emitted when the preview is updated.
     * @param pixmap - Passes along the updated pixmap to be put into the preview QLabel in MainWindow.
     */
    void previewUpdated(const QPixmap& pixmap, const QPixmap& trueSizePixmap);

public slots:
    /**
     * @brief Used for updating the next frame in the Vector of frames stored.
     */
    void updateNextFrame();

    /**
     * @brief Provides a new FPS when the FPS slider is updated.
     * @param newFps - New FPS to be used for preview window.
     */
    void onUpdatedFps(int newFps);

    /**
     * @brief Sets the pixel size for the drawing area which acts as our way of sizing the image.
     * @param size - Size of the pixels.
     */
    void setPixelSize(int size);

    /**
     * @brief Used for when the preview play/pause is selected to play/pause the preview animation.
     */
    void previewSelected();

    // void previewTrueSizeSelected();

    /**
     * @brief Displays animation of all the frames at the actual size of the sprite in pixels.
     */
    // void previewFramesTrueSize();

    /**
     * @brief When frame that is being edited is changed, this updates the current frame variable to that index.
     * @param index - New index of the current frame.
     */
    void updateCurrentFrame(int index);

    /**
     * @brief Adds a new frame that can be edited.
     * @param index - Index of the new frame being added.
     * @param copyIndex - Index of the last frame if the new frame being added is a copy of that one.
     */
    void addFrame(int index, int copyIndex);

    /**
     * @brief Deletes a frame.
     * @param index - Index of the frame to be deleted.
     */
    void deleteFrame(int index);

    /**
     * @brief Mirrors the frame horizontally.
     */
    void mirrorHorizontally();

    /**
     * @brief Mirrors the frame vertically.
     */
    void mirrorVertically();

    /**
     * @brief Sets the current tool being used by the program.
     * @param tool - Tool being used.
     */
    void setCurrentTool(DrawingArea::PaintTool tool);

protected:
    /**
     * @brief When the mouse is pressed this method checks the current tool and begins drawing.
     * @param event - Where the mouse was pressed.
     */
    void mousePressEvent(QMouseEvent* event) override;

    /**
     * @brief When the mouse is moved, this method checks if it is drawing and then will draw.
     * @param event - Where the mouse was moved.
     */
    void mouseMoveEvent(QMouseEvent* event) override;

    /**
     * @brief When the mouse button is released, this method stops drawing.
     * @param event - Where the mouse button was released.
     */
    void mouseReleaseEvent(QMouseEvent* event) override;

private:
    /**
     * @brief Defines the currently selected tool from the toolbar. Can be either Eraser, Select Tool, Paint Bucket, and Pen.
     */
    PaintTool currentTool;

    /**
     * @brief Vector of frames that represent the frames that can be animated.
     */
    std::vector<QImage> frameVector;

    /**
     * @brief Represents whether the mouse is pressed down and drawing.
     */
    bool drawing;

    /**
     * @brief Represents the currently selected brush color.
     */
    QColor brushColor;

    /**
     * @brief FPS for the preview animations.
     */
    int fps = 10;

    /**
     * @brief Size of the main drawing area where the user can edit.
     */
    int size;

    /**
     * @brief Size of the pixel when drawing.
     */
    int pixelSize = 40;

    /**
     * @brief Index of frame currently being displayed in the main drawing area.
     */
    int currFrameIndex = 0;

    /**
     * @brief Used to navigate a 2D array in BFS.
     */
    int dRow[4];
    /**
     * @brief Used to navigate a 2D array in BFS.
     */
    int dCol[4];

    /**
     * @brief Private method for drawing the pixel on the image.
     * @param pos - Position of the pixel in the image to be drawn.
     */
    void drawPixel(const QPoint& pos);

    /**
     * @brief Represents whether the preview button is on or off.
     */
    bool previewOn = false;

    /**
     * @brief Sends the frames to be displayed in the preview label.
     */
    void previewFrames();

    /**
     * @brief Index of the current frame in the animation being displayed.
     */
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
     * @return - Vector of QPoints that represent the Contiguous Pixels.
     */
    vector<QPoint> getAllContiguousPixels(int x, int y);

    /**
     * @brief Helper method for BFS that determines if a pixel is already visited.
     * @param visited - 2D Vector of pixels already visited.
     * @param row - Rows of the 2D Array.
     * @param col - Columns of the 2D Array.
     * @param startColor - Current starting color for figuring out which pixels are Contiguous.
     * @return - Returns true if the pixel is contiguous, false if not.
     */
    bool isValid(vector<vector<bool>> visited, int row, int col, QColor& startColor);

    /**
     * @brief Breadth First Search used for the Paint Bucket and Select Tools.
     * @param visited - 2D Vector of bools for if a pixel was visited.
     * @param row - Rows of the 2D Array.
     * @param col - Columns of the 2D Array.
     * @param startColor - Current starting color for figuring out which pixels are Contiguous.
     * @return - Vector of the contiguous pixels.
     */
    vector<QPoint> BFS(vector<vector<bool>> visited, int row, int col, QColor& startColor);
};

#endif // DRAWINGAREA_H
