/*!
* \file model/model.cpp
* \brief Model
* \author Nico Meyer
*/

#include "model.h"

Model::Model(QObject* parent) : QObject(parent)
{
}

int Model::value(void) const
{
    return _value;
}

void Model::setValue(const int value)
{
    _value = value * value;
   // emit valueChanged();
}
