#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "frame.h"
#include <vector>
#include <QActionGroup>


MainWindow::MainWindow(std::vector<Frame> frames, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->frames = frames;

    QToolBar *toolBar = ui->toolBar;

    QAction *paintBucketAction = new QAction("Paint Bucket", this);
    QAction *selectToolAction = new QAction("Select Tool", this);
    QAction *eraserAction = new QAction("Eraser", this);

    paintBucketAction->setCheckable(true);
    selectToolAction->setCheckable(true);
    eraserAction->setCheckable(true);


    QActionGroup *toolActionGroup = new QActionGroup(this);
    toolActionGroup->addAction(paintBucketAction);
    toolActionGroup->addAction(selectToolAction);
    toolActionGroup->addAction(eraserAction);

    toolBar->addAction(paintBucketAction);
    toolBar->addAction(selectToolAction);
    toolBar->addAction(eraserAction);

    connect(paintBucketAction, &QAction::triggered, this, &MainWindow::onPaintBucketClicked);
    connect(selectToolAction, &QAction::triggered, this, &MainWindow::onSelectToolClicked);
    connect(eraserAction, &QAction::triggered, this, &MainWindow::onEraserClicked);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onPaintBucketClicked() {

}

void MainWindow::onEraserClicked() {


}

void MainWindow::onSelectToolClicked() {

}
