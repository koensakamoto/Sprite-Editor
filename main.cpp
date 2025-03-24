#include "mainwindow.h"
#include <vector>
#include "frame.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    std::vector<Frame> frames;
    MainWindow w(frames);
    w.show();
    return a.exec();
}
