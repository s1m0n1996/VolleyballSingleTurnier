#include "viewerWindow.h"
#include "ui_viewerWindow.h"
#include "Model/statistics.h"
#include "View/dartboardViewer.h"
#include <QGraphicsScene>

ViewerWindow::ViewerWindow(Referee *referee, Viewer* viewer, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ViewerWindow),
    _referee(referee),
    _viewer(viewer)
{
    Viewer* viewers = new Viewer;
    _viewer = viewers;    

    setWindowIcon(QIcon(":/img/viewer.png"));
    ui->setupUi(this);
    ui->centralwidget->setStyleSheet("background: white;");

    modifiWidgets();
    connecting();
}


ViewerWindow::~ViewerWindow()
{
    delete ui;
}


void ViewerWindow::connecting(void)
{
    connect(_referee,SIGNAL(valueChanged()),this, SLOT(writeScore()));
    connect(_referee,SIGNAL(scoreIsUnder170()),this, SLOT(scoreIsUnder170InLeg()));
    connect(_referee,SIGNAL(remainingThrowsAreZero()),this, SLOT(remainingThrowsAreZeroInLeg()));
    connect(_viewer,SIGNAL(howToFinishLeg()),this,SLOT(possibleWayToFinishLeg()));

}


void ViewerWindow::modifiWidgets()
{
    ui->title->setMainTitleStyle();
    ui->nameOfPlayer1->setBold();
    ui->nameOfPlayer2->setBold();
    ui->aktivePlayer->setBold();
    ui->remainScore->setBold();
    ui->remainScorePlayer1Label->setBold();
    ui->remainScorePlayer1->setBold();
    ui->remainScorePlayer2Label->setBold();
    ui->remainScorePlayer2->setBold();
    ui->countOfWinningLegsPlayer1->setBold();
    ui->countOfWinningLegsPlayer1Label->setBold();
    ui->countOfWinningLegsPlayer2->setBold();
    ui->countOfWinningLegsPlayer2Label->setBold();

    ui->groupBox_3->setStyleSheet("border: none;");
    ui->photoPlayer2->setStyleSheet("border: none;");
    ui->photoPlayer1->setStyleSheet("border: none;");
    ui->graphicsView->setStyleSheet("border: none");

    _dartboard->setPos(0, 0);
    scene->addItem(_dartboard);

    ui->graphicsView->setScene(scene);
    ui->graphicsView->scale(0.8,0.8);
}


/*!
 * \brief Schreibt alle Werte in die Ui rein.
 *
 * \return void
 *
 * Diese Methode holt vom Model alle relevanten Werte und schreibt diese dann in die Ui Datei.
 * Es wird das Foto des jewailigen Spielers aus der Datenbank geladen und angezeigt.
 *
 */
void ViewerWindow::writeScore()
{   
    scene->update(0,0,20,10);

    _player1 = _referee->getAllPlayersForViewer()[0];
    _player2 = _referee->getAllPlayersForViewer()[1];

    Player playerA(_player1);
    Player playerB(_player2);

    Statistics statisticsPlayerA;
    Statistics statisticsPlayerB;

    ui->nameOfPlayer1->setText(playerA.getName());
    ui->nameOfPlayer2->setText(playerB.getName());

    setPhoto(playerA, playerB);

    _aktivePlayer = _referee->getAktivePlayer();
    Player aktivePlayer(_aktivePlayer);

    ui->aktivePlayer->setText(aktivePlayer.getName());


    if (_aktivePlayer == _player1)
    {
        setAktionPlayer1();
    }
    else
    {
        setAktionPlayer2();
    }

    ui->remainScorePlayer1->setText(QString::number(_referee->getRemainScoreForViewer()[0]));
    ui->remainScorePlayer2->setText(QString::number(_referee->getRemainScoreForViewer()[1]));
    ui->averageOfPlayer1->setText(QString::number(statisticsPlayerA.getAverageOfPlayerInCurrentGame(playerA)));
    ui->averageOfPlayer2->setText(QString::number(statisticsPlayerB.getAverageOfPlayerInCurrentGame(playerB)));
}


void ViewerWindow::setAktionPlayer1(void)
{
    ui->throw1FromPlayer1->setText(QString::number(_referee->getThrows()[0]));
    ui->throw2FromPlayer1->setText(QString::number(_referee->getThrows()[1]));
    ui->throw3FromPlayer1->setText(QString::number(_referee->getThrows()[2]));
    ui->all3ThrowsPlayer1->setText(QString::number(_referee->getThrowScore()));
    ui->remainingThrowsForPlayer1->setText(QString::number(_referee->getRemainingThrows()));
    ui->countOfWinningLegsPlayer1->setText(QString::number(_referee->getCountOfWinningLegs()));
    ui->remainScore->setText("Restpunktzahl: " + (QString::number(_referee->getRemainScoreForViewer()[0])));
    ui->player1GroupBox->setStyleSheet("QGroupBox{"
                                       "border-color: green;"
                                       "border-width: 3px;"
                                       "border-style: solid;}");
    ui->player2GroupBox->setStyleSheet("QGroupBox{"
                                       "border-color: black;}");
}


