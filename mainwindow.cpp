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


MainWindow::MainWindow(std::vector<Frame> frames, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->frames = frames;

    QToolBar *toolBar = ui->toolBar;



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

    connect(paintBucketAction, &QAction::triggered, this, &MainWindow::onPaintBucketClicked);
    connect(selectToolAction, &QAction::triggered, this, &MainWindow::onSelectToolClicked);
    connect(eraserAction, &QAction::triggered, this, &MainWindow::onEraserClicked);


    this->dialog = new QColorDialog(this);

    QAction *colorPickerAction = ui->actionColorPicker;

    toolBar->addAction(colorPickerAction);

    dialog->setOption(QColorDialog::ShowAlphaChannel);

    connect(ui->actionColorPicker, &QAction::triggered, this, &MainWindow::onColorSelectorClicked);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onColorSelectorClicked(){
    this->dialog->show();
}

void MainWindow::onColorSelected(const QColor &color){
    int red = color.red();
    int green = color.green();
    int blue = color.blue();
    int alpha = color.alpha();
}

void MainWindow::onPaintBucketClicked() {

}

void MainWindow::onEraserClicked() {


}

void MainWindow::onSelectToolClicked() {

}

// void MainWindow::loadProject(const QString& filePath, QVector<Frame> frames){

//     QFile file(filePath);

//     if (!file.open(QIODevice::ReadOnly)) {
//         qDebug() << "failure to open file:" << filePath;
//         return;
//     }

//     QByteArray projectJson = file.readAll();
//     file.close();

//     QJsonParseError parseError;
//     QJsonDocument jsonDoc = QJsonDocument::fromJson(projectJson, &parseError);

//     if (!jsonDoc.isObject()) {
//         qDebug() << "The structure of the JSON is incorrect!";
//         return;
//     }
//     frames.clear();

//     QJsonObject jsonObject = jsonDoc.object();

//     int height = jsonObject["height"].toInt();
//     int width = jsonObject["width"].toInt();

//     QJsonArray framesJson = jsonObject["frames"].toArray();

//     for (const QJsonValue& frameValue : framesJson){
//         QJsonObject frameObj = frameValue.toObject();

//         Frame currFrame(width, height);

//         QJsonArray gridJson = frameObj["grid"].toArray();

//         for(int row = 0; row < height; row++){
//             QJsonArray rowJson = gridJson[row].toArray();
//             for(int col = 0; col < width; col++){
//                 QJsonObject pixelObj = rowJson[col].toObject();

//                 unsigned char r = pixelObj["r"].toInt();
//                 unsigned char g = pixelObj["g"].toInt();
//                 unsigned char b = pixelObj["b"].toInt();
//                 unsigned char a = pixelObj["a"].toInt();

//                 currFrame.setPixel(col, row, r, g, b, a);
//             }
//         }
//         frames.append(currFrame);
//     }
// }
