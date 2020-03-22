#include "Model/viewer.h"

Viewer::Viewer()
{
    QFile loadFile;

    loadFile.setFileName(":/database/FinishCombination.json");
    loadFile.open(QFile::ReadOnly);
    QByteArray saveData = loadFile.readAll();

    _loadDoc = QJsonDocument::fromJson(saveData);
}

/*!
 * \brief Erstellt eine QList mit den möglichen Finishwegen.
 *
 * \return void
 *
 * In dieser Methode wird aus dem JSON-Dokument welches die möglichen Finishwege beinhaltet, eine Liste
 * erstellet, wo nun alle möglichen Finishwege enthalten sind.
 *
 */
void Viewer::createJsonDocument(int throwCounter, int remainScore)
{
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
