#ifndef TOURNEMENTNAMEPOPUP_H
#define TOURNEMENTNAMEPOPUP_H

#include <QWidget>
#include <QMainWindow>
#include <QGridLayout>
#include <View/tournementwindow.h>
#include "View/windowbutton.h"
#include "View/windowedit.h"
#include "View/windowlabel.h"
#include <QDebug>
//#include "View/windowpopup.h"

class TournementNamePopUp : public QWidget
{
    Q_OBJECT
public:
    explicit TournementNamePopUp(QWidget *parent = nullptr);


public slots:
    void openTournementWindow(void);


private:
    WindowLabel* _dateLabel = nullptr;
    WindowEdit* _dateEdit = nullptr;

    WindowLabel* _nameLabel = nullptr;
    WindowEdit* _nameEdit = nullptr;

    WindowButton* _createTournement = nullptr;
};

#endif // TOURNEMENTNAMEPOPUP_H
