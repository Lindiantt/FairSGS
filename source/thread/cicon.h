#ifndef CICON_H
#define CICON_H

#include "pch.h"

class CIcon
{
public:
    CIcon(const QString &);
    ~CIcon();
    void load();
    QString path;
    QPixmap pixmapNormal,pixmapDown,pixmapDisabled,pixmapHover;
    QIcon icon;
    bool loaded;
};

#endif // CICON_H
