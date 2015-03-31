#include "uiUtils.h"
#include <qimage.h>

QImage QSanUiUtils::produceShadow(const QImage &image, QColor shadowColor, int radius, double decade) {
    int cols = image.width();
    int rows = image.height();
    int alpha = shadowColor.alpha();
    QImage newImg(cols,rows,QImage::Format_ARGB32);
    QColor newColor=shadowColor;
    newColor.setAlpha(0);
    newImg.fill(newColor);
    for (int y = 0; y < rows; y++) {
        for (int x = 0; x < cols; x++) {
            if (qAlpha(image.pixel(x,y)) == 0) continue;
            for (int dy = -radius; dy <= radius; dy++) {
                for (int dx = -radius; dx <= radius; dx++) {
                    int wx = x + dx;
                    int wy = y + dy;
                    int dist = dx * dx + dy * dy;
                    if (wx < 0 || wy < 0 || wx >= cols || wy >= rows) continue;
                    if (dx * dx + dy * dy > radius * radius) continue;
                    int newVal = alpha - decade * dist;
                    if(qAlpha(newImg.pixel(wx,wy))<newVal)
                    {
                        newColor.setAlpha(newVal);
                        newImg.setPixel(wx,wy,newColor.rgba());
                    }
                }
            }
        }
    }
    return newImg;
}

void QSanUiUtils::makeGray(QPixmap &pixmap) {
    QImage img = pixmap.toImage();
    for (int i = 0; i < img.width(); i++) {
        for (int j = 0; j < img.height(); j++) {
            QColor color = QColor::fromRgba(img.pixel(i, j));
            int gray = qGray(color.rgb());
            img.setPixel(i, j, qRgba(gray, gray, gray, color.alpha()));
        }
    }
    pixmap = QPixmap::fromImage(img);
}
