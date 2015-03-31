#ifndef WORKERLOADSOURCE_H
#define WORKERLOADSOURCE_H

#include "pch.h"
#include "network/define.h"
#define WORKTYPE_MOVIE 0
#define WORKTYPE_IMAGE 1
#define WORKTYPE_SOUND 2
#define WORKTYPE_ICON 3

class CImage;
class CIcon;
class CCheckPointer;
typedef CImage* pCImage;
typedef QMovie* pQMovie;
typedef CIcon* pCIcon;

class workerLoadSource : public QObject
{
    Q_OBJECT
public:
    explicit workerLoadSource(QObject *parent = 0);
    ~workerLoadSource();
    bool ready;
    pQMovie movieLoading;
    pCImage generalCards[MAX_GENERAL],generalAvatars[MAX_GENERAL],shenCards[MAX_SHEN],shenAvatars[MAX_SHEN],bgRoom,
    bgGamePlayer,bgBottomLeft,bgBottomRight,bloodBack[5],blood[6],kingdomFrame[5],kingdomIcon[5],roleCorner[5],iconWuQi[3],
    iconFangJu,iconMa,suitsSmall[4],pointRed[13],pointBlack[13],cards[44],bgBottomEquip,handCardNumber,cardBack,
    frameMeSelected,framePlayerSelected,framePlayerPlaying,framePlayerResponding,framePlayerSOS;
    pCIcon buttonWuXie;
public slots:
    void handleNew(int workType,void* pointer,CCheckPointer* lab);
    void handleNew(int workType,void* pointer,void* lab);
    void doWork(int workType,void* pointer,void* lab,bool iscp=false);
    void handleStopMovie(QMovie*);
    void init();
signals:
    void newWork(int workType,void* pointer,CCheckPointer* lab);
    void newWork(int workType,void* pointer,void* lab);
    void stopMovie(QMovie*);
    void wakeUp();
private:
    void imageSet(CImage*,QWidget*);
};

#endif // WORKERLOADSOURCE_H
