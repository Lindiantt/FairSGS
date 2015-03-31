#include "labelclickable.h"

LabelClickable::LabelClickable(QWidget *parent) : QLabel(parent)
{

}

LabelClickable::~LabelClickable()
{

}

void LabelClickable::mousePressEvent(QMouseEvent *)
{
    emit clicked();
}
