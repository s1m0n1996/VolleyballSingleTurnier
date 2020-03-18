#ifndef WINNERPOPUP_H
#define WINNERPOPUP_H
#include <QWidget>
class WindowLabel;
class WindowButton;


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
