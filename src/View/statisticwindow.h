#ifndef STATISTICWINDOW_H
#define STATISTICWINDOW_H

#include <QObject>
#include <QMainWindow>

class QLabel;
class WindowButton;
class WindowEdit;
class WindowLabel;
class GameManagement;
class QComboBox;
class QRadioButton;
class Playerstatistics;
class PlayerManagement;
class Player;

#include <QtWidgets/QWidget>
#include <QtCharts/QChartGlobal>


QT_CHARTS_BEGIN_NAMESPACE
class QChartView;
class QChart;
QT_CHARTS_END_NAMESPACE


QT_CHARTS_USE_NAMESPACE
class StatisticWindow :  public QMainWindow
{
    Q_OBJECT
public:
    StatisticWindow(QWidget *parent = nullptr);
    QChart *createChart() const;

signals:
public slots:
    void showWinnerChart(void);
    void showSingleChart(void);
    void showDoubleChart(void);
    void showTripleChart(void);
    void showAverageChart(void);
    void getPlayerList(const QString &text);


private:
    QLabel*         _colorLabel = nullptr;
    WindowLabel*    _title      = nullptr;

    Playerstatistics* _playerstatistic;
    PlayerManagement* _playermanagment;
    WindowLabel* _choosePlayername = nullptr;
    QComboBox* _comboBox = nullptr;

    QChart* _chart = nullptr;


    WindowLabel* _categorie = nullptr;
    QRadioButton* _winners = nullptr;
    QRadioButton* _singlePoint = nullptr;
    QRadioButton* _doublePoint = nullptr;
    QRadioButton* _triplePoint = nullptr;
    QRadioButton* _average     = nullptr;

    QList<int> _listWinner;
    QList<double> _listeAverage;

    void createWidgets(void);

    void setLayout(void);

    void connect(void);

    void loadAllPlayersFromDatabase(void);

};

#endif // STATISTICWINDOW_H
