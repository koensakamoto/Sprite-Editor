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
#include <QInputDialog>
#include <QResizeEvent>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Set default sprite size to 25x25 (index 2)
    ui->spriteSizeComboBox->setCurrentIndex(2);

    // Set main window background and menu bar styling
    this->setStyleSheet("QMainWindow { background-color: #f5f5f5; } "
                       "QMenuBar { background-color: #f0f0f0; border-bottom: 1px solid #ccc; } "
                       "QMenuBar::item { padding: 4px 10px; } "
                       "QMenuBar::item:selected { background-color: #4a9eff; color: white; } "
                       "QMenu { background-color: white; border: 1px solid #ccc; } "
                       "QMenu::item:selected { background-color: #4a9eff; color: white; } "
                       "QStatusBar { background-color: #f0f0f0; border-top: 1px solid #ccc; }");

    // Apply modern styling to drawing area and preview labels
    // Geometry is set in the UI file
    ui->DrawingAreaLabel->setStyleSheet("QLabel { background-color: white; border: 2px solid #ccc; border-radius: 4px; }");
    ui->PreviewLabel->setStyleSheet("QLabel { background-color: white; border: 2px solid #ccc; border-radius: 4px; }");

    QToolBar *toolBar = ui->toolBar;
    toolBar->setStyleSheet("QToolBar { background-color: #e0e0e0; border: 1px solid #bbb; spacing: 5px; } "
                           "QToolButton { background-color: white; border: 1px solid #ccc; border-radius: 3px; padding: 5px; } "
                           "QToolButton:hover { background-color: #e8f4f8; border: 1px solid #4a9eff; } "
                           "QToolButton:checked { background-color: #4a9eff; border: 1px solid #3a7ed0; }");

    // Frame management setup
    // Hide the old toolbar (not used anymore)
    ui->frameToolBar->hide();

    // Create a simple widget for the first tab
    QWidget *firstTab = new QWidget();
    QLabel *firstLabel = new QLabel(QString("Frame %1").arg(1), firstTab);
    ui->frameTabBar->addTab(firstLabel, QString("Frame %1").arg(1));

    // Connect frame management buttons
    connect(ui->addFrameButton, &QPushButton::clicked, this, &MainWindow::on_addFrameButton_clicked);
    connect(ui->deleteFrameButton, &QPushButton::clicked, this, &MainWindow::on_deleteFrameButton_clicked);

    // Add keyboard shortcuts for frame management
    this->addAction(ui->actionAddFrame);
    this->addAction(ui->actionDeleteFrame);
    this->addAction(ui->actionNextFrame);
    this->addAction(ui->actionPrevFrame);

    // Update frame count display
    updateFrameCount();

    // Disable delete button initially (only 1 frame)
    ui->deleteFrameButton->setEnabled(false);

    // Style the FPS slider
    ui->fpsSlider->setStyleSheet("QSlider::groove:horizontal { border: 1px solid #bbb; height: 8px; background: #e0e0e0; border-radius: 4px; } "
                                 "QSlider::handle:horizontal { background: #4a9eff; border: 1px solid #3a7ed0; width: 18px; margin: -5px 0; border-radius: 9px; } "
                                 "QSlider::handle:horizontal:hover { background: #6ab4ff; }");

    // Instantiation of the model
    // Default sprite dimension (will be changeable from UI)
    int dim = 25;
    int canvasSize = 500;  // Match the DrawingAreaLabel size
    int pSize = canvasSize / dim;

    drawingArea = new DrawingArea(parent, canvasSize);  // Canvas matches label size
    drawingArea->setPixelSize(pSize);             // Grid size
    QAction *paintBucketAction = ui->actionPaintBucket;

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
    toolActionGroup->addAction(eraserAction);

    toolBar->addAction(penAction);
    toolBar->addAction(paintBucketAction);
    toolBar->addAction(eraserAction);
    toolBar->addAction(colorPickerAction);

    this->dialog = new QColorDialog(this);

    dialog->setOption(QColorDialog::ShowAlphaChannel);

    connect(paintBucketAction, &QAction::triggered, this, &MainWindow::onPaintBucketClicked);
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
    drawingArea->setGeometry(0, 0, ui->DrawingAreaLabel->width(), ui->DrawingAreaLabel->height());
    drawingArea->setBrushColor(QColor(Qt::red));
    dialog->setCurrentColor(QColor(Qt::red));

    // connect color dialog to brush color
    connect(this->dialog, &QColorDialog::currentColorChanged, drawingArea, &DrawingArea::setBrushColor);


    //connect slider to pixelSize
    // connect(this, &MainWindow::updatePixelSize, drawingArea, &DrawingArea::setPixelSize);
    // // animationPreview();

    //connect save button
    connect(ui->saveButton, &QAction::triggered, this, &MainWindow::saveClicked);
    connect(ui->loadButton, &QAction::triggered, this, &MainWindow::loadClicked);

    connect(drawingArea, &DrawingArea::previewUpdated, this, &MainWindow::updatedPreviewFrame);
    connect(ui->previewButton, &QPushButton::clicked, drawingArea, &DrawingArea::previewSelected);

    // connect(ui->previewButton, &QPushButton::clicked, drawingArea, &DrawingArea::previewTrueSizeSelected);

    connect(ui->fpsSlider, &QSlider::valueChanged, drawingArea, &DrawingArea::onUpdatedFps);

    // Update FPS label when slider changes
    connect(ui->fpsSlider, &QSlider::valueChanged, this, [this](int value) {
        ui->fpsLabel->setText(QString("Animation FPS: %1").arg(value));
    });

    //connect add + delete frames
    connect(this, &MainWindow::addFrame, drawingArea, &DrawingArea::addFrame);
    connect(this, &MainWindow::deleteFrame, drawingArea, &DrawingArea::deleteFrame);
    connect(this, &MainWindow::updateCurrentFrame, drawingArea, &DrawingArea::updateCurrentFrame);

    // // Connecting the drawing tools to drawing area
    connect(this,&MainWindow::changeTool, drawingArea, &DrawingArea::setCurrentTool);

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
    drawingArea->setBrushColor(QColor(Qt::white));

}

