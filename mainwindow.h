#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QColorDialog>
#include <vector>
#include "frame.h"
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

    // WILL NEED TO DELETE FRAME VECTOR
    MainWindow(std::vector<Frame> frames, QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void onPaintBucketClicked();
    void onEraserClicked();
    void onSelectToolClicked();
    void onPenClicked();

    void saveClicked();
    void loadClicked();

private slots:
    void on_fpsSlider_sliderMoved(int position);
    void on_pixelSizeSlider_sliderMoved(int position);

signals :
    void updatePixelSize(int size);
private:
    Ui::MainWindow *ui;
    std::vector<QImage> frames;
    void loadProject(const QString& filePath, QVector<QImage> frames);

    int currentFrame;
    DrawingArea *drawingArea;
    void onColorSelected(const QColor &color);
    void onColorSelectorClicked();
    QColorDialog *dialog;

    void animationPreview();

    void saveFrames(std::vector<QImage>& frames, QString& filepath);

    void loadFrames(std::vector<QImage>& frames, QString& filepath);
};
#endif // MAINWINDOW_H
