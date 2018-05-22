#include "plateblacklistcheck.h"
#include"LogModel/glogmodel.h"
#include"UtilityFunction/utilityfunction.h"
#include"DataModel/custresultlist.h"
#include"data_commomdef.h"
#include <QTextCodec>
#include<QtAlgorithms>
#include<QDateTime>


#define UPDATE_TIME (5*60*1000)

PlateBlackListCheck::PlateBlackListCheck(QObject *parent):
    BaseEventCheck(parent),
    m_bExitCheck(false)
{

}

PlateBlackListCheck::~PlateBlackListCheck()
{
    StopCheck();
    ClearResultList();
}

void PlateBlackListCheck::slot_StartEvent()
{
    GLogModel::GetInstant()->WriteLog("PlateBlackListCheck","StartEvent begin.");
    QString qstrLog;

    bool bRet = false;
    qint64 iCurrentTime = 0;
    QStringList lsPlateBlacklist, lsPlateWhitelist;
    CameraResult* pTempResult = NULL;
    QTextCodec* textCdec = QTextCodec::codecForName("GB18030");
    while(!bRet)
    {
        QThread::currentThread()->msleep(100);
        bRet = GetExitStatus();

        //update list
        if(iCurrentTime == 0 || QDateTime::currentMSecsSinceEpoch() - iCurrentTime >= UPDATE_TIME )
        {
            lsPlateBlacklist.clear();
            lsPlateWhitelist.clear();
            GetPlateBlackList(BLACK_LIST_FILE_NAME, lsPlateBlacklist);
            GetPlateWhiteList(WHITE_LIST_FILE_NAME, lsPlateWhitelist);
            iCurrentTime = QDateTime::currentMSecsSinceEpoch();
        }
        //pTempResult = GetOneResultFromList();
        pTempResult = GetOneResultFromResultList();
        if(pTempResult)
        {
            QString qstrPlateNo =  textCdec->toUnicode(pTempResult->chPlateNO);
            if(ComparePlateWithBlackList(lsPlateBlacklist,qstrPlateNo ))
            {
                qstrLog.clear();
                qstrLog= tr("%1 Find from the black list, send alarm signal.").arg(qstrPlateNo);
                GLogModel::GetInstant()->WriteLog("PlateBlackListCheck",qstrLog);

                AlarmMessage alarMsg;
                alarMsg.iType = ALARM_EVENT_BALCKLIST;
                alarMsg.qstrDeviceIP = pTempResult->chDeviceIp;
                alarMsg.qstrContent = qstrLog;

                emit signal_SendAlarm(alarMsg);
            }
            else if(ComparePlateWithWhiteList(lsPlateWhitelist, qstrPlateNo))
            {
                qstrLog.clear();
                qstrLog= tr("%1 Find from the white list, send alarm signal.").arg(qstrPlateNo);
                GLogModel::GetInstant()->WriteLog("PlateBlackListCheck",qstrLog);

                AlarmMessage alarMsg;
                alarMsg.iType = ALARM_EVENT_WHITELIST;
                alarMsg.qstrDeviceIP = pTempResult->chDeviceIp;
                alarMsg.qstrContent = qstrLog;

                emit signal_SendAlarm(alarMsg);
            }
            else
            {
                qstrLog.clear();
                qstrLog= tr("%1 not find from the white list.").arg(qstrPlateNo);
                GLogModel::GetInstant()->WriteLog("PlateBlackListCheck",qstrLog);
            }
            delete pTempResult;
            pTempResult = NULL;
            GLogModel::GetInstant()->WriteLog("PlateBlackListCheck","Compare finish.");
        }
    }

    GLogModel::GetInstant()->WriteLog("PlateBlackListCheck","StartEvent end.");
}

void PlateBlackListCheck::StopCheck()
{
    QMutexLocker locker(&m_mutex);
    m_bExitCheck = true;
}

bool PlateBlackListCheck::GetPlateBlackList(const QString filePath, QStringList &plateList)
{
    bool bRet = GetStringListFromFile(filePath,plateList);
    if(bRet)
    {
        GLogModel::GetInstant()->WriteLog("PlateBlackListCheck", "Get BlackList:");
        for(auto &it: plateList)
        {
            GLogModel::GetInstant()->WriteLog("PlateBlackListCheck", it);
        }
    }
    else
    {
        GLogModel::GetInstant()->WriteLog("PlateBlackListCheck", "Get BlackList failed.");
    }
    return bRet;
}

