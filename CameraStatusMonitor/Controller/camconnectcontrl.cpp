#include "camconnectcontrl.h"
#include"LogModel/glogmodel.h"
#include<QDebug>
#include<QMutexLocker>
#include"data_commomdef.h"


CamConnectContrl::CamConnectContrl(QObject *parent)
    : QObject(parent),
      m_pDataModel(NULL),
      m_bUpLoadExit(false)
{
    this->moveToThread(&uploadWorkerThread);
    connect(this, signal_UpLoadCamerStatus, this, slot_UpLoadCamerStatus);
    uploadWorkerThread.start();
}

CamConnectContrl::~CamConnectContrl()
{
    ExitUpLoad();
    SetDataModel(NULL);

    disconnect(this, signal_UpLoadCamerStatus, this, slot_UpLoadCamerStatus);


    uploadWorkerThread.quit();
    uploadWorkerThread.wait();

     GLogModel::GetInstant()->WriteLog("CamConnectContrl", QString("Thread id [%1] ~CamConnectContrl .").arg(quintptr(QThread::currentThreadId())));
}

bool CamConnectContrl::addCameraByIPaddress(QString ipaddress, QString CameraName)
{
    if(ipaddress.isEmpty())
    {
        GLogModel::GetInstant()->WriteLog("CamConnectContrl", QString("Thread id [%1] addCameraByIPaddress failed, the ip address is null.").arg(quintptr(QThread::currentThreadId())));
        return false;
    }

    int index = -1;
    for(int i = 0 ; i < m_pCamList.count(); i++)
    {
       Camera6467* pCamer = NULL;
        pCamer = m_pCamList[i];
        if(pCamer && ipaddress == QString(pCamer->GetCameraIP().c_str()))
        {
            index = i;
            break;
        }
    }

    if(index == -1)
    {
        Camera6467* pNewCamera = new Camera6467(ipaddress.toStdString().c_str());
        if(pNewCamera)
        {
            CameraInfo tempInfo;
            sprintf(tempInfo.chIP,"%s", ipaddress.toStdString().c_str());
            sprintf(tempInfo.chStationID,"%s", CameraName.toStdString().c_str());

            if(!CameraName.isEmpty())
            {
                pNewCamera->SetCameraInfo(tempInfo);                
            }
            PushDataToModel(tempInfo);      //加入设备时，先放一条信息到界面

            m_pCamList.push_back(pNewCamera);
            pNewCamera = NULL;

            GLogModel::GetInstant()->WriteLog("CamConnectContrl", QString("Thread id [%1] addCameraByIPaddress success, the ip =%2").arg(quintptr(QThread::currentThreadId())).arg(ipaddress));

            return true;
        }
        else
        {
            GLogModel::GetInstant()->WriteLog("CamConnectContrl", QString("Thread id [%1] addCameraByIPaddress failed, create object failed, the ip =%2").arg(quintptr(QThread::currentThreadId())).arg(ipaddress));
            return false;
        }
    }
    else
    {
        GLogModel::GetInstant()->WriteLog("CamConnectContrl", QString("Thread id [%1] addCameraByIPaddress failed, the camera is already exist, the ip =%1").arg(quintptr(QThread::currentThreadId())).arg(ipaddress));
        return false;
    }
}

void CamConnectContrl::SetDataModel(CustomTableModel *DataModel)
{
    m_pDataModel = DataModel;
}

void CamConnectContrl::StarUpLoad()
{
    emit signal_UpLoadCamerStatus();
}

