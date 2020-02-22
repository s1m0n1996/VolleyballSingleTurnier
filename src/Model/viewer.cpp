#include "viewer.h"

Viewer::Viewer()
{
    QFile loadFile;

    loadFile.setFileName(":/database/FinishCombination.json");
    loadFile.open(QFile::ReadOnly);
    QByteArray saveData = loadFile.readAll();

    _loadDoc = QJsonDocument::fromJson(saveData);
}

void Viewer::createJsonDocument(int throwCounter, int remainScore)
{
    qDebug() << "hrowCOunter" << throwCounter;
    qDebug() << "remain Score" << remainScore;
    QJsonArray listWithHowToFinishLeg = _loadDoc[QString::number(throwCounter)][QString::number(remainScore)].toArray();

    _listWithHowToFinishLeg.clear();
    for (QJsonValue value : listWithHowToFinishLeg)
    {
        _listWithHowToFinishLeg.append(value.toString());
    }
    emit howToFinishLeg();
}

QList<QString> Viewer::getHowToFinishLeg()
{
    return _listWithHowToFinishLeg;
}











