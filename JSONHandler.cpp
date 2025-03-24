#endif // JSONHANDLER_H

#include "JSONHandler.h"
#include "pixel.h"
#include <QFile>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonParseError>

void JSONHandler::loadProject(const QString& filePath, QVector<Frame> frames){
    QFile file(filePath);

    //Check right here if file loaded correctly

    QByteArray projectJson = file.readAll();
    file.close();

    QJsonParseError parseError;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(projectJson, &parseError);
    if(jsonDoc.isObject()){
        QJsonObject jsonObject = jsonDoc.object();
        QJsonArray framesJson = jsonObject["frames"].toArray();

        //Need to get actual height and width from reading json
        int height = 10;
        int width = 10;

        for(int i = 0; i < frames.size(); i++){
            QJsonObject frameObj = framesJson.at(i).toObject();
            QJsonArray frameRows = jsonObject["grid"].toArray();
            Frame currFrame(width, height);
            QVector<QVector<Pixel>> grid;
            for(int r = 0; r < frameRows.size(); r++){
                QJsonObject rowPixelsObj = framesJson.at(i).toObject();
                QJsonArray pixelsInRow = jsonObject["pixel"].toArray();
                QVector<Pixel> currRow;
                for(int c = 0; c < pixelsInRow.size(); c++){
                    QJsonObject pixelObj = pixelsInRow.at(c).toObject();
                    //currRow.append(currPixel);
                }
                grid.append(currRow);
            }
            frames.append(currFrame);
        }
    }
    else{
        return;
    }
}


