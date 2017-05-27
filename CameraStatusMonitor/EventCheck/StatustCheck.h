#ifndef STATUSTCHECK_H
#define STATUSTCHECK_H

#include <QObject>
#include<QMutex>
#include"DataModel/CustomTableModel.h"
#include"EventCheck/BaseEventCheck.h"
#include"AlarmModule/AlarmModule.h"

struct CamStatusInfo
{
    QString qstrCamIP;      //�豸IP

    //�������������⡰�Ͽ���״̬������ʱ��
    qint64 iLastNormalTick;  //���һ������״̬��ʱ��

    //���������������ڼ��һ��ʱ����״̬�仯�����Ƿ��쳣
    qint64 iCheckTimeBegin;
    int iChangeTimes;
    int iLastStatus;        //����״̬Ϊ0��������Ϊ-1
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
    bool m_bExitCheck;                     //�¼���������־λ
    int m_iMaxChangeCounts;         //��λʱ�������״̬�仯����  ��λ����
    int m_iCheckTimeRange;          //���仯������ʱ���    ��λ������

    int m_iDiconStateDurationTime;  //�Ͽ�״̬������ʱ��  //��λ������

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
