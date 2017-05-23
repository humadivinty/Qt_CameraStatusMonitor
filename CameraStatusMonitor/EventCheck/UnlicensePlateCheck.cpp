#include "UnlicensePlateCheck.h"
#include<QMutexLocker>
#include<QDebug>
#include"LogModel/glogmodel.h"

UnlicensePlateCheck::UnlicensePlateCheck(QObject *parent)
    : BaseEventCheck(parent)
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
                QString qstrText = pDataModel->FindColTextFromRow(i, 3);
                float fRate = qstrText.toFloat();
                if(fRate > float(0.0))
                {
                    QString qstrIp = pDataModel->FindColTextFromRow(i, 1);
                    qDebug()<<qstrIp<<"is  beyong 0, Rate is:"<<qstrText;
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

