#ifndef JSONHANDLER_H
#define JSONHANDLER_H
#include <QString>
#include <frame.h>
#include <QVector>

class JSONHandler{
    //TODO: pass grid by referecne
private:
    void loadProject(const QString& filePath, QVector<Frame> frames);
};


