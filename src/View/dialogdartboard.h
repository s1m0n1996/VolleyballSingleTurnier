#ifndef DIALOGDARTBOARD_H
#define DIALOGDARTBOARD_H

#include <QDialog>

namespace Ui {
class DialogDartboard;
}

class DialogDartboard : public QDialog
{
    Q_OBJECT

public:
    explicit DialogDartboard(QWidget *parent = nullptr);
    ~DialogDartboard();

private slots:
    void mouseCurrentPos();
    void mouseReleasedOnDartboard();
    void on_naechsterSpieler_released();
    void on_undoLetzterWurf_released();

private:
    Ui::DialogDartboard *ui;
};

#endif // DIALOGDARTBOARD_H

