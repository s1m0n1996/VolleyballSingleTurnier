#ifndef WINDOWEDIT_H
#define WINDOWEDIT_H

#include <QLineEdit>

class WindowEdit: public QLineEdit
{
public:
    WindowEdit(QString placeholder, QWidget *parent = nullptr);
};

#endif // WINDOWEDIT_H