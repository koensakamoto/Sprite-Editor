#include "JSONHandler.h"
#include "pixel.h"
#include <QFile>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonParseError>

void JSONHandler::loadProject(const QString& filePath){
    QFile file(filePath);

    //Check right here if file loaded correctly

    QByteArray projectJson = file.readAll();
    file.close();

    QJsonParseError parseError;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(projectJson, &parseError);
    if(jsonDoc.isObject()){
        QJsonObject jsonObject = jsonDoc.object();
        QJsonArray frames = jsonObject["frames"].toArray();

        for(int i = 0; i < frames.size(); i++){
            QJsonObject frameObj = frames.at(i).toObject();
            QJsonArray frameRows = jsonObject["grid"].toArray();

            for(int r = 0; r < frameRows.size(); r++){
                QJsonObject rowPixelsObj = frames.at(i).toObject();
                QJsonArray pixelsInRow = jsonObject[""].toArray();
                for(int c = 0; c < pixelsInRow.size(); c++){
                    Pixel p = pixelsInRow.at(c).toObject();
                }
            }


        }
    }
    else{
        return;
    }

