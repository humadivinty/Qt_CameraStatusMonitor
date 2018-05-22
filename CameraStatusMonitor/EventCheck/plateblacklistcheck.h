#ifndef PLATEBLACKLISTCHECK_H
#define PLATEBLACKLISTCHECK_H

#include"BaseEventCheck.h"
#include"Camera/cameraresult.h"
#include"AlarmModule/AlarmModule.h"
#include <QObject>
#include<QMutex>
#include<QList>


class PlateBlackListCheck : public BaseEventCheck
{
    Q_OBJECT
public:
    explicit PlateBlackListCheck(QObject *parent = 0);
    ~PlateBlackListCheck();

signals:
    void signal_SendAlarm(AlarmMessage);
public slots:
    void slot_StartEvent();

public:
    void StopCheck();
    bool GetPlateBlackList(const QString filePath, QStringList& plateList);
    bool ComparePlateWithBlackList(const QStringList& plateList,const QString plateNo);

    bool GetPlateWhiteList(const QString filePath,QStringList& plateList);
    bool ComparePlateWithWhiteList(const QStringList& plateList, const QString plateNo);

    bool AddOneToPlateList(CameraResult* result);
    CameraResult *GetOneResultFromList();
    CameraResult* GetOneResultFromResultList();

    bool GetExitStatus();
    void SetPlateList(void *list);

private:
    void ClearResultList();
    int GetResultListSize();
private:
    bool m_bExitCheck;                     //事件检测结束标志位
    QList<CameraResult*> m_lsplateResultList;
    void* g_pList;

    QMutex m_mutex;

};

#endif // PLATEBLACKLISTCHECK_H
