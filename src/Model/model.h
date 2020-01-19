/*!
* \file model/model.h
* \brief Model
* \author Nico Meyer
*/

#ifndef MODEL_H
#define MODEL_H

#include <QObject>

class Model : public QObject
{
    Q_OBJECT
public:
    explicit Model(QObject* parent = nullptr);
    int value(void) const;
    void setValue(const int value);

signals:
    void valueChanged(void);

public slots:

private:
    int _value = 0;
};

#endif // MODEL_H
