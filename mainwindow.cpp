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
#include <QTimer>


MainWindow::MainWindow(std::vector<Frame> frames, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->DrawingAreaLabel->setGeometry(120,50,400,400);
    ui->PreviewLabel->setGeometry(600,50,100,100);

    frames.push_back(Frame(400, 400));

    currentFrame = 0;

    this->frames = frames;

    QToolBar *toolBar = ui->toolBar;

    drawingArea = new DrawingArea(frames[currentFrame], parent);

    QAction *paintBucketAction = ui->actionPaintBucket;


    QAction *selectToolAction = ui->actionSelectTool;
    QAction *eraserAction = ui->actionEraser;
    QAction *colorPickerAction = ui->actionColorPicker;
    QAction *penAction = ui->actionPen;
    penAction->setChecked(true);

    QActionGroup *toolActionGroup = new QActionGroup(this);
    toolActionGroup->addAction(penAction);
    toolActionGroup->addAction(paintBucketAction);
    toolActionGroup->addAction(selectToolAction);
    toolActionGroup->addAction(eraserAction);

    toolBar->addAction(penAction);
    toolBar->addAction(paintBucketAction);
    toolBar->addAction(selectToolAction);
    toolBar->addAction(eraserAction);
    toolBar->addAction(colorPickerAction);

    this->dialog = new QColorDialog(this);

    dialog->setOption(QColorDialog::ShowAlphaChannel);



    connect(paintBucketAction, &QAction::triggered, this, &MainWindow::onPaintBucketClicked);
    connect(selectToolAction, &QAction::triggered, this, &MainWindow::onSelectToolClicked);
    connect(eraserAction, &QAction::triggered, this, &MainWindow::onEraserClicked);
    connect(penAction, &QAction::triggered, this, &MainWindow::onPenClicked);

    connect(ui->actionColorPicker, &QAction::triggered, this, &MainWindow::onColorSelectorClicked);

    connect(drawingArea, &DrawingArea::imageUpdated, this, [=](const QPixmap &pixmap) {
        ui->DrawingAreaLabel->setPixmap(pixmap);});

    drawingArea->setUpCanvas();
    drawingArea->setParent(ui->DrawingAreaLabel);
    drawingArea->setBrushColor(QColor(Qt::red));
    dialog->setCurrentColor(QColor(Qt::red));


   // DrawingArea* previewArea = new DrawingArea(frames[currentFrame], parent);
    // connect(previewArea, &DrawingArea::onUpdatedFps, ui->fpsSlider, &QSlider::setValue);

    // connect color dialog to brush color
    connect(this->dialog, &QColorDialog::currentColorChanged, drawingArea, &DrawingArea::setBrushColor);


    //connect slider to pixelSize
    connect(this, &MainWindow::updatePixelSize, drawingArea, &DrawingArea::setPixelSize);
    animationPreview();


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onColorSelectorClicked(){
    this->dialog->show();
    onPenClicked();
}


void MainWindow::onPaintBucketClicked() {

}

void MainWindow::onEraserClicked() {
    drawingArea->setBrushColor(QColor(Qt::white)); //FIXME signals and slots
    animationPreview();

}

void MainWindow::onSelectToolClicked() {

}

void MainWindow::onPenClicked() {

    drawingArea->setBrushColor(dialog->currentColor()); //FIXME signals and slots
}

void MainWindow::animationPreview(){

    for(size_t i = 0; i < frames.size(); i++){
        // Ensure delete/add frame buttons are disabled during animation.
        if(i == frames.size()){
            i = 0;
        }
        //edge case 0  fps
        int delay = 1000 / drawingArea->getFps();

        Frame currFrame = frames.at(i);

        QImage previewImage = currFrame.getImage();
        QPixmap previewPixmap = QPixmap::fromImage(previewImage);

        // Quickshot: Using a method pointer
        QTimer::singleShot(delay, this, [=]() {
            ui->PreviewLabel->setPixmap(previewPixmap);
        });
    }
}


void MainWindow::on_fpsSlider_sliderMoved(int position)
{

}


void MainWindow::on_pixelSizeSlider_sliderMoved(int position)
{
     emit updatePixelSize(position);
}

