#include "cicon.h"

CIcon::CIcon(const QString &path)
{
    this->path=path;
    loaded=false;
}

CIcon::~CIcon()
{

}

void CIcon::load()
{
    if(loaded) return;
    pixmapNormal.load(path+"normal.png");
    pixmapDisabled.load(path+"disabled.png");
    pixmapDown.load(path+"down.png");
    pixmapHover.load(path+"hover.png");
    icon=QIcon();
    icon.addPixmap(pixmapNormal);
    icon.addPixmap(pixmapDisabled,QIcon::Disabled);
    icon.addPixmap(pixmapDown,QIcon::Normal,QIcon::On);
    icon.addPixmap(pixmapHover,QIcon::Active);
    loaded=true;
}
