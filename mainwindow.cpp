#include "mainwindow.h"
#include "ui_mainwindow.h"
// #include "frame.h"
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
#include <QFileDialog>
#include <QDir>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->DrawingAreaLabel->setGeometry(120,50,400,400);
    ui->PreviewLabel->setGeometry(600,50,100,100);
    ui->DrawingAreaLabel->setStyleSheet("QLabel { background-color: white; }");
    ui->PreviewLabel->setStyleSheet("QLabel { background-color: white; }");

    QToolBar *toolBar = ui->toolBar;
    // frame toolbar
    QToolBar *frameToolBar = ui->frameToolBar;
    frameToolBar->setMovable(false);

    QAction *newFrameAction = ui->actionAddFrame;
    QAction *deleteFrameAction = ui->actionDeleteFrame;

    frameToolBar->addAction(newFrameAction);
    frameToolBar->addAction(deleteFrameAction);

    //frame tab bar
    QTabWidget *frameTabBar = ui->frameTabBar;
    ui->frameToolBar->addWidget(frameTabBar);
    // Create a simple widget for the first tab
    QWidget *firstTab = new QWidget();
    QLabel *firstLabel = new QLabel(QString("Frame %1").arg(1), firstTab);
    ui->frameTabBar->addTab(firstLabel, QString("Frame %1").arg(1));

    // Instantiation of the model
    drawingArea = new DrawingArea(parent, 400);

    QAction *paintBucketAction = ui->actionPaintBucket;

    QAction *selectToolAction = ui->actionSelectTool;
    QAction *eraserAction = ui->actionEraser;
    QAction *colorPickerAction = ui->actionColorPicker;
    QAction *penAction = ui->actionPen;

    QAction *actionHorizontalMirror = ui->actionHorizontalMirror;
    QAction *actionVerticalMirror = ui->actionVerticalMirror;

    connect(actionHorizontalMirror, &QAction::triggered, drawingArea, &DrawingArea::mirrorHorizontally);
    connect(actionVerticalMirror, &QAction::triggered, drawingArea, &DrawingArea::mirrorVertically);

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

    connect(drawingArea, &DrawingArea::imageUpdated, this, [=](const QPixmap &pixmap) {
    ui->PreviewLabel->setPixmap(
        pixmap.scaled(
            ui->PreviewLabel->size(),
            Qt::KeepAspectRatio,
            Qt::SmoothTransformation
            )
        );});

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
    // animationPreview();

    //connect save button
    connect(ui->saveButton, &QAction::triggered, this, &MainWindow::saveClicked);
    connect(ui->loadButton, &QAction::triggered, this, &MainWindow::loadClicked);

    connect(drawingArea, &DrawingArea::previewUpdated, this, &MainWindow::updatedPreviewFrame);
    connect(ui->previewButton, &QPushButton::clicked, drawingArea, &DrawingArea::previewSelected);

    connect(ui->previewButton, &QPushButton::clicked, drawingArea, &DrawingArea::previewFramesTrueSize);


    connect(ui->fpsSlider, &QSlider::valueChanged, drawingArea, &DrawingArea::onUpdatedFps);

    //connect add + delete frames
    connect(this, &MainWindow::addFrame, drawingArea, &DrawingArea::addFrame);
    connect(this, &MainWindow::deleteFrame, drawingArea, &DrawingArea::deleteFrame);
    connect(this, &MainWindow::updateCurrentFrame, drawingArea, &DrawingArea::updateCurrentFrame);

    // // Connecting the drawing tools to drawing area
    connect(this,&MainWindow::changeTool, drawingArea, &DrawingArea::setCurrentTool);

    int trueSize = drawingArea->size/drawingArea->pixelSize;
    ui->trueSizeLabel->setGeometry(650,320,trueSize,trueSize);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onColorSelectorClicked(){
    this->dialog->show();
    this->ui->actionPen->setChecked(true);
    onPenClicked();
}


void MainWindow::onPaintBucketClicked() {
    emit changeTool(DrawingArea::PaintTool::PAINTBUCKET);

}