void ViewerWindow::setAktionPlayer2(void)
{
    ui->throw1FromPlayer2->setText(QString::number(_referee->getThrows()[0]));
    ui->throw2FromPlayer2->setText(QString::number(_referee->getThrows()[1]));
    ui->throw3FromPlayer2->setText(QString::number(_referee->getThrows()[2]));
    ui->all3ThrowsPlayer2->setText(QString::number(_referee->getThrowScore()));
    ui->remainingThrowsForPlayer2->setText(QString::number(_referee->getRemainingThrows()));
    ui->countOfWinningLegsPlayer2->setText(QString::number(_referee->getCountOfWinningLegs()));
    ui->remainScore->setText("Restpunktzahl: " + QString::number(_referee->getRemainScoreForViewer()[1]));
    ui->player2GroupBox->setStyleSheet("QGroupBox{"
                                       "border-color: green;"
                                       "border-width: 3px;"
                                       "border-style: solid;}");
    ui->player1GroupBox->setStyleSheet("QGroupBox{"
                                       "border-color: black;}");
}


void ViewerWindow::setPhoto(Player playerA, Player playerB)
{
    QByteArray dataA = playerA.loadPicture();
    QByteArray dataB = playerB.loadPicture();

    QPixmap pixmapA;
    QPixmap pixmapB;

    pixmapA.loadFromData(dataA);
    pixmapB.loadFromData(dataB);

    int wA = ui->photoPlayer1->width();
    int hA = ui->photoPlayer1->height();

    int wB = ui->photoPlayer1->width();
    int hB = ui->photoPlayer1->height();

//    QTransform rotate1;
//    rotate1.rotate(90);

//    QTransform rotate2;
//    rotate2.rotate(270);

    pixmapA = pixmapA.scaled(wA,hA,Qt::KeepAspectRatio);
//    pixmapA = pixmapA.transformed(rotate1);

    pixmapB = pixmapB.scaled(wB,hB,Qt::KeepAspectRatio);
//    pixmapB = pixmapB.transformed(rotate2);

    ui->photoPlayer1->setPixmap(pixmapA);
    ui->photoPlayer2->setPixmap(pixmapB);
}


void ViewerWindow::scoreIsUnder170InLeg()
{
    _viewer->createJsonDocument(_referee->getRemainingThrows(), _referee->getRemainScore());
}


/*!
 * \brief Schreibt den möglichen Weg ein Leg zu gewinnen in die Ui.
 *
 * \return void
 *
 * Diese Methode schreibt sobald sie das Signal vom Model bekommt den möglichen Weg ein Leg zu gewinnen
 * in die Ui rein.
 *
 */
void ViewerWindow::possibleWayToFinishLeg()
{
    qDebug() << "emit";
    _aktivePlayer = _referee->getAktivePlayer();
    qDebug() << _aktivePlayer;

    if (_aktivePlayer == _player1)
    {

        ui->possibleWayToFinishThrow1FromPlayer1->setText(_viewer->getHowToFinishLeg()[0]);
        ui->possibleWayToFinishThrow2FromPlayer1->setText(_viewer->getHowToFinishLeg()[1]);
        ui->possibleWayToFinishThrow3FromPlayer1->setText(_viewer->getHowToFinishLeg()[2]);
    }
    else
    {

        ui->possibleWayToFinishThrow1FromPlayer2->setText(_viewer->getHowToFinishLeg()[0]);
        ui->possibleWayToFinishThrow2FromPlayer2->setText(_viewer->getHowToFinishLeg()[1]);
        ui->possibleWayToFinishThrow3FromPlayer2->setText(_viewer->getHowToFinishLeg()[2]);

    }

}


/*!
 * \brief Setzt den möglichen Weg das Leg zu gewinnen auf einen leeren String zurück
 *
 * \return void
 *
 * Diese Methode setzt die möglichen Weg das Leg zu gewinnen zurück, wenn vom Model das Signal kommt,
 * das kein Wurf mehr verfügbar ist.
 *
 */
void ViewerWindow::remainingThrowsAreZeroInLeg()
{
    if (_aktivePlayer == _player1)
    {
        ui->possibleWayToFinishThrow1FromPlayer1->setText("");
        ui->possibleWayToFinishThrow2FromPlayer1->setText("");
        ui->possibleWayToFinishThrow3FromPlayer1->setText("");
    }
    else
    {
        ui->possibleWayToFinishThrow1FromPlayer2->setText("");
        ui->possibleWayToFinishThrow2FromPlayer2->setText("");
        ui->possibleWayToFinishThrow3FromPlayer2->setText("");
    }
}