bool PlateBlackListCheck::ComparePlateWithBlackList(const QStringList &plateList,const QString plateNo)
{
    QString qstrLog = QString("ComparePlateWithBlackList, plateNo = %1").arg(plateNo);
    GLogModel::GetInstant()->WriteLog("PlateBlackListCheck",qstrLog);

    QStringList::const_iterator i1 = qFind(plateList.begin(), plateList.end(), plateNo);
    if(i1 != plateList.end())
    {
        GLogModel::GetInstant()->WriteLog("PlateBlackListCheck","find the plate NO in the Black list.");
        return true;
    }
    else
    {
        GLogModel::GetInstant()->WriteLog("PlateBlackListCheck","could not find the plate NO from the Black list.");
        return false;
    }
}

bool PlateBlackListCheck::GetPlateWhiteList(const QString filePath, QStringList &plateList)
{
    bool bRet = GetStringListFromFile(filePath,plateList);
    if(bRet)
    {
        GLogModel::GetInstant()->WriteLog("PlateBlackListCheck", "Get WhiteList:");
        for(auto &it: plateList)
        {
            GLogModel::GetInstant()->WriteLog("PlateBlackListCheck", it);
        }
    }
    else
    {
        GLogModel::GetInstant()->WriteLog("PlateBlackListCheck", "Get WhiteList failed.");
    }
    return bRet;
}

bool PlateBlackListCheck::ComparePlateWithWhiteList(const QStringList &plateList,const QString plateNo)
{
    QString qstrLog = QString("ComparePlateWithWhiteList, plateNo = %1").arg(plateNo);
    GLogModel::GetInstant()->WriteLog("PlateBlackListCheck",qstrLog);

    QStringList::const_iterator i1 = qFind(plateList.begin(), plateList.end(), plateNo);
    if(i1 != plateList.end())
    {
        GLogModel::GetInstant()->WriteLog("PlateBlackListCheck","find the plate NO in the White list.");
        return true;
    }
    else
    {
        GLogModel::GetInstant()->WriteLog("PlateBlackListCheck","could not find the plate NO from the White list.");
        return false;
    }
}

bool PlateBlackListCheck::AddOneToPlateList(CameraResult *result)
{
    if(!result)
    {
        GLogModel::GetInstant()->WriteLog("PlateBlackListCheck","AddOneToPlateList failed, result is null.");
        return false;
    }

    if(m_mutex.tryLock(500))
    {
        QTextCodec* textCdec = QTextCodec::codecForName("GB18030");
        QString qstrLog = QString("AddOneToPlateList, plateNo = %1").arg(textCdec->toUnicode(result->chPlateNO));
        GLogModel::GetInstant()->WriteLog("PlateBlackListCheck",qstrLog);

        m_lsplateResultList.push_back(result);
        m_mutex.unlock();
        return true;
    }
    else
    {
        GLogModel::GetInstant()->WriteLog("PlateBlackListCheck","AddOneToPlateList failed, tryLock is time out.");
        return false;
    }
}

CameraResult *PlateBlackListCheck::GetOneResultFromList()
{
    CameraResult* tempResult = NULL;
    if(m_mutex.tryLock(500))
    {
        tempResult = m_lsplateResultList.front();
        m_lsplateResultList.pop_front();

        m_mutex.unlock();

        QTextCodec* textCdec = QTextCodec::codecForName("GB18030");
        QString qstrLog = QString("GetOneResultFromList, plateNo = %1").arg(textCdec->toUnicode(tempResult->chPlateNO));
        GLogModel::GetInstant()->WriteLog("PlateBlackListCheck",qstrLog);
    }
    else
    {
        GLogModel::GetInstant()->WriteLog("PlateBlackListCheck","GetOneResultFromList failed, tryLock is time out.");
    }
    return tempResult;
}

CameraResult* PlateBlackListCheck::GetOneResultFromResultList()
{
    CameraResult* pTemp = NULL;
    m_mutex.lock();
    if(g_pList)
    {
        m_mutex.unlock();
        if(0 != ((ThreadSafeList<CameraResult*> *)g_pList)->TimePop(pTemp, 100))
        {
            pTemp = NULL;
        }
    }
    else
    {
        m_mutex.unlock();
    }
    return pTemp;
}

bool PlateBlackListCheck::GetExitStatus()
{
    bool bSatus = false;
    m_mutex.lock();
    bSatus = m_bExitCheck;
    m_mutex.unlock();
    return bSatus;
}

void PlateBlackListCheck::SetPlateList(void *plist)
{
    QMutexLocker locker(&m_mutex);
    g_pList = plist;
}

void PlateBlackListCheck::ClearResultList()
{
    CameraResult* pTempResult = NULL;
    while(GetResultListSize()> 0)
    {
        pTempResult = GetOneResultFromList();
        if(pTempResult)
        {
            delete pTempResult;
            pTempResult = NULL;
        }
    }
}

int PlateBlackListCheck::GetResultListSize()
{
    int iListSize = 0;
    m_mutex.lock();
    iListSize = m_lsplateResultList.size();
    m_mutex.unlock();
    return iListSize;
}
