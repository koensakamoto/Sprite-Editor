#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "frame.h"
#include <vector>
#include <QActionGroup>
#include <QFile>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonParseError>
#include <QColor>
#include <QColorDialog>
#include "drawingarea.h"


MainWindow::MainWindow(std::vector<Frame> frames, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    frames.push_back(Frame(250, 250));

    currentFrame = 0;

    this->frames = frames;

    QToolBar *toolBar = ui->toolBar;

    drawingArea = new DrawingArea(frames[currentFrame], parent);

    QAction *paintBucketAction = ui->actionPaintBucket;


    QAction *selectToolAction = ui->actionSelectTool;
    QAction *eraserAction = ui->actionEraser;


    QActionGroup *toolActionGroup = new QActionGroup(this);
    toolActionGroup->addAction(paintBucketAction);
    toolActionGroup->addAction(selectToolAction);
    toolActionGroup->addAction(eraserAction);

    toolBar->addAction(paintBucketAction);
    toolBar->addAction(selectToolAction);
    toolBar->addAction(eraserAction);

    this->dialog = new QColorDialog(this);

    QAction *colorPickerAction = ui->actionColorPicker;

    toolBar->addAction(colorPickerAction);

    dialog->setOption(QColorDialog::ShowAlphaChannel);

    QPixmap pixmap2 = QPixmap(200,200);
    pixmap2.fill(Qt::black);



    connect(paintBucketAction, &QAction::triggered, this, &MainWindow::onPaintBucketClicked);
    connect(selectToolAction, &QAction::triggered, this, &MainWindow::onSelectToolClicked);
    connect(eraserAction, &QAction::triggered, this, &MainWindow::onEraserClicked);
    connect(ui->actionColorPicker, &QAction::triggered, this, &MainWindow::onColorSelectorClicked);

    connect(drawingArea, &DrawingArea::imageUpdated, this, [=](const QPixmap &pixmap) {
        ui->DrawingAreaLabel->setPixmap(pixmap);
        ui->DrawingAreaLabel->setScaledContents(true);
    });
    drawingArea->updateCanvas();
    drawingArea->setBrushColor(QColor(Qt::black));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onColorSelectorClicked(){
    this->dialog->show();
}

void MainWindow::onColorSelected(const QColor &color){
    if (color.isValid()) {
        drawingArea->setBrushColor(color);
    }
}

void MainWindow::onPaintBucketClicked() {

}

void MainWindow::onEraserClicked() {


}

void MainWindow::onSelectToolClicked() {

}
