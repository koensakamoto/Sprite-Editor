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

public slots:
    void onPaintBucketClicked();
    void onEraserClicked();
    void onSelectToolClicked();
    void onPenClicked();

    void saveClicked();
    void loadClicked();

    void updatedPreviewFrame(const QPixmap& pixmap);


private slots:
    void on_pixelSizeSlider_sliderMoved(int position);


    void on_actionAddFrame_triggered();

    void on_actionDeleteFrame_triggered();

    void on_frameTabBar_tabBarClicked(int index);


signals :
    void updatePixelSize(int size);
    void addFrame(int pixelSize, int copyIndex);
    void deleteFrame(int index);
    void updateCurrentFrame(int index);

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
