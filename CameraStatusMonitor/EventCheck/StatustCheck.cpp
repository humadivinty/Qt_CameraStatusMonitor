#include "StatustCheck.h"
#include<QMutexLocker>
#include<QDebug>
#include"LogModel/glogmodel.h"
#include<QDateTime>

#define MS_OF_ONE_MIMINUTE 60*1000
#include"data_commomdef.h"

StatustCheck::StatustCheck(QObject *parent)
    : BaseEventCheck(parent),
      m_iMaxChangeCounts(5),
      m_iCheckTimeRange(10),
      m_iDiconStateDurationTime(1)
{
    m_bExitCheck = false;
    m_pDataModel = NULL;
}

StatustCheck::~StatustCheck()
{
    SetDataModel(NULL);
    StopCheck();
    clearCamStatusList();
}

void StatustCheck::slot_StartEvent()
{
    GLogModel::GetInstant()->WriteLog("StatustCheck","StartEvent begin.");

    bool bRet = false;
    CustomTableModel* pDataModel = NULL;
    int iTimeRange = 0;      //“某段时间内状态变化次数” 中 的时间段, 在这里单位是分钟
    int iMaxChangeTimes =0;     //“某段时间内状态变化次数” 中 的“变化次数”

    int iTimeOut = 0;           //“断开状态”持续时间  ,在这里单位是分钟


    while(!bRet)
    {
        m_mutex.lock();
        bRet = m_bExitCheck;
        pDataModel = m_pDataModel;
        iTimeRange = m_iCheckTimeRange;
        iTimeOut = m_iDiconStateDurationTime ;
        iMaxChangeTimes = m_iMaxChangeCounts;
        m_mutex.unlock();

        if(bRet)
            break;

        if(pDataModel)
        {
            int iRowCount = pDataModel->GetRowCount();
            if (iRowCount <= 0)
                continue;
            for(int i = 0; i < iRowCount; i ++)
            {
                //获取相机IP
                //QString qstrIp = pDataModel->FindColTextFromRow(i, 1);
                QString qstrIp = pDataModel->FindColTextFromRow(i, IPADDRESS_COLUMN_NUMBER);

                //检查该IP是否在本地的状态队列中,如果不在，则将该IP加入到本地状态队列
                int iFindIndex = -1;
                iFindIndex = findIndexFromCamInfoListByIp(qstrIp);

                //获取相机状态
                //QString qstrStatus = pDataModel->FindColTextFromRow(i, 2);
                QString qstrStatus = pDataModel->FindColTextFromRow(i, STATUS_COLUMN_NUMBER);
                if(iFindIndex != -1)
                {
                    //获取本地状态队列中对应IP的状态
                    CamStatusInfo* tempInfo = CamStatusList.at(iFindIndex);

                    //检查该IP对应相机 一段时间内状态变化次数是否超过指定数值
                    CheckChangeTimes(tempInfo, qstrStatus, iTimeRange, iMaxChangeTimes);

                    //检查该IP对应相机 “断开”状态持续时间是否超过指定时间范围
                    CheckDisConnectTimeOut(tempInfo, qstrStatus, iTimeOut);

                    tempInfo = NULL;
                }
            }
        }
        QThread::currentThread()->sleep(10);
    }

    GLogModel::GetInstant()->WriteLog("StatustCheck","StartEvent finish.");
}

void StatustCheck::StopCheck()
{
    QMutexLocker locker(&m_mutex);
    m_bExitCheck = true;
}

void StatustCheck::SetDataModel(CustomTableModel *DataModel)
{
    QMutexLocker locker(&m_mutex);
    m_pDataModel = DataModel;
}

void StatustCheck::SetDisConTimeRangeAndCount(int timeRange, int Count)
{
    QMutexLocker locker(&m_mutex);

    QString qstrLog;
    qstrLog = QString("SetDisConCount ,input range = %1 minutes, Count = %2").arg(timeRange).arg(Count);
    GLogModel::GetInstant()->WriteLog("StatustCheck",qstrLog);

    if(Count > 0)
    {
        m_iMaxChangeCounts = Count;
    }

    qstrLog = QString("SetDisConCount ,final Count = %1").arg(m_iMaxChangeCounts);
    GLogModel::GetInstant()->WriteLog("StatustCheck",qstrLog);
}

