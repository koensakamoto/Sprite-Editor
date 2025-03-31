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
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    /**
     * @brief mousePressEvent
     * @param event
     */
    void mousePressEvent(QMouseEvent* event) override;

    /**
     * @brief mouseMoveEvent
     * @param event
     */
    void mouseMoveEvent(QMouseEvent* event) override;

    /**
     * @brief mouseReleaseEvent
     * @param event
     */
    void mouseReleaseEvent(QMouseEvent* event) override;

public slots:
    /**
     * @brief onPaintBucketClicked
     */
    void onPaintBucketClicked();
    /**
     * @brief onEraserClicked
     */
    void onEraserClicked();
    /**
     * @brief onSelectToolClicked
     */
    void onSelectToolClicked();
    /**
     * @brief onPenClicked
     */
    void onPenClicked();
    /**
     * @brief saveClicked
     */
    void saveClicked();
    /**
     * @brief loadClicked
     */
    void loadClicked();
    /**
     * @brief updatedPreviewFrame
     * @param pixmap
     */
    void updatedPreviewFrame(const QPixmap& pixmap);


private slots:
    void on_pixelSizeSlider_sliderMoved(int position);

    void on_actionAddFrame_triggered();

    void on_actionDeleteFrame_triggered();

    void on_frameTabBar_tabBarClicked(int index);


signals:
    void updatePixelSize(int size);
    void addFrame(int pixelSize, int copyIndex);
    void deleteFrame(int index);
    void updateCurrentFrame(int index);
    void changeTool(DrawingArea::PaintTool tool);
    void mousePressPosition(QPoint point);
    void mouseMovePosition(QPoint point);
    void mouseReleasePosition(QPoint point);



private:
    Ui::MainWindow *ui;
    //std::vector<QImage> frames;
    void loadProject(const QString& filePath, QVector<QImage> frames);

    int currentFrame =0;
    int totalNumFrames =1;
    DrawingArea *drawingArea;
    void onColorSelected(const QColor &color);
    void onColorSelectorClicked();
    QColorDialog *dialog;

    void animationPreview();

    void saveFrames(std::vector<QImage>& frames, QString& filepath);

    void loadFrames(std::vector<QImage>& frames, QString& filepath);
};
#endif // MAINWINDOW_H
