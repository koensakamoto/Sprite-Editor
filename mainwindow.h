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
    MainWindow(std::vector<Frame> frames, QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void onPaintBucketClicked();
    void onEraserClicked();
    void onSelectToolClicked();
    void onPenClicked();

private:
    Ui::MainWindow *ui;
    std::vector<Frame> frames;
    void loadProject(const QString& filePath, QVector<Frame> frames);

    int currentFrame;
    DrawingArea *drawingArea;
    void onColorSelected(const QColor &color);
    void onColorSelectorClicked();
    QColorDialog *dialog;

    void animationPreview(DrawingArea previewArea);
};
#endif // MAINWINDOW_H
