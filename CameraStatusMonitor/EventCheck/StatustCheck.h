#ifndef STATUSTCHECK_H
#define STATUSTCHECK_H

#include <QObject>
#include<QMutex>
#include"DataModel/CustomTableModel.h"
#include"EventCheck/BaseEventCheck.h"
#include"AlarmModule/AlarmModule.h"

struct CamStatusInfo
{
    QString qstrCamIP;      //设备IP

    //这个变量用来检测“断开”状态持续的时间
    qint64 iLastNormalTick;  //最后一次正常状态的时间

    //下面三个变量用于检测一段时间内状态变化次数是否异常
    qint64 iCheckTimeBegin;
    int iChangeTimes;
    int iLastStatus;        //正常状态为0，不正常为-1
};

class StatustCheck : public BaseEventCheck
{
    Q_OBJECT
public:
    explicit StatustCheck(QObject *parent = 0);
    ~StatustCheck();
signals:
    void signal_SendAlarm(AlarmMessage);
public slots:
    void slot_StartEvent();

private:
    bool m_bExitCheck;                     //事件检测结束标志位
    int m_iMaxChangeCounts;         //单位时间内最大状态变化次数  单位：次
    int m_iCheckTimeRange;          //检测变化次数的时间段    单位：分钟

    int m_iDiconStateDurationTime;  //断开状态最大持续时间  //单位：分钟

    QMutex m_mutex;

    CustomTableModel* m_pDataModel;
    QList<CamStatusInfo*> CamStatusList;
public:
    void StopCheck();
    void SetDataModel(CustomTableModel* DataModel);
    void SetDisConTimeRangeAndCount(int timeRange, int Count);
    void SetCheckDisconnectTimeOut(int iMinutes);

private:
    int findIndexFromCamInfoListByIp(QString IpAddress);

    void CheckChangeTimes(CamStatusInfo * Info,QString StatusText, int timeRange, int MaxChangeTimes);
    void CheckDisConnectTimeOut(CamStatusInfo *Info, QString StatusText, int timeOut);

    void clearCamStatusList();
};

#endif // STATUSTCHECK_H