void StatustCheck::SetCheckDisconnectTimeOut(int iMinutes)
{
    QMutexLocker locker(&m_mutex);

    QString qstrLog;
    qstrLog = QString("SetCheckDisconnectTimeOut ,input timeout = %1 minutes").arg(iMinutes);
    GLogModel::GetInstant()->WriteLog("StatustCheck",qstrLog);

    if(iMinutes > 0)
    {
        m_iDiconStateDurationTime = iMinutes;
    }

    qstrLog = QString("SetCheckDisconnectTimeOut ,final timeout = %1").arg(m_iDiconStateDurationTime);
    GLogModel::GetInstant()->WriteLog("StatustCheck",qstrLog);
}

int StatustCheck::findIndexFromCamInfoListByIp(QString IpAddress)
{
    int iFindIndex = -1;

    if(IpAddress.isEmpty())
        return iFindIndex;

    QMutexLocker locker(&m_mutex);

    for(int i = 0; i< CamStatusList.count() ;i++)
    {
        if(IpAddress == CamStatusList[i]->qstrCamIP)
        {
            iFindIndex = i;
        }
    }
    if(iFindIndex == -1)
    {
        CamStatusInfo* tempInfo = new CamStatusInfo;
        tempInfo->qstrCamIP = IpAddress;
        tempInfo->iChangeTimes = 0;
        tempInfo->iCheckTimeBegin =0;
        tempInfo->iLastNormalTick = 0;
        tempInfo->iLastStatus = -1;

        CamStatusList.append(tempInfo);

        iFindIndex = CamStatusList.count() -1;

        GLogModel::GetInstant()->WriteLog("StatustCheck",QString("findIndexFromCamInfoListByIp ,appdend %1 to List.").arg(IpAddress));
    }

    return iFindIndex;
}

void StatustCheck::clearCamStatusList()
{
    GLogModel::GetInstant()->WriteLog("StatustCheck","clearCamStatusList begin.");
    QMutexLocker locker(&m_mutex);

    CamStatusInfo* tempInfo = NULL;
    for(int i = 0; i< CamStatusList.count(); i++)
    {
        tempInfo = CamStatusList.front();
        if(tempInfo)
        {
            delete tempInfo;
            tempInfo = NULL;
        }
        CamStatusList.pop_front();
    }
    GLogModel::GetInstant()->WriteLog("StatustCheck","clearCamStatusList finish.");
}

void StatustCheck::CheckChangeTimes(CamStatusInfo * Info,QString StatusText, int timeRange, int MaxChangeTimes)
{
    if(!Info)
    {
        GLogModel::GetInstant()->WriteLog("StatustCheck","CheckChangeTimes, the parameter 'Info' is null, return an do noting.");
        return;
    }

    QString qstrLog;

    qint64 i64TimeRange = 0;
    int iChangeTimes = 0;

    if(timeRange <= 0 || MaxChangeTimes <= 0)
    {
        i64TimeRange = 10 * MS_OF_ONE_MIMINUTE;
        iChangeTimes = 5;

        GLogModel::GetInstant()->WriteLog("StatustCheck","CheckChangeTimes, the parameter 'partimeRange'' or 'MaxChangeTimes'' less than 0, use default 10 minutes,and 5 count.");
    }
    else
    {
        i64TimeRange = timeRange * MS_OF_ONE_MIMINUTE;
        iChangeTimes = MaxChangeTimes;
    }

    CamStatusInfo * tempInfo = Info;
    int iStatus = -1;

    if(StatusText == tr("DisConnect"))
    {
        iStatus = -1;
    }
    else
    {
        iStatus = 0;
    }

    if(tempInfo->iCheckTimeBegin == 0)
    {
        tempInfo->iCheckTimeBegin = QDateTime::currentMSecsSinceEpoch();
    }

    qint64 iDvalue = QDateTime::currentMSecsSinceEpoch() -tempInfo->iCheckTimeBegin;
    if(tempInfo->iLastStatus != iStatus)
    {
        if(iDvalue <= i64TimeRange)
        {
            tempInfo->iChangeTimes++;

            qstrLog.clear();
            qstrLog = QString("CheckChangeTimes, %1 `s status is change from %2 to %3").arg(tempInfo->qstrCamIP).arg(tempInfo->iLastStatus).arg(iStatus);
            GLogModel::GetInstant()->WriteLog("StatustCheck",qstrLog);
        }
        else
        {
            //当检测的时间超过指定时间范围时重置
            tempInfo->iChangeTimes = 0;
            tempInfo->iCheckTimeBegin = QDateTime::currentMSecsSinceEpoch();

            qstrLog.clear();
            qstrLog = QString("CheckChangeTimes, %1 is out of time range, reset.").arg(tempInfo->qstrCamIP);
            GLogModel::GetInstant()->WriteLog("StatustCheck",qstrLog);
        }
        tempInfo->iLastStatus = iStatus;
    }
    else
    {
        if(iDvalue > i64TimeRange)
        {
            //当检测的时间超过指定时间范围时重置
            tempInfo->iChangeTimes = 0;
            tempInfo->iCheckTimeBegin = QDateTime::currentMSecsSinceEpoch();

            qstrLog.clear();
            qstrLog = QString("CheckChangeTimes, %1 status is no chage, and is out of time range, reset.").arg(tempInfo->qstrCamIP);
            GLogModel::GetInstant()->WriteLog("StatustCheck",qstrLog);
        }
    }

    if(tempInfo->iChangeTimes > iChangeTimes)
    {
        qstrLog.clear();
        qstrLog = tr("CheckChangeTimes, %1 status change times is over range %2, send alarm message.").arg(tempInfo->qstrCamIP).arg(iChangeTimes);
        //qstrLog = QString::fromLocal8Bit("CheckChangeTimes, %1 在指定时间段内状态变化次数过多，变化次数为%2 次, 发送警报消息.").arg(tempInfo->qstrCamIP).arg(iChangeTimes);

        AlarmMessage alarMsg;
        alarMsg.iType = ALARM_EVENT_DISCONNECT_TIMEOUT;
        alarMsg.qstrDeviceIP = tempInfo->qstrCamIP;
        alarMsg.qstrContent = qstrLog;

        //报警信号发送后，要重置
        tempInfo->iChangeTimes = 0;


        GLogModel::GetInstant()->WriteLog("StatustCheck", qstrLog);
        emit signal_SendAlarm(alarMsg);
    }
}

