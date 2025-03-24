#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <vector>
#include "frame.h"

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

private:
    Ui::MainWindow *ui;
    std::vector<Frame> frames;
};
#endif // MAINWINDOW_H
