#ifndef WINNERPOPUP_H
#define WINNERPOPUP_H

#include <QWidget>

class WindowLabel;
class WindowButton;

/*!
 * \file winnerpopup.h
 * \brief Die Klasse WinnerPopup erzuegt ein PopUp Fenster, welches den Gewinner anzeigt
 * \author Lea Kesselmeier
 *
 * Der Gewinner ene Spieles bzw Turniees wird angezeigt
*/
class WinnerPopup : public QWidget
{
    Q_OBJECT
public:
    WinnerPopup(QString winnerName, QWidget* parent = nullptr);
    ~WinnerPopup(void);
    void setWinnerLeg(void);
    void setWinnerTournament(void);

private slots:
    void closeRefereeWindow(void);


signals:
    void clickedOk(void);

private:
    void connecting(void);
    void createWidgets(QString winnerName);
    void setwholeLayout(void);

    WindowLabel* _crown     = nullptr;
    WindowLabel* _winner    = nullptr;
    WindowLabel* _title     = nullptr;
    WindowButton* _okButton = nullptr;



};

#endif // WINNERPOPUP_H
