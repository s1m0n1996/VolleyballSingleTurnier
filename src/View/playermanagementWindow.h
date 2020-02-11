#ifndef PLAYERMANAGMENT_H
#define PLAYERMANAGMENT_H

#include <QLabel>
#include <QLineEdit>
#include <QMainWindow>
#include <QMessageBox>
#include <QPushButton>
#include <QList>


#include "ui_playermanagementWindow.h"
#include "windowbutton.h"
#include "windowedit.h"
#include "windowlabel.h"
#include "View/tournementnamepopup.h"
#include "Model/playermanagement.h"
#include "View/tabelview.h"

#include <cmath>
#include <QDebug>

QT_BEGIN_NAMESPACE
namespace Ui { class PlayermanagementWindow; }
QT_END_NAMESPACE

class PlayermanagementWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit PlayermanagementWindow(QWidget *parent = nullptr);
    ~PlayermanagementWindow();
    QList<QList<QString>> getList(void);


signals:
public slots:
    void addPlayer(void);
    void setMaxPlayerLabel(void);
    void tournementName(void);
    void showTabel(void);
    void highlightRow(void);
    void giveModel(void);



private slots:


private:
    void connecting();
    void createAddPlayerEdit();
    void createButtons(void);
    void createMaxPlayer();
    void setAllLayout(void);

    Ui::PlayermanagementWindow *ui;
    WindowButton* _addPlayerButton          = nullptr;
    WindowButton* _startTournementButton    = nullptr;
    WindowButton* _add                      = nullptr;
    WindowButton* _delete                   = nullptr;
    WindowLabel* _playernameLabel           = nullptr;
    WindowLabel* _birthdayLabel             = nullptr;
    WindowLabel* _countryLabel              = nullptr;
    WindowLabel* _nameMaxPlayerLabel        = nullptr;
    WindowLabel* _valueMaxPlayerLabel       = nullptr;
    WindowEdit* _playernameEdit             = nullptr;
    WindowEdit* _birthdayEdit               = nullptr;
    WindowEdit* _countryEdit                = nullptr;
    PlayerManagement* _model                = nullptr;
    PlayerManagement* _modelGame            = nullptr;
    TableView* _allPlayer                   = nullptr;
    TableView* _gamePlayer                  = nullptr;
    QList<QList<QString>> _listAll;
};
#endif // PLAYERMANAGMENT_H
