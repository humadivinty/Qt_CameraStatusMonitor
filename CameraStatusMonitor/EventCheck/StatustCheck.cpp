#include "StatustCheck.h"
#include<QMutexLocker>
#include<QDebug>
#include"LogModel/glogmodel.h"

StatustCheck::StatustCheck(QObject *parent) : BaseEventCheck(parent)
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
                    qDebug()<<qstrIp<<"is DisConnect";
                }
            }
        }
        QThread::currentThread()->sleep(1);
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

