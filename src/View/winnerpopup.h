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
    ~WinnerPopup();
    void setWinnerLeg(void);
    void setWinnerTournament(void);

private slots:
    void closeRefereeWindow();


signals:
    void clickedOk();

private:
    void connecting();
    void createWidgets(QString winnerName);
    void setwholeLayout();

    WindowLabel* _crown     = nullptr;
    WindowLabel* _winner    = nullptr;
    WindowLabel* _title     = nullptr;
    WindowButton* _okButton = nullptr;



};

#endif // WINNERPOPUP_H
