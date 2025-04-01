#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QColorDialog>
#include <vector>
#include "drawingarea.h"

/**
 * This is the main window class that represents the main window of our program.
 */

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief MainWindow - Creates the Main Window, its UI, signals and slots for our program
     * @param parent
     */
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    /**
     * @brief onPaintBucketClicked - Handles tool selection for the Paint Bucket
     */
    void onPaintBucketClicked();
    /**
     * @brief onEraserClicked - Handles tool selection for the Eraser
     */
    void onEraserClicked();
    /**
     * @brief onSelectToolClicked - Handles tool selection for the Select Tool
     */
    void onSelectToolClicked();
    /**
     * @brief onPenClicked - Handles tool selection for the Pen
     */
    void onPenClicked();
    /**
     * @brief saveClicked - Handles the save button in the menu bar
     */
    void saveClicked();
    /**
     * @brief loadClicked - Handles the load button in the menu bar
     */
    void loadClicked();
    /**
     * @brief updatedPreviewFrame - Updates the pixmap in the preview window to reflect the current frame
     * @param pixmap
     */
    void updatedPreviewFrame(const QPixmap& pixmap, const QPixmap& trueSizePixmap);

    /**
     * @brief updatedPreviewFrame - Updates the pixmap in the trueSize window to reflect the current frame
     * @param pixmap
     */
    // void updateTrueLabelSize(const QPixmap& pixmap);

private slots:

    //  * @brief on_pixelSizeSlider_sliderMoved - Handles change in brush/pixel size slider
    //  * @param position - The size that the user sets in the slider
    //
    // void on_pixelSizeSlider_sliderMoved(int position);

    /**
     * @brief on_actionAddFrame_triggered - Handles the creation of a copied frame from the current drawing area's frame when clicked
     */
    void on_actionAddFrame_triggered();

    /**
     * @brief on_actionDeleteFrame_triggered - Handles the deletion of the current frame when clicked
     */
    void on_actionDeleteFrame_triggered();

    /**
     * @brief on_frameTabBar_tabBarClicked - Handles changing the current frame in the drawing area
     * @param index - The index of the frame to change to
     */
    void on_frameTabBar_tabBarClicked(int index);


signals:
    /**
     * @brief updatePixelSize - Sends the signal to update brush/pixel size
     * @param size - What size (px) for the brush to change to
     */
    void updatePixelSize(int size);

    /**
     * @brief addFrame - Sends the signal to create the copy of a new frame
     * @param pixelSize
     * @param copyIndex
     */
    void addFrame(int pixelSize, int copyIndex);

    /**
     * @brief deleteFrame - Sends the signal to delete the current frame
     * @param index - The index of the current frame
     */
    void deleteFrame(int index);

    /**
     * @brief updateCurrentFrame - Sends the signal to update the current frame for the user's view when changing frames
     * @param index - The index of the frame to change to
     */
    void updateCurrentFrame(int index);

    /**
     * @brief changeTool - Handles changing the currently selected tool
     * @param tool - Which tool to change to
     */
    void changeTool(DrawingArea::PaintTool tool);





private:
    /**
     * @brief ui - The actual user interface of the window
     */
    Ui::MainWindow *ui;
    //std::vector<QImage> frames;

    /**
     * @brief loadProject
     * @param filePath
     * @param frames
     */
    void loadProject(const QString& filePath, QVector<QImage> frames);

    /**
     * @brief currentFrame - Tracks the current frame that the user is editing/viewing
     */
    int currentFrame = 0;

    /**
     * @brief totalNumFrames - Total count of frames for the sprite
     */
    int totalNumFrames = 1;

    /**
     * @brief drawingArea - The actual sprite/frame editing portion of the UI
     */
    DrawingArea *drawingArea;

    /**
     * @brief onColorSelected - Handles the selection of a color from the color palette
     * @param color - RGBA color
     */
    void onColorSelected(const QColor &color);

    /**
     * @brief onColorSelectorClicked - Handles the use of the color palette button to open the palette
     */
    void onColorSelectorClicked();

    /**
     * @brief dialog - The window that pops up when color palette tool is selected
     */
    QColorDialog *dialog;

    // void animationPreview();

    /**
     * @brief saveFrames -
     * @param frames
     * @param filepath
     */
    void saveFrames(std::vector<QImage>& frames, QString& filepath);

    /**
     * @brief loadFrames
     * @param frames
     * @param filepath
     */
    void loadFrames(std::vector<QImage>& frames, QString& filepath);
};
#endif // MAINWINDOW_H
