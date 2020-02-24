#ifndef WINDOWLABEL_H
#define WINDOWLABEL_H

#include <QLabel>
/*!
 * \file windowlabel.h
 * \brief Die Klasse WindowLabel erzeugt Labels
 * \author Lea Kesselmeier
 *
 * Jedes Label bekommt das selbe StyleSheet beim Erzeugen
 * JE nach Anwendung werden andere StyleSheets benutzt
*/
class WindowLabel: public QLabel
{
    Q_OBJECT
public:
    explicit WindowLabel(QString text = "", QWidget *parent = nullptr);
    void setNotStartTournamentStyle(void);
    void setStartTournamentStyle(void);
    void setTitleStyel(void);


public:
    WindowLabel();
};

#endif // WINDOWLABEL_H
