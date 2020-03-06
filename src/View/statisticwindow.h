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
class QGroupBox;

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
    void createChart() const;

signals:
public slots:
    void showWinnerChart(void);
    void showSingleChart(void);
    void showDoubleChart(void);
    void showTripleChart(void);
    void showAverageChart(void);


private:
    QLabel*         _colorLabel = nullptr;
    WindowLabel*    _title      = nullptr;

    Playerstatistics* _playerStatistic;
    PlayerManagement* _playerManagement;
    QComboBox* _comboBox = nullptr;

    QChart* _chart = nullptr;

    QComboBox* _chooseCategoryComboBox = nullptr;
    QComboBox* _choosePlayerComboBox = nullptr;
    QComboBox* _chooseTournamentComboBox = nullptr;
    QComboBox* _chooseGameComboBox = nullptr;

    const QString _separator = "    ";


    void _createWidgets(void);

    void _setLayout(void);

    void _connect(void);


    void _dataChangesDetected(void);
    Player _getSelectedPlayer(void);
    void _refreshDiagram(const QMap<QString, int> diagramData);
    QGroupBox* _createSelectCategoryGroupBox(void);
    QGroupBox* _createSelectPlayerGroupBox(void);
    QGroupBox* _createFilterGroupBox(void);

};

#endif // STATISTICWINDOW_H