void CamConnectContrl::slot_UpLoadCamerStatus()
{
    GLogModel::GetInstant()->WriteLog("CamConnectContrl", QString("Thread id [%1] UpLoadCamerStatus begin.").arg(quintptr(QThread::currentThreadId())));
    bool bExit = false;

    while(!bExit)
    {
        //GLogModel::GetInstant()->WriteLog("CamConnectContrl", QString("Thread id [%1] UpLoadCamerStatus run..").arg(quintptr(QThread::currentThreadId())));

        m_mutex.lock();
        bExit = m_bUpLoadExit;
        m_mutex.unlock();
        if(bExit)
            break;

        slot_ConnectCamer();

        for(int i = 0 ; i < m_pCamList.count(); i++)
        {
           Camera6467* pCamer = NULL;
            pCamer = m_pCamList[i];
            if(pCamer)
            {
                CameraInfo tempCamInfo = pCamer->GetCameraInfo();
                PushDataToModel(tempCamInfo);
            }
        }
        QThread::currentThread()->sleep(1);
    }
    GLogModel::GetInstant()->WriteLog("CamConnectContrl", QString("Thread id [%1] UpLoadCamerStatus end.").arg(quintptr(QThread::currentThreadId())));
}

void CamConnectContrl::ExitUpLoad()
{
    m_mutex.lock();
    m_bUpLoadExit = true;
    m_mutex.unlock();
    GLogModel::GetInstant()->WriteLog("CamConnectContrl",QString("ExitUpLoad %1.").arg(quintptr(QThread::currentThreadId())));
}

void CamConnectContrl::slot_ConnectCamer()
{
    QMutexLocker locker(&m_mutex);
//    GLogModel::GetInstant()->WriteLog("CamConnectContrl",QString("Thread id [%1] ConnectCamer.").arg(quintptr(QThread::currentThreadId())));

    for(int i = 0 ; i < m_pCamList.count(); i++)
    {
       Camera6467* pCamer = NULL;
        pCamer = m_pCamList[i];
        if(pCamer && pCamer->IsFirstConnect())
        {
            bool bRet = pCamer->ConnectToCamera();
            GLogModel::GetInstant()->WriteLog("CamConnectContrl",QString("Thread id [%1] %2 begin Connect to camera....").arg(quintptr(QThread::currentThreadId())).arg(QString(pCamer->GetCameraIP().c_str())));

            if(bRet)
            {
                GLogModel::GetInstant()->WriteLog("CamConnectContrl",QString("Thread id [%1] Connect success").arg(quintptr(QThread::currentThreadId())));
            }
            else
            {
                GLogModel::GetInstant()->WriteLog("CamConnectContrl",QString("Thread id [%1] Connect failed, but it will reconnect soon.").arg(quintptr(QThread::currentThreadId())));
            }
        }
    }
//    GLogModel::GetInstant()->WriteLog("CamConnectContrl",QString("Thread id [%1] ConnectCamer finished.").arg(quintptr(QThread::currentThreadId())));
}

void CamConnectContrl::PushDataToModel(CameraInfo &info)
{
     if(m_pDataModel)
     {
         //目前定义列表为  IP - ConnectInfo - UnlicensedPlateRatio
         //QString qstrStationID = QString(info.chStationID);
         QString qstrIP = QString(info.chIP);
         QString qstrConnectInfo = (info.iConnectStatus == 0 ? tr("Connect") : tr("DisConnect"));
         QString qstrfUnlicensedPlateRatio = QString("%1%").arg(QString::number(info.fUnlicensedPlateRatio *100, 'f', 2));

         QStringList CamInfoList;
         //CamInfoList.append(qstrStationID);
         CamInfoList.append(qstrIP);
         CamInfoList.append(qstrConnectInfo);
         CamInfoList.append(qstrfUnlicensedPlateRatio);

         //int index = m_pDataModel->FindRowFromQString(1, qstrIP);
         int index = m_pDataModel->FindRowFromQString(IPADDRESS_COLUMN_NUMBER, qstrIP);
         if(index > -1)
         {
             m_pDataModel->UpdataRowFromQStringList(index, CamInfoList);
         }
         else
         {
             m_pDataModel->InsertRowFromQStringList(CamInfoList);
         }
     }
}

