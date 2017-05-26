#include "UnlicensePlateCheck.h"
#include<QMutexLocker>
#include<QDebug>
#include"LogModel/glogmodel.h"

UnlicensePlateCheck::UnlicensePlateCheck(QObject *parent)
    : BaseEventCheck(parent),
      m_fRate(0.30)
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
    float fUnlicenseRate = 0.10;
    CustomTableModel* pDataModel = NULL;

    while(!bRet)
    {
        m_mutex.lock();
        bRet = m_bExitCheck;
        pDataModel = m_pDataModel;
        fUnlicenseRate = m_fRate;
        m_mutex.unlock();

        if(pDataModel)
        {
            int iRowCount = pDataModel->GetRowCount();
            if (iRowCount <= 0)
                continue;
            for(int i = 0; i < iRowCount; i ++)
            {
                QString qstrText = pDataModel->FindColTextFromRow(i, 3);
                float fRate = qstrText.toFloat();
                if(fRate > fUnlicenseRate)
                {
                    QString qstrIp = pDataModel->FindColTextFromRow(i, 1);
                    QString qstrLog = QString("%1 `s Rate is %2  over range of the Unlicense rate %2").arg(qstrIp).arg(QString::number(fRate, 'f', 2)).arg(QString::number(fUnlicenseRate, 'f', 2));
                    AlarmMessage alarMsg;
                    alarMsg.iType = ALARM_EVENT_UNLICENSE;
                    alarMsg.qstrContent = qstrLog;

                    GLogModel::GetInstant()->WriteLog("UnlicensePlateCheck", qstrLog);
                    emit signal_SendAlarm(alarMsg);
                }
            }
        }
        QThread::currentThread()->sleep(1);
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
        int iRate = Rate%100;
        m_fRate = (float)iRate/(float)100;
    }

    qstrLog = QString("SetUnlicenseRate ,final Rate = %1").arg(QString::number(m_fRate, 'f', 2));
    GLogModel::GetInstant()->WriteLog("UnlicensePlateCheck",qstrLog);
}

