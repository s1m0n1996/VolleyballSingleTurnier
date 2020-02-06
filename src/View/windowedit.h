#ifndef WINDOWEDIT_H
#define WINDOWEDIT_H

#include <QLineEdit>

class windowEdit: public QLineEdit
{
public:
    windowEdit(QString text, QWidget *parent = nullptr);
};

#endif // WINDOWEDIT_H
