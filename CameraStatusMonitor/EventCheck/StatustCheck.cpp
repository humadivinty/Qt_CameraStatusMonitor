#include "StatustCheck.h"
#include<QMutexLocker>
#include<QDebug>
#include"LogModel/glogmodel.h"

StatustCheck::StatustCheck(QObject *parent)
    : BaseEventCheck(parent),
      m_iCount(5)
{
    m_bExitCheck = false;
    m_pDataModel = NULL;
}

StatustCheck::~StatustCheck()
{
    SetDataModel(NULL);
    StopCheck();
}

void StatustCheck::slot_StartEvent()
{
    GLogModel::GetInstant()->WriteLog("StatustCheck","StartEvent begin.");

    bool bRet = false;
    CustomTableModel* pDataModel = NULL;

    while(!bRet)
    {
        m_mutex.lock();
        bRet = m_bExitCheck;
        pDataModel = m_pDataModel;
        m_mutex.unlock();

        if(pDataModel)
        {
            int iRowCount = pDataModel->GetRowCount();
            if (iRowCount <= 0)
                continue;
            for(int i = 0; i < iRowCount; i ++)
            {
                QString qstrText = pDataModel->FindColTextFromRow(i, 2);
                if(qstrText == QString("DisConnect"))
                {
                    QString qstrIp = pDataModel->FindColTextFromRow(i, 1);
                    QString qstrLog = QString("%1 is disconnect").arg(qstrIp);
                    AlarmMessage alarMsg;
                    alarMsg.iType = ALARM_EVENT_DISCONNECT;
                    alarMsg.qstrContent = qstrLog;

                    GLogModel::GetInstant()->WriteLog("StatustCheck", qstrLog);
                    emit signal_SendAlarm(alarMsg);
                }
            }
        }
        QThread::currentThread()->sleep(10);
    }

    GLogModel::GetInstant()->WriteLog("StatustCheck","StartEvent end.");
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

void StatustCheck::SetDisConCount(int Count)
{
    QString qstrLog;
    qstrLog = QString("SetDisConCount ,input Count = %1").arg(Count);
    GLogModel::GetInstant()->WriteLog("StatustCheck",qstrLog);

    if(Count > 0)
    {
      m_iCount = Count;
    }

    qstrLog = QString("SetDisConCount ,final Count = %1").arg(m_iCount);
    GLogModel::GetInstant()->WriteLog("StatustCheck",qstrLog);
}