void MainWindow::onPenClicked() {
    emit changeTool(DrawingArea::PaintTool::PEN);
    drawingArea->setBrushColor(dialog->currentColor());
}


// void MainWindow::on_pixelSizeSlider_sliderMoved(int position)
// {
//      emit updatePixelSize(position);
//     // setting up the true to scale sprite preview

// }

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


void MainWindow::on_addFrameButton_clicked()
{
    totalNumFrames++;

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Add Frame",
                                  "Do you want to copy the current frame?",
                                  QMessageBox::Yes | QMessageBox::No,
                                  QMessageBox::Yes);

    if (reply == QMessageBox::Yes) {
        emit addFrame(40, currentFrame);
    } else {
        emit addFrame(40, -1);
    }

    // Create a simple widget for the new tab
    QWidget *newTab = new QWidget();
    QLabel *newLabel = new QLabel(QString("Frame %1").arg(totalNumFrames), newTab);

    // Add the new tab
    ui->frameTabBar->addTab(newLabel, QString("Frame %1").arg(totalNumFrames));

    // Switch to the new frame
    ui->frameTabBar->setCurrentIndex(totalNumFrames - 1);
    currentFrame = totalNumFrames - 1;

    // Update UI
    updateFrameCount();
    ui->deleteFrameButton->setEnabled(totalNumFrames > 1);
}

void MainWindow::on_actionAddFrame_triggered()
{
    on_addFrameButton_clicked();
}


void MainWindow::on_deleteFrameButton_clicked()
{
    // Don't allow deleting the last frame
    if (totalNumFrames <= 1) {
        QMessageBox::information(this, "Cannot Delete",
                                "You must have at least one frame in your project.",
                                QMessageBox::Ok);
        return;
    }

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Delete Frame",
                                  QString("Are you sure you want to delete Frame %1?").arg(currentFrame + 1),
                                  QMessageBox::Yes | QMessageBox::No,
                                  QMessageBox::Yes);

    if (reply == QMessageBox::Yes) {
        ui->frameTabBar->removeTab(currentFrame);
        emit deleteFrame(currentFrame);
        totalNumFrames--;

        // Switch to first frame
        currentFrame = 0;
        on_frameTabBar_tabBarClicked(0);

        // Update UI
        updateFrameCount();
        ui->deleteFrameButton->setEnabled(totalNumFrames > 1);
    }
}

void MainWindow::on_actionDeleteFrame_triggered()
{
    on_deleteFrameButton_clicked();
}


void MainWindow::on_frameTabBar_tabBarClicked(int index)
{
    currentFrame = index;
    emit updateCurrentFrame(index);
    updateFrameCount();
}

