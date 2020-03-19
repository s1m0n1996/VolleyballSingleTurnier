#ifndef WINDOWLABEL_H
#define WINDOWLABEL_H

#include <QLabel>
/*!
 * \file windowlabel.h
 * \brief Die Klasse WindowLabel erzeugt Labels
 * \author Lea Kesselmeier
 *
 * Jedes Label bekommt das selbe StyleSheet beim Erzeugen
 * Je nach Anwendung werden andere StyleSheets benutzt
*/
class WindowLabel: public QLabel
{
    Q_OBJECT
public:
    explicit WindowLabel(QString text, QWidget *parent = nullptr);
    explicit WindowLabel(QWidget *parent = nullptr,QString text = "");
    void setNotStartTournamentStyle(void);
    void setStartTournamentStyle(void);
    void setTitleStyle(void);
    void setMainTitleStyle(void);
    void setBold(void);
    void setWinnerStyle(void);


public:
    WindowLabel();
};

#endif // WINDOWLABEL_H
