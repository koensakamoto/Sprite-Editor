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
#include <QDebug>


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

    //connect save button
    connect(ui->saveButton, &QAction::triggered, this, &MainWindow::saveClicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onColorSelectorClicked(){
    this->dialog->show();
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

void MainWindow::saveFrames(std::vector<Frame>& frames, QString& filePath){
    //If there are not frames then return
    if (frames.empty()) {
        return;
    }

    //Guarantee the file has a .ssp extension
    if (!filePath.endsWith(".ssp", Qt::CaseInsensitive)) {
        filePath += ".ssp";
    }

    //Get the dimension of the frames.
    Frame& firstFrame = frames.front();
    int height = firstFrame.getHeight();
    int width = firstFrame.getWidth();


    QJsonArray frameArray;

    for (size_t frameIndex = 0; frameIndex < frames.size(); frameIndex++) {
        QJsonObject frameObject;
        frameObject["frame_Index"] = static_cast<int>(frameIndex);
        QJsonArray grid;
        QImage image = frames[frameIndex].getImage();

        for (int y = 0; y < height; y++) {
            QJsonArray row;
            for (int x = 0; x < width; x++) {
                QColor color = image.pixelColor(x, y);

                QJsonObject pixelObj {
                    //Pixel are represented by textual values
                    {"r", QString::number(color.red())},
                    {"g", QString::number(color.green())},
                    {"b", QString::number(color.blue())},
                    {"a", QString::number(color.alpha())}
                };
                row.append(pixelObj);
            }
            grid.append(row);
        }

        frameObject["pixels"] = grid;
        frameArray.append(frameObject);
    }

    //Create Json object for spirte editor that stores the height, width, and frames
    QJsonObject spriteEditor;
    spriteEditor["height"] = height;
    spriteEditor["width"] = width;
    spriteEditor["frames"] = frameArray;

    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)) {
        throw std::runtime_error("Failed to open file for writing: " + filePath.toStdString());
    }

    //Write json contents to the file
    QJsonDocument jsonDoc(spriteEditor);
    QByteArray jsonBytes = jsonDoc.toJson(QJsonDocument::Indented);
    file.write(jsonBytes);
    file.close();
}

void MainWindow::loadFrames(std::vector<Frame>& frames, QString& filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        throw std::runtime_error("Failed to open file for reading: " + filePath.toStdString());
    }

    QByteArray jsonData = file.readAll();
    file.close();

    QJsonParseError parseError;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData, &parseError);

    if (!jsonDoc.isObject()) {
        throw std::runtime_error("Invalid JSON format: " + parseError.errorString().toStdString());
    }

    QJsonObject jsonObject = jsonDoc.object();
    int height = jsonObject["height"].toInt();
    int width = jsonObject["width"].toInt();
    QJsonArray framesJson = jsonObject["frames"].toArray();

    //Ensure frames vector doesn't hold pre existing data
    frames.clear();

    for (const QJsonValue& frameValue : framesJson) {
        QJsonObject frameObject = frameValue.toObject();
        QJsonArray gridJson = frameObject["pixels"].toArray();

        QImage image(width, height, QImage::Format_ARGB32);

        //Iterate through the pixel grid and set Image pixel values
        for (int y = 0; y < height; y++) {
            QJsonArray rowJson = gridJson[y].toArray();
            for (int x = 0; x < width; x++) {
                QJsonObject pixelObj = rowJson[x].toObject();
                QRgb color = qRgba(
                    pixelObj["r"].toString().toInt(),
                    pixelObj["g"].toString().toInt(),
                    pixelObj["b"].toString().toInt(),
                    pixelObj["a"].toString().toInt()
                    );
                image.setPixel(x, y, color);
            }
        }
        frames.push_back(Frame(image));
    }
}

void MainWindow::saveClicked(){
    QString filePath = "loadJsonFile.txt";
    loadFrames(this->frames, filePath);
}