void MainWindow::updateFrameCount()
{
    ui->frameCountLabel->setText(QString("%1 / %2").arg(currentFrame + 1).arg(totalNumFrames));
}

void MainWindow::on_actionNextFrame_triggered()
{
    if (currentFrame < totalNumFrames - 1) {
        currentFrame++;
        ui->frameTabBar->setCurrentIndex(currentFrame);
        emit updateCurrentFrame(currentFrame);
        updateFrameCount();
    }
}

void MainWindow::on_actionPrevFrame_triggered()
{
    if (currentFrame > 0) {
        currentFrame--;
        ui->frameTabBar->setCurrentIndex(currentFrame);
        emit updateCurrentFrame(currentFrame);
        updateFrameCount();
    }
}

void MainWindow::on_newSpriteButton_clicked()
{
    // Get selected dimension from combo box
    QString sizeText = ui->spriteSizeComboBox->currentText();
    // Extract dimension from "10x10 (XL pixels)" format
    int dim = sizeText.split("x")[0].toInt();  // Gets "10" from "10x10 (XL pixels)"

    // Confirm with user
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "New Sprite",
                                  QString("Create a new %1x%1 sprite?\nThis will clear the current canvas and all frames.").arg(dim),
                                  QMessageBox::Yes | QMessageBox::No,
                                  QMessageBox::Yes);

    if (reply == QMessageBox::Yes) {
        // Calculate new pixel size
        int canvasSize = 500;
        int pSize = canvasSize / dim;

        // Reset the drawing area with new dimensions
        drawingArea->resetCanvas(canvasSize, pSize);

        // Reset frames
        currentFrame = 0;
        totalNumFrames = 1;

        // Clear and reset frame tabs
        ui->frameTabBar->clear();
        QWidget *firstTab = new QWidget();
        QLabel *firstLabel = new QLabel(QString("Frame %1").arg(1), firstTab);
        ui->frameTabBar->addTab(firstLabel, QString("Frame %1").arg(1));

        // Update frame count display and button states
        updateFrameCount();
        ui->deleteFrameButton->setEnabled(false);
    }
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);

    if (!drawingArea) return;

    // Base dimensions (default window size)
    const int baseWidth = 1200;
    const int baseHeight = 750;
    const int baseCanvasSize = 500;
    const int basePreviewSize = 250;

    // Current window dimensions
    int currentWidth = event->size().width();
    int currentHeight = event->size().height();

    // Calculate scale factors
    float scaleX = (float)currentWidth / baseWidth;
    float scaleY = (float)currentHeight / baseHeight;
    float scale = qMin(scaleX, scaleY);  // Use minimum to maintain aspect ratio

    // Only scale if window is larger than base size
    if (scale > 1.0f) {
        // Scale canvas
        int newCanvasSize = baseCanvasSize * scale;
        ui->DrawingAreaLabel->setGeometry(180 * scale, 60 * scale, newCanvasSize, newCanvasSize);

        // Update DrawingArea to match label
        drawingArea->setGeometry(0, 0, newCanvasSize, newCanvasSize);

        // Scale canvas title
        ui->canvasTitleLabel->setGeometry(180 * scale, 30 * scale, newCanvasSize, 25 * scale);

        // Scale preview
        int newPreviewSize = basePreviewSize * scale;
        ui->PreviewLabel->setGeometry(720 * scale, 80 * scale, newPreviewSize, newPreviewSize);
        ui->previewTitleLabel->setGeometry(720 * scale, 50 * scale, newPreviewSize, 25 * scale);

        // Scale animation controls
        ui->fpsLabel->setGeometry(745 * scale, 340 * scale, 200 * scale, 20 * scale);
        ui->fpsSlider->setGeometry(745 * scale, 365 * scale, 200 * scale, 25 * scale);
        ui->previewButton->setGeometry(785 * scale, 400 * scale, 120 * scale, 40 * scale);

        // Scale frames section
        ui->framesLabel->setGeometry(180 * scale, 570 * scale, 120 * scale, 25 * scale);
        ui->frameCountLabel->setGeometry(310 * scale, 570 * scale, 80 * scale, 25 * scale);
        ui->addFrameButton->setGeometry(570 * scale, 570 * scale, 50 * scale, 25 * scale);
        ui->deleteFrameButton->setGeometry(625 * scale, 570 * scale, 55 * scale, 25 * scale);
        ui->frameTabBar->setGeometry(180 * scale, 600 * scale, newCanvasSize, 80 * scale);
    }
}

