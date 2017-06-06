#include "UnlicensePlateCheck.h"
#include<QMutexLocker>
#include<QDebug>
#include"LogModel/glogmodel.h"
#include"data_commomdef.h"
#include<QRegExp>

UnlicensePlateCheck::UnlicensePlateCheck(QObject *parent)
    : BaseEventCheck(parent),
      m_iRate(0.30)
{
    m_bExitCheck = false;
    m_pDataModel = NULL;
}

UnlicensePlateCheck::~UnlicensePlateCheck()
{
    SetDataModel(NULL);
    StopCheck();
}

void UnlicensePlateCheck::slot_StartEvent()
{
    GLogModel::GetInstant()->WriteLog("UnlicensePlateCheck","StartEvent begin.");

    bool bRet = false;
    int iUnlicenseRate = 0.10;
    CustomTableModel* pDataModel = NULL;

    while(!bRet)
    {
        m_mutex.lock();
        bRet = m_bExitCheck;
        pDataModel = m_pDataModel;
        iUnlicenseRate = m_iRate;
        m_mutex.unlock();

        if(pDataModel)
        {
            int iRowCount = pDataModel->GetRowCount();
            if (iRowCount <= 0)
                continue;
            for(int i = 0; i < iRowCount; i ++)
            {
                //QString qstrText = pDataModel->FindColTextFromRow(i, 3);
                int iRate = 0;
                QString qstrText = pDataModel->FindColTextFromRow(i, RATE_COLUMN_NUMBER);
                QRegExp rx("(\\d+)");  // Æ¥ÅäÊý×Ö
                int pos = 0;
                if ((pos = rx.indexIn(qstrText, pos)) != -1) {
                    iRate = rx.cap(0).toInt();
                    pos += rx.matchedLength();
                }

                if(iRate > iUnlicenseRate)
                {
                    //QString qstrIp = pDataModel->FindColTextFromRow(i, 1);
                    QString qstrIp = pDataModel->FindColTextFromRow(i, IPADDRESS_COLUMN_NUMBER);
                    QString qstrLog = tr("%1 `s Rate is %2%  over range of the Unlicense rate %3%").arg(qstrIp).arg(iRate).arg(iUnlicenseRate);
                    AlarmMessage alarMsg;
                    alarMsg.iType = ALARM_EVENT_UNLICENSE;
                    alarMsg.qstrDeviceIP = qstrIp;
                    alarMsg.qstrContent = qstrLog;

                    GLogModel::GetInstant()->WriteLog("UnlicensePlateCheck", qstrLog);
                    emit signal_SendAlarm(alarMsg);
                }
            }
        }
        QThread::currentThread()->sleep(60);
    }

    GLogModel::GetInstant()->WriteLog("UnlicensePlateCheck","StartEvent end.");
}

void UnlicensePlateCheck::StopCheck()
{
    QMutexLocker locker(&m_mutex);
    m_bExitCheck = true;
}

void UnlicensePlateCheck::SetDataModel(CustomTableModel *DataModel)
{
    QMutexLocker locker(&m_mutex);
    m_pDataModel = DataModel;
}

void UnlicensePlateCheck::SetUnlicenseRate(int Rate)
{
    QString qstrLog;
    qstrLog = QString("SetUnlicenseRate ,input Rate = %1").arg(Rate);
    GLogModel::GetInstant()->WriteLog("UnlicensePlateCheck",qstrLog);

    if(Rate > 0)
    {
        m_iRate = Rate;
    }

    qstrLog = QString("SetUnlicenseRate ,final Rate = %1%").arg(m_iRate);
    GLogModel::GetInstant()->WriteLog("UnlicensePlateCheck",qstrLog);
}

