#include <QtWidgets>

#include "View/dartboardViewer.h"

DartboardViewer::DartboardViewer(Referee* referee, QGraphicsItem* parent) :
        _referee(referee)
{
    Q_UNUSED(parent);
}

QRectF DartboardViewer::boundingRect() const
{
    return QRectF();
}

/*!
 * \brief Zeichnet die Dartscheibe für den Zuschauer und färbt diese ein sobald etwas geworfen wurde.
 *
 * \return void
 *
 * Diese Methode zeichnet die Dartscheibe. Es werden die einzelnen Felder generiert, indem in Objekt
 * davon gezeichnet wird und dieses dann immer um 18 Grad gedreht wird bis es 20 Felder davon gibt.
 * Die unterschiedlichen Segmenete werden jewails übereinander gelget. Sobald etwas geworfen wurde
 * ändert sich die Farbe in dem jeweiligen Feld. Wenn es der erste Wurf war, dann ist die Farbe "Magenta"
 * beim zweiten Wurf "Blau" und beim dritten Wurf "Gelb".
 *
 */
void DartboardViewer::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    QPainterPath singleFieldOneAll;
    QPainterPath singleFieldOneRightLine;

    QPainterPath singleFieldTwoAll;
    QPainterPath singleFieldTwoRightLine;

    QPainterPath doubleFieldAll;
    QPainterPath doubleFieldRightLine;

    QPainterPath tripleFieldAll;
    QPainterPath tripleFieldRightLine;

    QPainterPath missFieldAll;
    QPainterPath missFieldRightLine;

    QList<QPainterPath> texts;

    QList<QList<QBrush>> colourListOfAllFields;

    QList<QList<int>> valuesOfDartboard;
    QList<QList<QBrush>> changingColour;

    QList<QString> allFields = {"20", " 1", "18", " 4", "13", " 6", "10", "15", " 2", "17", " 3", "19", " 7", "16",
                                " 8", "11", "14", " 9", "12", " 5"};


    int valueOfMultiplikator = _referee->getValueMultiplikator();
    _allThrows = _referee->getAllThrowsWithoutMultiplikator();
    int remainingThrows = _referee->getRemainingThrows();


    // Hier wird die QList für den Multiplikator gesetzt, damit man immer weiß welcher Multiplikator
    // zu welchem Wurf gehört.
    if (remainingThrows == 2)
    {
        _allMulti[0] = valueOfMultiplikator;
    }
    else if (remainingThrows == 1)
    {
        _allMulti[1] = valueOfMultiplikator;
    }
    else if (remainingThrows == 0)
    {
        _allMulti[2] = valueOfMultiplikator;
    }
    else
    {
        for (int i = 0; i < _allMulti.size(); i++)
        {
            _allMulti[i] = -1;
        }
    }

    // Die Farbliste für die geworfenen Würfe wird angelegt
    for (int i = 0; i < 4; i++)
    {
        changingColour.append(QList<QBrush>());
    }

    changingColour[0].append(Qt::yellow);
    changingColour[1].append(Qt::blue);
    changingColour[2].append(Qt::magenta);
    changingColour[3].append(Qt::black);

    for (int i = 0; i < 4; i++)
    {
        colourListOfAllFields.append(QList<QBrush>());
    }

    for (int i = 0; i < colourListOfAllFields.size(); i++)
    {
        // Standart Farben für die Miss Felder erstellen
        if (i == 0)
        {
            for (int k = 0; k < 10; k++)
            {
                colourListOfAllFields[i].append(Qt::black);
                colourListOfAllFields[i].append(Qt::black);
            }
        }
            // Standart Farben für die Single Felder erstellen
        else if (i == 1)
        {
            for (int k = 0; k < 10; k++)
            {
                colourListOfAllFields[i].append(Qt::lightGray);
                colourListOfAllFields[i].append(Qt::black);
            }

        }
            // Standart Farben für die Triple und Double Felder erstellen
        else if (i == 2 or i == 3)
        {
            for (int k = 0; k < 10; k++)
            {
                colourListOfAllFields[i].append(Qt::red);
                colourListOfAllFields[i].append(Qt::darkGreen);
            }

        }
    }

    //Bulls_Eye
    colourListOfAllFields[1].append(Qt::darkGreen);
    colourListOfAllFields[2].append(Qt::red);

    for (int i = 0; i < 4; i++)
    {
        valuesOfDartboard.append(QList<int>());
    }

    for (int i = 0; i < valuesOfDartboard.size(); i++)
    {
        if (i == 0)
        {
            valuesOfDartboard[i] = {0};
        }
        else if (i == 1 or i == 2)
        {
            valuesOfDartboard[i] = {20, 1, 18, 4, 13, 6, 10, 15, 2, 17, 3, 19, 7, 16, 8, 11, 14, 9, 12, 5, 25};
        }
        else
        {
            valuesOfDartboard[i] = {20, 1, 18, 4, 13, 6, 10, 15, 2, 17, 3, 19, 7, 16, 8, 11, 14, 9, 12, 5};
        }
    }

    for (int i = 0; i < 3; i++)
    {
        if (_allMulti[i] > 0)
        {
            for (int t = 0; t < valuesOfDartboard[_allMulti[i]].size(); t++)
            {
                if (valuesOfDartboard[_allMulti[i]][t] == _allThrows[i])
                {
                    colourListOfAllFields[_allMulti[i]][t] = changingColour[remainingThrows][0];
                }
            }
        }
        else if (_allMulti[i] == 0)
        {
            for (int k = 0; k < colourListOfAllFields[0].size(); k++)
                colourListOfAllFields[0][k] = changingColour[remainingThrows][0];
        }
    }

    //Die einzelnen Segmente der Dartscheibe werden erstellt.
    singleFieldOneRightLine.moveTo(0, 0);
    singleFieldOneRightLine.lineTo(37, -230);
    singleFieldOneRightLine.closeSubpath();

    singleFieldOneAll.moveTo(0, 0);
    singleFieldOneAll.lineTo(-37, -230);
    singleFieldOneAll.arcMoveTo(-37, -235, 74, 10, 0);
    singleFieldOneAll.arcTo(-37, -235, 74, 10, 0, 180);
    singleFieldOneAll.connectPath(singleFieldOneRightLine);
    singleFieldOneAll.closeSubpath();

    singleFieldOneAll.setFillRule(Qt::WindingFill);


    tripleFieldRightLine.moveTo(0, 0);
    tripleFieldRightLine.lineTo(40, -255);
    tripleFieldRightLine.closeSubpath();

    tripleFieldAll.moveTo(0, 0);
    tripleFieldAll.lineTo(-40, -255);
    tripleFieldAll.arcMoveTo(-40, -260, 80, 10, 0);
    tripleFieldAll.arcTo(-40, -260, 80, 10, 0, 180);
    tripleFieldAll.connectPath(tripleFieldRightLine);
    tripleFieldAll.closeSubpath();

    tripleFieldAll.setFillRule(Qt::WindingFill);


    singleFieldTwoRightLine.moveTo(0, 0);
    singleFieldTwoRightLine.lineTo(58, -370);
    singleFieldTwoRightLine.closeSubpath();

    singleFieldTwoAll.moveTo(0, 0);
    singleFieldTwoAll.lineTo(-58, -370);
    singleFieldTwoAll.arcMoveTo(-58, -375, 116, 10, 0);
    singleFieldTwoAll.arcTo(-58, -375, 116, 10, 0, 180);
    singleFieldTwoAll.connectPath(singleFieldTwoRightLine);
    singleFieldTwoAll.closeSubpath();

    singleFieldTwoAll.setFillRule(Qt::WindingFill);

    doubleFieldRightLine.moveTo(0, 0);
    doubleFieldRightLine.lineTo(62, -395);
    doubleFieldRightLine.closeSubpath();

    doubleFieldAll.moveTo(0, 0);
    doubleFieldAll.lineTo(-62, -395);
    doubleFieldAll.arcMoveTo(-62, -400, 124, 10, 0);
    doubleFieldAll.arcTo(-62, -400, 126, 10, 0, 180);
    doubleFieldAll.connectPath(doubleFieldRightLine);
    doubleFieldAll.closeSubpath();

    doubleFieldAll.setFillRule(Qt::WindingFill);

    missFieldRightLine.moveTo(0, 0);
    missFieldRightLine.lineTo(82, -510);
    missFieldRightLine.closeSubpath();

    missFieldAll.moveTo(0, 0);
    missFieldAll.lineTo(-82, -510);
    missFieldAll.arcMoveTo(-80, -515, 164, 10, 0);
    missFieldAll.arcTo(-82, -515, 164, 10, 0, 180);
    missFieldAll.connectPath(missFieldRightLine);
    missFieldAll.closeSubpath();

    missFieldAll.setFillRule(Qt::WindingFill);

    // Zeichnet alle Felder bis auf das Bull- Eye
    for (int i = 0; i < 20; i++)
    {
        painter->fillPath(missFieldAll, colourListOfAllFields[0][i]);
        painter->drawPath(missFieldAll);
        painter->rotate(18);
    }

    for (int i = 0; i < 20; i++)
    {
        painter->fillPath(doubleFieldAll, colourListOfAllFields[2][i]);
        painter->drawPath(doubleFieldAll);
        painter->rotate(18);
    }

    for (int i = 0; i < 20; i++)
    {
        painter->fillPath(singleFieldTwoAll, colourListOfAllFields[1][i]);
        painter->drawPath(singleFieldTwoAll);
        painter->rotate(18);

    }

    for (int i = 0; i < 20; i++)
    {
        painter->fillPath(tripleFieldAll, colourListOfAllFields[3][i]);
        painter->drawPath(tripleFieldAll);
        painter->rotate(18);
    }

    for (int i = 0; i < 20; i++)
    {
        painter->fillPath(singleFieldOneAll, colourListOfAllFields[1][i]);
        painter->drawPath(singleFieldOneAll);
        painter->rotate(18);
    }

    for (int i = 0; i < 20; i++)
    {
        texts.append(QPainterPath());
    }

    for (int i = 0; i < texts.size(); i++)
    {
        texts[i].addText(-20, -450, QFont("Times", 16, QFont::Bold), allFields[i]);
    }

    for (int i = 0; i < 20; i++)
    {
        painter->setBrush(Qt::white);
        painter->drawPath(texts[i]);
        painter->rotate(18);
    }


    // Zeichnen des Bulls-Eyes
    painter->setBrush(colourListOfAllFields[1][20]);
    painter->drawEllipse(-25, -25, 50, 50);

    painter->setBrush(colourListOfAllFields[2][20]);
    painter->drawEllipse(-12, -12, 25, 25);

}
