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
class PlayerStatistics;
class PlayerManagement;
class Player;
class QGroupBox;

#include <QtWidgets/QWidget>
#include <QtCharts/QChartGlobal>


QT_CHARTS_BEGIN_NAMESPACE
class QChartView;
class QChart;
QT_CHARTS_END_NAMESPACE

namespace statistic
{
    enum type
    {
        Winner, Singles, Doubles, Triples, Average
    };
}

QT_CHARTS_USE_NAMESPACE
class StatisticWindow :  public QMainWindow
{
    Q_OBJECT
public:
    explicit StatisticWindow(QWidget *parent = nullptr);

signals:
public slots:
    void showWinnerChart(void);
    void showSingleChart(void);
    void showDoubleChart(void);
    void showTripleChart(void);
    void showAverageChart(void);
    void _dataChangesDetected(void);


private:
    QLabel*         _colorLabel = nullptr;
    WindowLabel*    _title      = nullptr;

    PlayerStatistics* _playerStatistic;
    PlayerManagement* _playerManagement;
    GameManagement* _gameManagement;

    QChart* _chart = nullptr;

    QComboBox* _chooseCategoryComboBox = nullptr;
    QComboBox* _choosePlayerComboBox = nullptr;
    QComboBox* _chooseTournamentComboBox = nullptr;
    // QComboBox* _chooseGameComboBox = nullptr;

    Player* _selectedPlayer = nullptr;

    void _createWidgets(void);
    void _setLayout(void);
    void _connect(void);

    void _refreshSelectedPlayer(void);
    void _refreshDiagram(const QMap<QString, double> diagramData);
    QGroupBox* _createSelectCategoryGroupBox(void);
    QGroupBox* _createSelectPlayerGroupBox(void);
    QGroupBox* _createFilterGroupBox(void);

    statistic::type _getSelectedChartType(void);
    int _getSelectedTournamentId(void);
};

#endif // STATISTICWINDOW_H
