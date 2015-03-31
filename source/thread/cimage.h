#ifndef CIMAGE_H
#define CIMAGE_H

#include "pch.h"

class CImage
{
public:
    CImage(const QString &);
    ~CImage();
    static CImage* avatarById(quint16);
    QString filename;
    QPixmap pixmap;
    bool loaded;
};

#endif // CIMAGE_H