void StatustCheck::CheckDisConnectTimeOut(CamStatusInfo *Info, QString StatusText, int timeOut)
{
    if(!Info)
    {
        GLogModel::GetInstant()->WriteLog("StatustCheck","CheckDisConnectTimeOut, the parameter 'Info' is null, return an do noting.");
        return;
    }

    QString qstrLog;
    CamStatusInfo * tempInfo = Info;
    qint64 iTimeOut = 0;
    if(timeOut <= 0)
    {
        iTimeOut = 5 * MS_OF_ONE_MIMINUTE;

        GLogModel::GetInstant()->WriteLog("StatustCheck","CheckDisConnectTimeOut, the parameter 'timeOut''  less than 0, use default 5 minutes.");
    }
    else
    {
        iTimeOut = timeOut * MS_OF_ONE_MIMINUTE;
    }

    if(StatusText == tr("DisConnect"))
    {
        if(tempInfo->iLastNormalTick == 0)
        {
            //如果是第一次获取状态,且为未连接，则此时初始化该变量，计时开始
            tempInfo->iLastNormalTick = QDateTime::currentMSecsSinceEpoch();

            qstrLog.clear();
            qstrLog = QString("CheckDisConnectTimeOut, %1 `s is new, reset to begin.").arg(tempInfo->qstrCamIP);
            GLogModel::GetInstant()->WriteLog("StatustCheck",qstrLog);
        }
        else
        {
            qint64 iDvalue = QDateTime::currentMSecsSinceEpoch() - tempInfo->iLastNormalTick;
            if(iDvalue > iTimeOut)
            {
                tempInfo->iLastNormalTick = 0;

                //如果不正常的状态超过指定时间，则发出警报信号
                qstrLog.clear();
                qstrLog = tr("CheckDisConnectTimeOut, %1 is disconnect and over range the timeout %2 minutes").arg(tempInfo->qstrCamIP).arg(timeOut);
                //qstrLog = QString::fromLocal8Bit("CheckDisConnectTimeOut, %1 断开状态持续时间超过 %2 分钟").arg(tempInfo->qstrCamIP).arg(timeOut);

                AlarmMessage alarMsg;
                alarMsg.iType = ALARM_EVENT_DISCONNECT_TIMEOUT;
                alarMsg.qstrDeviceIP = tempInfo->qstrCamIP;
                alarMsg.qstrContent = qstrLog;

                GLogModel::GetInstant()->WriteLog("StatustCheck", qstrLog);
                emit signal_SendAlarm(alarMsg);
            }
        }
    }
    else
    {
        //如果但前状态是正常的，则更新正常状态的时间
        tempInfo->iLastNormalTick = QDateTime::currentMSecsSinceEpoch();
    }
}
