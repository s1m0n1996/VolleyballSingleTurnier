#ifndef STATISTICWINDOW_H
#define STATISTICWINDOW_H

#include <QObject>
#include <QWidget>

class QLabel;
class WindowButton;
class WindowEdit;
class WindowLabel;


class StatisticWindow : public QWidget
{
    Q_OBJECT
public:
    StatisticWindow(QWidget *parent = nullptr);

private:
    QLabel*         _colorLabel = nullptr;
    WindowLabel*    _title      = nullptr;

    WindowLabel*    _name1      = nullptr;
    WindowLabel*    _wert1      = nullptr;

    WindowLabel*    _name2      = nullptr;
    WindowLabel*    _wert2      = nullptr;

    WindowLabel*    _name3      = nullptr;
    WindowLabel*    _wert3      = nullptr;

    WindowLabel*    _name4      = nullptr;
    WindowLabel*    _wert4      = nullptr;
};

#endif // STATISTICWINDOW_H