void MainWindow::onEraserClicked() {
    emit changeTool(DrawingArea::PaintTool::ERASER);
    drawingArea->setBrushColor(QColor(Qt::white)); //FIXME signals and slots
    // emit eraserClicked
    // animationPreview();

}

void MainWindow::onSelectToolClicked() {
    drawingArea->setBrushColor(dialog->currentColor());
    emit changeTool(DrawingArea::PaintTool::SELECT);
}

void MainWindow::onPenClicked() {
    emit changeTool(DrawingArea::PaintTool::PEN);
    drawingArea->setBrushColor(dialog->currentColor()); //FIXME signals and slots
}




void MainWindow::on_pixelSizeSlider_sliderMoved(int position)
{
     emit updatePixelSize(position);
}

void MainWindow::saveFrames(std::vector<QImage>& frames, QString& filePath){
    //If there are not frames then return
    if (frames.empty()) {
        return;
    }

    //Guarantee the file has a .ssp extension
    if (!filePath.endsWith(".ssp", Qt::CaseInsensitive)) {
        filePath += ".ssp";
    }

    //Get the dimension of the frames.
    QImage firstFrame = frames.at(0);
    int height = firstFrame.height();
    int width = firstFrame.width();

    QJsonArray frameArray;

    for (int frameIndex = 0; frameIndex < (int)frames.size(); frameIndex++) {
        QJsonObject frameObject;
        frameObject["frame_Index"] = frameIndex;
        QJsonArray grid;
        QImage& image = frames[frameIndex];
        for (int y = 0; y < height; y++) {
            QJsonArray row;
            for (int x = 0; x < width; x++) {
                QColor color = image.pixelColor(x, y);
                QJsonObject pixelObj {
                    //Pixel are represented by textual values
                    {"r", color.red()},
                    {"g", color.green()},
                    {"b", color.blue()},
                    {"a", color.alpha()}
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

void MainWindow::loadFrames(std::vector<QImage>& frames, QString& filePath) {
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
                    pixelObj["r"].toInt(),
                    pixelObj["g"].toInt(),
                    pixelObj["b"].toInt(),
                    pixelObj["a"].toInt()
                    );
                image.setPixel(x, y, color);
            }
        }
        frames.push_back(image);
    }
}

void MainWindow::saveClicked(){
    QString filePath = QFileDialog::getSaveFileName(this, QDir::homePath());
    saveFrames(drawingArea->getFrames(), filePath);
}

void MainWindow::loadClicked(){
     QString filePath = QFileDialog::getOpenFileName(this, QDir::homePath());
    loadFrames(drawingArea->getFrames(), filePath);

    std::vector<QImage> framesVector = drawingArea->getFrames();

    drawingArea->setFrameVector(framesVector);

    drawingArea->setUpCanvas();
}

void MainWindow::updatedPreviewFrame(const QPixmap& pixmap){
    ui->PreviewLabel->setPixmap(
        pixmap.scaled(
            ui->PreviewLabel->size(),
            Qt::KeepAspectRatio,
            Qt::SmoothTransformation
            )
        );
}

void MainWindow::on_actionAddFrame_triggered()
{
    totalNumFrames++;
    // ask user for size + copy

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Add Frame", "Do you want to copy current frame?", QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::Yes) {
            emit addFrame(40, currentFrame);
        } else {
            emit addFrame(40, -1);
        }

        // Create a simple widget for the first tab
        QWidget *firstTab = new QWidget();
        QLabel *firstLabel = new QLabel(QString("Frame %1").arg(totalNumFrames), firstTab);


        // Add the first tab with text
        ui->frameTabBar->addTab(firstLabel, QString("Frame %1").arg(totalNumFrames));

}


void MainWindow::on_actionDeleteFrame_triggered()
{
    // Ask the user for confirmation
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Delete Frame", "Are you sure you want to delete this frame?", QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        ui->frameTabBar->removeTab(currentFrame);
        emit deleteFrame(currentFrame);
        // reset frame selected to first
        on_frameTabBar_tabBarClicked(0);
    }
}


void MainWindow::on_frameTabBar_tabBarClicked(int index)
{
    currentFrame = index;
    emit updateCurrentFrame(index);
}

