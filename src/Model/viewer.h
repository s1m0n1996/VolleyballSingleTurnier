#ifndef VIEWER_H
#define VIEWER_H

#include <QObject>
#include <QJsonObject>
#include <QFile>

#include "Model/referee.h"

class Viewer : public QObject
{
    Q_OBJECT
public:
    Viewer(void);
    void createJsonDocument(int throwCounter, int remainScore);
    QList<QString> getHowToFinishLeg(void);
    QJsonDocument _loadDoc;

signals:
    void howToFinishLeg(void);

private:
    Referee* _referee;
    QList<QString> _listWithHowToFinishLeg;


};

#endif // VIEWER_H
