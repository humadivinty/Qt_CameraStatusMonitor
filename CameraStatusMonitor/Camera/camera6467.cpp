#include "Camera/camera6467.h"
#include "Camera/cameraresult.h"
#include "HvDeviceAPI/SWErrCode.h"
#include <QDir>
#include <QTextStream>
#include <QDate>
#include <QTime>
#include <QDebug>
#include <QSettings>
#define MAX_PATH 260

Camera6467::Camera6467()
    : m_CameraResult(NULL),
      m_hHvHandle(NULL),
      m_iConnectStatus(0),
      m_bLogEnable(false),
      m_bSynTime(false),
      m_bResultComplete(false),
      m_bFirstConnect(true),
      m_dwConnectStatus(-1),
      m_iCompressQuality(30),
      m_iDirection(0),
      m_iTotalPlateCount(0),
      m_iUnlisensePlateCount(0)
{
    memset(m_chStationID, 0 , sizeof(m_chStationID));
    memset(m_chDeviceID, 0 , sizeof(m_chDeviceID));

    ReadConfig();
}

Camera6467::Camera6467(const char *chIP)
    : m_CameraResult(NULL),
      m_hHvHandle(NULL),
      m_iConnectStatus(0),
      m_bLogEnable(false),
      m_bSynTime(false),
      m_bResultComplete(false),
      m_bFirstConnect(true),
      m_dwConnectStatus(-1),
      m_iCompressQuality(30),
      m_iDirection(0),
      m_iTotalPlateCount(0),
      m_iUnlisensePlateCount(0)
{
    if(NULL != chIP)
    {
        m_strIP = std::string(chIP);
    }
    memset(m_chStationID, 0 , sizeof(m_chStationID));
    memset(m_chDeviceID, 0 , sizeof(m_chDeviceID));

    ReadConfig();
}

Camera6467::~Camera6467()
{
    if(NULL != m_hHvHandle)
    {
        HVAPI_CloseEx((HVAPI_HANDLE_EX)m_hHvHandle);
        m_hHvHandle = NULL;
    }
    if(NULL != m_CameraResult)
    {
        delete m_CameraResult;
        m_CameraResult = NULL;
    }

}

bool Camera6467::SetCameraInfo(CameraInfo &camInfo)
{
    m_strIP = std::string(camInfo.chIP);
    m_strDeviceID = std::string(camInfo.chDeviceID);
    sprintf(m_chDeviceID, "%s", camInfo.chDeviceID);
    sprintf(m_chLaneID, "%s", camInfo.chLaneID);
    sprintf(m_chStationID, "%s", camInfo.chStationID);
    m_bLogEnable = camInfo.bLogEnable;
    m_bSynTime = camInfo.bSynTimeEnable;
    m_iDirection = camInfo.iDirection;

    QMutexLocker Locker(&m_ResultMutex);

    strcpy(m_CamInfo.chIP,this->m_strIP.c_str());
    strcpy(m_CamInfo.chDeviceID, this->m_chDeviceID);
    strcpy(m_CamInfo.chLaneID, this->m_chLaneID);
    strcpy(m_CamInfo.chStationID, this->m_chStationID);
    m_CamInfo.fUnlicensedPlateRatio = 0.0;
    m_CamInfo.iConnectStatus = 1;  //Connect :  0 ; DisConnect : 1
    m_CamInfo.iDirection = camInfo.iDirection;
    m_CamInfo.bIsInit = false;
    m_CamInfo.bLogEnable = camInfo.bLogEnable;
    m_CamInfo.bSynTimeEnable = camInfo.bSynTimeEnable;;

    return true;
}

bool Camera6467::ConnectToCamera()
{
    if(m_strIP.empty())
    {
        WriteLog("ConnectToCamera:: please finish the camera ip address");
        return false;
    }
    if(NULL != m_hHvHandle)
    {
        HVAPI_CloseEx((HVAPI_HANDLE_EX)m_hHvHandle);
        m_hHvHandle = NULL;
    }
    m_hHvHandle = HVAPI_OpenEx(m_strIP.c_str(), NULL);
    if(NULL == m_hHvHandle)
    {
        WriteLog("ConnectToCamera:: Open CameraHandle failed!");
        return false;
    }

    ReadHistoryInfo();
    char chCommand[1024] = {0};
    sprintf(chCommand, "DownloadRecord,BeginTime[%s],Index[%d],Enable[%d],EndTime[%s],DataInfo[%d]", m_SaveModelInfo.chBeginTime, m_SaveModelInfo.iIndex, m_SaveModelInfo.iSafeModeEnable, m_SaveModelInfo.chEndTime, m_SaveModelInfo.iDataType);
    qDebug()<<chCommand;
    WriteLog(chCommand);

    if ( (HVAPI_SetCallBackEx(m_hHvHandle, (PVOID)RecordInfoBeginCallBack, this, 0, CALLBACK_TYPE_RECORD_INFOBEGIN, /*NULL*/chCommand) != S_OK) ||
         (HVAPI_SetCallBackEx(m_hHvHandle, (PVOID)RecordInfoPlateCallBack, this, 0, CALLBACK_TYPE_RECORD_PLATE, /*NULL*/chCommand) != S_OK) ||
//         (HVAPI_SetCallBackEx(m_hHvHandle, (PVOID)RecordInfoBigImageCallBack, this, 0, CALLBACK_TYPE_RECORD_BIGIMAGE, /*NULL*/chCommand) != S_OK) ||
//         (HVAPI_SetCallBackEx(m_hHvHandle, (PVOID)RecordInfoSmallImageCallBack, this, 0, CALLBACK_TYPE_RECORD_SMALLIMAGE, /*NULL*/chCommand) != S_OK) ||
//         (HVAPI_SetCallBackEx(m_hHvHandle, (PVOID)RecordInfoBinaryImageCallBack, this, 0, CALLBACK_TYPE_RECORD_BINARYIMAGE, /*NULL*/chCommand) != S_OK) ||
         (HVAPI_SetCallBackEx(m_hHvHandle, (PVOID)RecordInfoEndCallBack, this, 0, CALLBACK_TYPE_RECORD_INFOEND, /*NULL*/chCommand) != S_OK)
         )
    {
        qDebug()<<m_strIP.c_str()<<"ConnectToCamera:: SetCallBack failed.";
        WriteLog("ConnectToCamera:: SetCallBack failed.");
        HVAPI_CloseEx(m_hHvHandle);
        m_hHvHandle = NULL;
        m_dwConnectStatus = -1;
        return false;
    }
    else
    {
        m_dwConnectStatus = 0;
        WriteLog("ConnectToCamera:: SetCallBack success.");
        qDebug()<<m_strIP.c_str()<<"ConnectToCamera:: SetCallBack success.";
        m_bFirstConnect = false;
    }

    return true;
}

int Camera6467::GetCamStatus()
{
    if(NULL == m_hHvHandle)
        return 1;
    DWORD dwStatus = 1;

    if ( HVAPI_GetConnStatusEx((HVAPI_HANDLE_EX)m_hHvHandle, CONN_TYPE_RECORD, &dwStatus) == S_OK )
    {
        if (dwStatus == CONN_STATUS_NORMAL
                || dwStatus == CONN_STATUS_RECVDONE)
        {
            m_iConnectStatus = 0;
        }
        else if (dwStatus == CONN_STATUS_RECONN)
        {
            m_iConnectStatus = 1;
        }
        else
        {
            m_iConnectStatus = 1;
        }
    }
    else
    {
        m_iConnectStatus = 1;
    }
    return m_iConnectStatus;
}

std::string Camera6467::GetCameraIP()
{
    return this->m_strIP;
}

void Camera6467::ReadConfig()
{
    QString qstrCurrentPath = QDir::currentPath();
    QString qstrFilePath = qstrCurrentPath+"/XLW_Config.ini";
    QSettings App_cfg(qstrFilePath,QSettings::IniFormat );
    m_qstrBackupPath = App_cfg.value("ApplicationInfo/BackupResultSavePath",qstrCurrentPath.append("/BackupResult/") ).toString();
    m_qstrRootPath = App_cfg.value("ApplicationInfo/ResultSavePath",qstrCurrentPath.append("/Result/") ).toString();
    m_iCompressQuality = App_cfg.value("ApplicationInfo/CompressQuality",-1 ).toInt();
    m_qstrTextCodeName = App_cfg.value("ApplicationInfo/CodeName","GBK" ).toString();

    qDebug()<<"BackupResultSavePath:"<<m_qstrBackupPath;
    qDebug()<<"ResultSavePath:"<<m_qstrRootPath;
    qDebug()<<"CompressQuality:"<<m_iCompressQuality;
    qDebug()<<"CodeName"<<m_qstrTextCodeName;
}

bool Camera6467::WriteLog(const char* chlog)
{
    if(!m_bLogEnable || NULL == chlog)
        return false;

    QDir tempDir;
    QString qstrCurrentPath = QDir::currentPath();
    qstrCurrentPath.append("/SNW_log");
    qstrCurrentPath.append(QString("/%1").arg(QString(m_strIP.c_str())) );
    if(!tempDir.exists(qstrCurrentPath))
    {
        tempDir.mkpath(qstrCurrentPath);
    }
    QDate currentDate = QDate::currentDate();
    QTime currentTime = QTime::currentTime();
    QString qstrFileName = QString(qstrCurrentPath+"/Camera_log_%1.log").arg(currentDate.toString("yyyy-MM-dd"));

    QFile logFile(qstrFileName);
    if(!logFile.open(QIODevice::Append | QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug()<<"open log File failed";
        return false;
    }
    QString qstrLog = QString("%1 %2: %3").arg(currentDate.toString("yyyy-MM-dd")).arg(currentTime.toString("hh:mm:ss zzz")).arg(QString(chlog));

    QTextStream qStreamLog(&logFile);
    qStreamLog<<qstrLog<<endl;

    logFile.close();
    return true;
}

bool Camera6467::WriteLog(const QString &qstrLogBuff)
{
    if(qstrLogBuff.isEmpty())
        return false;
    QDir tempDir;
    QString qstrCurrentPath = QDir::currentPath();
    qstrCurrentPath.append("//SNW_log");
    qstrCurrentPath.append(QString("/%1").arg(QString(m_strIP.c_str())) );
    if(!tempDir.exists(qstrCurrentPath))
    {
        tempDir.mkdir(qstrCurrentPath);
    }
    QDate currentDate = QDate::currentDate();
    QTime currentTime = QTime::currentTime();
    QString qstrFileName = QString(qstrCurrentPath+"//Camera_log_%1.log").arg(currentDate.toString("yyyy-MM-dd"));

    QFile logFile(qstrFileName);
    if(!logFile.open(QIODevice::Append | QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug()<<"open log File failed";
        return false;
    }
    QString qstrLog = QString("%1 %2: %3").arg(currentDate.toString("yyyy-MM-dd")).arg(currentTime.toString("hh:mm:ss zzz")).arg(qstrLogBuff);

    QTextStream qStreamLog(&logFile);
    qStreamLog<<qstrLog<<endl;

    logFile.close();
    return true;
}

bool Camera6467::TakeCapture()
{
    if (NULL == m_hHvHandle)
        return false;

    bool bRet = true;
    char chRetBuf[1024] = {0};
    int nRetLen = 0;

    if (HVAPI_ExecCmdEx(m_hHvHandle, "SoftTriggerCapture", chRetBuf, 1024, &nRetLen) != S_OK)
    {
        bRet = false;
        char chCaptureLog3[MAX_PATH] = {0};
        sprintf(chCaptureLog3, "Camera: %s SoftTriggerCapture success", m_strIP.c_str());
        WriteLog(chCaptureLog3);
    }
    else
    {
        char chCaptureLog4[MAX_PATH] = {0};
        sprintf(chCaptureLog4, "Camera: %s SoftTriggerCapture failed", m_strIP.c_str());
        WriteLog(chCaptureLog4);
    }
    return bRet;
}

bool Camera6467::SynTime()
{
    if (NULL == m_hHvHandle || m_dwConnectStatus != 0)
        return false;
    if (!m_bSynTime)
    {
        return false;
    }
    WriteLog("SynTime begin");
    QDate currentDate = QDate::currentDate();
    QTime currentTime = QTime::currentTime();
    char chTemp[256]={ 0 };
    sprintf(chTemp, "SetTime,Date[%d.%02d.%02d],Time[%02d:%02d:%02d]",
            currentDate.year(), currentDate.month(), currentDate.day(),
            currentTime.hour(), currentTime.minute(), currentTime.second());
    WriteLog(chTemp);
    char szRetBuf[1024] = {0};
    int nRetLen = 0;
    if (m_hHvHandle != NULL)
    {
        try
        {
            if (HVAPI_ExecCmdEx(m_hHvHandle, chTemp, szRetBuf, 1024, &nRetLen) != S_OK)
            {
                char chSynTimeLogBuf1[MAX_PATH] = {0};
                sprintf(chSynTimeLogBuf1, "Camera: %s SynTime failed", m_strIP.c_str());
                WriteLog(chSynTimeLogBuf1);
                return false;
            }
            else
            {
                char chSynTimeLogBuf2[MAX_PATH] = {0};
                sprintf(chSynTimeLogBuf2, "Camera: %s SynTime success", m_strIP.c_str());
                WriteLog(chSynTimeLogBuf2);
            }
        } catch (...)
        {
            char chSynTimeLogBuf3[MAX_PATH] = {0};
            sprintf(chSynTimeLogBuf3, "Camera: %s SynTime take exception", m_strIP.c_str());
            WriteLog(chSynTimeLogBuf3);
        }
    }
    WriteLog("SynTime end");
    return 0;
}

void Camera6467::AnalysisAppendInfo(CameraResult* CamResult)
{
    QDateTime qTime;
    qTime.setMSecsSinceEpoch(qint64(CamResult->dw64TimeMS));
    QString qstrTime = qTime.toString("yyyy-MM-dd hh:mm:ss.zzz");
    qDebug()<<qstrTime;
    WriteLog(qstrTime);
    sprintf(CamResult->chPlateTime, "%s", qstrTime.toLocal8Bit().data());

    sprintf(CamResult->chDeviceID, "%s", m_chDeviceID);
    sprintf(CamResult->chDeviceIp, "%s", m_strIP.c_str());
    sprintf(CamResult->chLaneID, "%s", m_chLaneID);
    sprintf(CamResult->chSignStationID, "%s", m_chStationID);
    CamResult->iDirection = m_iDirection;

    static int iSerialNum =0;
    iSerialNum = iSerialNum>255 ? 0 : iSerialNum;


    QString qstrStationID = GetStationID();
    QString qstrDeviceID = GetDeviceID();
    QString qstrListNo;
    qstrListNo.sprintf("%s%s%s%s%03d",qstrStationID.toStdString().c_str(), qstrDeviceID.toStdString().c_str(), qTime.toString("yyyyMMdd").toStdString().c_str(), qTime.toString("hhmmss").toStdString().c_str(), iSerialNum);
    sprintf(CamResult->chListNo, "%s", qstrListNo.toStdString().c_str());

    CamResult->iResultNo = iSerialNum++;

    qDebug()<<"ListNo"<<qstrListNo;

    CamResult->iPlateTypeNo = 255;

    QString qstrAppendInfo, qstrPlateNo;
    if(m_qstrTextCodeName.contains("UTF"))
    {
        qstrAppendInfo = QString::fromLocal8Bit(CamResult->pcAppendInfo);
        qstrPlateNo = QString::fromLocal8Bit(CamResult->chPlateNO);
    }
    else if(m_qstrTextCodeName.contains("GB"))
    {
        qstrAppendInfo = QString(CamResult->pcAppendInfo);
        qstrPlateNo = QString(CamResult->chPlateNO);
    }


    WriteLog(qstrAppendInfo);

    if(!qstrPlateNo.contains("无"))
    {
        char chTempPlateNo[32] = {0};
        char *chPlate = CamResult->chPlateNO;
        sprintf(chTempPlateNo, "%s", chPlate+3);
        sprintf(CamResult->chPlateNO, "%s", chTempPlateNo);
        QString tempInfo = QString("the final plateNO is : %1").arg(CamResult->chPlateNO);
        qDebug()<<tempInfo;
        WriteLog(tempInfo);
    }

    if(qstrPlateNo.contains("蓝"))
    {
        qDebug()<<"blue";
        WriteLog("blue");
        CamResult->iPlateColor = 0;
    }
    else if(qstrPlateNo.contains("黄"))
    {
        qDebug()<<"yellow";
        WriteLog("yellow");
        CamResult->iPlateColor = 1;
    }
    else if(qstrPlateNo.contains("黑"))
    {
        qDebug()<<"black";
        WriteLog("black");
        CamResult->iPlateColor = 2;
    }
    else if(qstrPlateNo.contains("白"))
    {
        qDebug()<<"white";
        WriteLog("white");
        CamResult->iPlateColor = 3;
    }
    else
    {
        qDebug()<<"other";
        WriteLog("other");
        CamResult->iPlateColor = 255;
    }

    if(qstrAppendInfo.contains("车辆类型:小"))
    {
        qDebug()<<"Car type: Small";
        WriteLog("Car type: Small");
        CamResult->iVehTypeNo = 1;
    }
    else if(qstrAppendInfo.contains("车辆类型:中"))
    {
        qDebug()<<"Car type: Middle";
        WriteLog("Car type: Middle");
        CamResult->iVehTypeNo = 2;
    }
    else if(qstrAppendInfo.contains("车辆类型:大"))
    {
        qDebug()<<"Car type: Big";
        WriteLog("Car type: Big");
        CamResult->iVehTypeNo = 3;
    }
    else if(qstrAppendInfo.contains("客车"))
    {
        qDebug()<<"Car type: passenger";
        WriteLog("Car type: passenger");
        CamResult->iVehTypeNo = 1;
    }
    else if(qstrAppendInfo.contains("货车"))
    {
        qDebug()<<"Car type: freight";
        WriteLog("Car type: freight");
        CamResult->iVehTypeNo = 2;
    }
    else if(qstrAppendInfo.contains("牵引车"))
    {
        qDebug()<<"Car type: tractor";
        WriteLog("Car type: tractor");
        CamResult->iVehTypeNo = 3;
    }
    else if(qstrAppendInfo.contains("专项作业车"))
    {
        qDebug()<<"Car type: special motor vehicle";
        WriteLog("Car type: special motor vehicle");
        CamResult->iVehTypeNo = 4;
    }
    else if(qstrAppendInfo.contains("电车"))
    {
        qDebug()<<"Car type: electric car";
        WriteLog("Car type: electric car");
        CamResult->iVehTypeNo = 5;
    }
    else if(qstrAppendInfo.contains("摩托车"))
    {
        qDebug()<<"Car type: moto";
        WriteLog("Car type: moto");
        CamResult->iVehTypeNo = 6;
    }
    else if(qstrAppendInfo.contains("三轮汽车"))
    {
        qDebug()<<"Car type: three-wheeled automobile";
        WriteLog("Car type: three-wheeled automobile");
        CamResult->iVehTypeNo = 7;
    }
    else if(qstrAppendInfo.contains("拖拉机"))
    {
        qDebug()<<"Car type: tractor";
        WriteLog("Car type: tractor");
        CamResult->iVehTypeNo = 8;
    }
    else if(qstrAppendInfo.contains("机械"))
    {
        qDebug()<<"Car type: Machinery vehicles";
        WriteLog("Car type: Machinery vehicles");
        CamResult->iVehTypeNo = 9;
    }
    else if(qstrAppendInfo.contains("全挂车"))
    {
        qDebug()<<"Car type:  full trailer";
        WriteLog("Car type:  full trailer");
        CamResult->iVehTypeNo = 10;
    }
    else if(qstrAppendInfo.contains("半挂车"))
    {
        qDebug()<<"Car type: half trailer";
        WriteLog("Car type:  half trailer");
        CamResult->iVehTypeNo = 11;
    }
    else   //unknow type
    {
        qDebug()<<"Car type: unknow";
        WriteLog("Car type: unknow");
        CamResult->iVehTypeNo = 255;
    }

    if(qstrAppendInfo.contains("车身颜色:白"))
    {
        qDebug()<<"Car Color white";
        WriteLog("Car Color white");
        CamResult->iVehBodyColorNo = 1;
    }
    else if(qstrAppendInfo.contains("车身颜色:灰"))
    {
        qDebug()<<"Car Color gray";
        WriteLog("Car Color gray");
        CamResult->iVehBodyColorNo = 2;
    }
    else if(qstrAppendInfo.contains("车身颜色:黄"))
    {
        qDebug()<<"Car Color yellow";
        WriteLog("Car Color yellow");
        CamResult->iVehBodyColorNo = 3;
    }
    else if(qstrAppendInfo.contains("车身颜色:粉"))
    {
        qDebug()<<"Car Color pink";
        WriteLog("Car Color pink");
        CamResult->iVehBodyColorNo = 4;
    }
    else if(qstrAppendInfo.contains("车身颜色:红"))
    {
        qDebug()<<"Car Color: red";
        WriteLog("Car Color: red");
        CamResult->iVehBodyColorNo = 5;
    }
    else if(qstrAppendInfo.contains("车身颜色:紫"))
    {
        qDebug()<<"Car Color: red";
        WriteLog("Car Color: red");
        CamResult->iVehBodyColorNo = 6;
    }
    else if(qstrAppendInfo.contains("车身颜色:绿"))
    {
        qDebug()<<"Car Color: purple";
        WriteLog("Car Color: purple");
        CamResult->iVehBodyColorNo = 7;
    }
    else if(qstrAppendInfo.contains("车身颜色:蓝"))
    {
        qDebug()<<"Car Color: blue";
        WriteLog("Car Color: blue");
        CamResult->iVehBodyColorNo = 8;
    }
    else if(qstrAppendInfo.contains("车身颜色:棕"))
    {
        qDebug()<<"Car Color: brown";
        WriteLog("Car Color: brown");
        CamResult->iVehBodyColorNo = 9;
    }
    else if(qstrAppendInfo.contains("车身颜色:黑"))
    {
        qDebug()<<"Car Color: black";
        WriteLog("Car Color: black");
        CamResult->iVehBodyColorNo = 10;
    }
    else
    {
        qDebug()<<"Car Color: unknow";
        WriteLog("Car Color: unknow");
        CamResult->iVehBodyColorNo = 255;
    }
    CamResult->iVehBodyDeepNo = 255;

    int pos = -1, pos1=-1;
    pos = qstrAppendInfo.indexOf("测速");
    if(pos != -1)
    {
        QRegExp rx("(\\d+)");
        if(rx.indexIn(qstrAppendInfo, pos) != -1)
        {
            CamResult->iSpeed = rx.cap(1).toInt();
        }
    }
    QString qstrCarSpeed = QString("Car speed: %1 km/h").arg(CamResult->iSpeed);
    qDebug()<<qstrCarSpeed;
    WriteLog(qstrCarSpeed);

    pos1 = qstrAppendInfo.indexOf("车道");
    if(pos1 != -1)
    {
        QRegExp rx("(\\d+)");
        if(rx.indexIn(qstrAppendInfo, pos1) != -1)
        {
            CamResult->iLaneNo = rx.cap(1).toInt();
            //sprintf(CamResult->chLaneID, "%d", CamResult->iLaneNo);
        }
    }
    sprintf(CamResult->chLaneID, "%d", CamResult->iLaneNo);
    QString qstrLaneNo = QString("Lane NO: %1 ").arg(CamResult->chLaneID);
    qDebug()<<qstrLaneNo;
    WriteLog(qstrLaneNo);
}

bool Camera6467::IsFirstConnect()
{
    return m_bFirstConnect;
}

CameraInfo &Camera6467::GetCameraInfo()
{
    //    CameraInfo TempCamInfo;
    //    strcpy(TempCamInfo.chIP,this->m_strIP.c_str());
    //    strcpy(TempCamInfo.chDeviceID, this->m_chDeviceID);
    //    strcpy(TempCamInfo.chLaneID, this->m_chLaneID);
    //    strcpy(TempCamInfo.chStationID, this->m_chDeviceID);
    //    TempCamInfo.fUnlicensedPlateRatio = 0.0;
    //    TempCamInfo.iConnectStatus = GetCamStatus();  //Connect :  0 ; DisConnect : 1
    //    TempCamInfo.iDirection = 0;
    //    TempCamInfo.bIsInit = false;
    //    TempCamInfo.bLogEnable = m_bLogEnable;
    //    TempCamInfo.bSynTimeEnable = m_bSynTime;

    QMutexLocker Locker(&m_ResultMutex);

    strcpy(m_CamInfo.chIP,this->m_strIP.c_str());
    strcpy(m_CamInfo.chDeviceID, this->m_chDeviceID);
    strcpy(m_CamInfo.chLaneID, this->m_chLaneID);
    strcpy(m_CamInfo.chStationID, this->m_chDeviceID);
    m_CamInfo.iConnectStatus = GetCamStatus();  //Connect :  0 ; DisConnect : 1

    return m_CamInfo;
}

void Camera6467::ReadHistoryInfo()
{
    QString qstrCurrentPath = QDir::currentPath();
    QString qstrFilePath = qstrCurrentPath+"/XLW_SaveModelInfo.ini";
    QSettings App_cfg(qstrFilePath,QSettings::IniFormat );

    QString qstrBeginTime, qstrEndTime;
    QString qstrParameterGroupName = QString("%1").arg(m_strIP.c_str());
    App_cfg.beginGroup(qstrParameterGroupName);

    QDateTime qdtime;
    qdtime = QDateTime::currentDateTime();
    QString qstrSaveTime = qdtime.toString("yyyy.MM.dd_hh");

    qstrBeginTime = App_cfg.value("BeginTime",qstrSaveTime).toString();
    qstrEndTime = App_cfg.value("EndTime","").toString();
    sprintf(m_SaveModelInfo.chBeginTime, "%s", qstrBeginTime.toStdString().c_str());
    sprintf(m_SaveModelInfo.chEndTime, "%s", qstrEndTime.toStdString().c_str());
    m_SaveModelInfo.iSafeModeEnable = App_cfg.value("SaveModeEnable",0).toInt();
    m_SaveModelInfo.iIndex = App_cfg.value("Index", 0).toInt();
    m_SaveModelInfo.iDataType = App_cfg.value("DataType", 0).toInt();

    App_cfg.endGroup();
}

void Camera6467::WriteHistoryInfo(SaveModeInfo &SaveInfo)
{
    QString qstrCurrentPath = QDir::currentPath();
    QString qstrFilePath = qstrCurrentPath+"/XLW_SaveModelInfo.ini";
    QSettings App_cfg(qstrFilePath,QSettings::IniFormat );

    QString qstrParameterGroupName = QString("%1").arg(m_strIP.c_str());
    App_cfg.beginGroup(qstrParameterGroupName);

    QString qstrBeginTime(SaveInfo.chBeginTime);
    QString qstrEndTime(SaveInfo.chEndTime);

    App_cfg.setValue("BeginTime", qstrBeginTime);
    App_cfg.setValue("EndTime", qstrEndTime);
    App_cfg.setValue("Index", SaveInfo.iIndex);
    App_cfg.setValue("DataType", SaveInfo.iDataType);
    App_cfg.setValue("SaveModeEnable", SaveInfo.iSafeModeEnable);

    App_cfg.endGroup();
}

//void Camera6467::SetList(Result_lister* ResultList)
//{
//    g_ResultList = ResultList;
//}

QString Camera6467::GetStationID()
{
    return QString::fromLocal8Bit(m_chStationID);
}

QString Camera6467::GetDeviceID()
{
    return QString::fromLocal8Bit(m_chDeviceID);
}

QString Camera6467::GetLaneID()
{
    return QString::fromLocal8Bit(m_chLaneID);
}

bool Camera6467::SaveImgToDisk(QString chImgPath, BYTE *pImgData, DWORD dwImgSize)
{
    WriteLog("begin SaveImgToDisk");
    if(NULL == pImgData)
    {
        WriteLog("end1 SaveImgToDisk");
        return false;
    }
    bool bRet = false;
    size_t iWritedSpecialSize = 0;
    QDir tempDir;
    int iPosition = chImgPath.lastIndexOf("/");
    if (tempDir.mkpath(chImgPath.mid(0, iPosition)))
    {
        FILE* fp = fopen(chImgPath.toStdString().c_str(), "wb");
        if(fp)
        {
            //iWritedSpecialSize = fwrite(pImgData, dwImgSize , 1, fp);
            iWritedSpecialSize = fwrite(pImgData, 1 , dwImgSize, fp);
            fclose(fp);
            fp = NULL;
            bRet = true;
        }
        if (iWritedSpecialSize == dwImgSize)
        {
            char chLogBuff[MAX_PATH] = {0};
            sprintf(chLogBuff, "%s save success", chImgPath.toStdString().c_str());
            WriteLog(chLogBuff);
        }
    }
    else
    {
        char chLogBuff[MAX_PATH] = {0};
        sprintf(chLogBuff, "%s save failed", chImgPath.toStdString().c_str());
        WriteLog(chLogBuff);
        bRet = false;
    }
    WriteLog("end SaveImgToDisk");
    return bRet;
}

//bool Camera6467::SaveImgToDiskByQt(CameraIMG &camImageStruct)
//{
//    QImage qBigImg;
//    bool bLoad = qBigImg.loadFromData(camImageStruct.pbImgData, camImageStruct.dwImgSize);
//    //qDebug()<<QString("Image Load")<<bLoad;
//    QString qstrSavePath = QString::fromLocal8Bit(camImageStruct.chSavePath);
//    bool bret = false;
//    if(!qstrSavePath.isEmpty())
//    {
//        QString qstrTempPath = qstrSavePath.mid(0, qstrSavePath.lastIndexOf("/"));
//        QDir tempDir;
//        tempDir.mkpath(qstrTempPath);
//        //qDebug()<<qstrTempPath;
//        bret = qBigImg.save(qstrSavePath,"JPEG", m_iCompressQuality);
//    }
//    return bret;
//}

void Camera6467::SetImgPath(CameraResult *camResult)
{
    QDateTime qdTime;
    qdTime.setMSecsSinceEpoch(qint64(camResult->dw64TimeMS));
    QString qstrImgPath = QString("%1/%2/%3/%4/%5/%6/%7.jpg")
            .arg(QString(m_chStationID))
            .arg(QString(m_chDeviceID))
            .arg(qdTime.toString("yyyy"))
            .arg(qdTime.toString("MM"))
            .arg(qdTime.toString("dd"))
            .arg(qdTime.toString("hh"))
            .arg(QString(camResult->chListNo));

    QString qstrFullImgPath = QString("%1/Result/%2").arg(m_qstrRootPath).arg(qstrImgPath);
    //QString qstrCloseUpImgPath = QString("%1/CloseUp/%2").arg(m_qstrRootPath).arg(qstrImgPath);

    sprintf(camResult->CIMG_BestCapture.chSavePath,"%s",qstrFullImgPath.toLocal8Bit().data());
    sprintf(camResult->CIMG_BeginCapture.chSavePath,"%s",qstrFullImgPath.toLocal8Bit().data());
    sprintf(camResult->CIMG_LastCapture.chSavePath,"%s",qstrFullImgPath.toLocal8Bit().data());
    sprintf(camResult->CIMG_BestSnapshot.chSavePath,"%s",qstrFullImgPath.toLocal8Bit().data());
    sprintf(camResult->CIMG_LastSnapshot.chSavePath,"%s",qstrFullImgPath.toLocal8Bit().data());
    sprintf(camResult->CIMG_PlateImage.chSavePath,"%s",qstrFullImgPath.toLocal8Bit().data());
    sprintf(camResult->CIMG_BinImage.chSavePath,"%s",qstrFullImgPath.toLocal8Bit().data());


    //     QString qstrPlayteImg = QString("%1/PlateImg/%2_plate.jpg").arg(m_qstrRootPath).arg(QString(camResult->chListNo));
    //     sprintf(camResult->CIMG_PlateImage.chSavePath,"%s",qstrPlayteImg.toLocal8Bit().data());

}

int Camera6467::RecordInfoBegin(DWORD dwCarID)
{
    QString qstrlogInfo = QString("RecordInfoBegin -begin- dwCarID = %1").arg(dwCarID);
    qDebug() << qstrlogInfo;
    WriteLog(qstrlogInfo);
    m_bResultComplete = false;

    if(!m_CameraResult)
    {
        m_CameraResult = new CameraResult();
    }

    if(NULL == m_CameraResult)
    {
        WriteLog("new CamerResult Failed,return.");
        return 0;
    }
    memset(m_CameraResult->chDeviceIp, 0, sizeof(m_CameraResult->chDeviceIp));
    sprintf(m_CameraResult->chDeviceIp, "%s", m_strIP.c_str());
    m_CameraResult->dwCarID = dwCarID;
    QString qstrlogInfo2 = QString("RecordInfoBegin -end- dwCarID = %1").arg(dwCarID);
    qDebug()<<qstrlogInfo2;
    WriteLog(qstrlogInfo2);
    return 0;
}

int Camera6467::RecordInfoEnd(DWORD dwCarID)
{
    QString qstrlogInfo = QString("RecordInfoEnd  -begin- dwCarID = %1").arg(dwCarID);
    qDebug() << qstrlogInfo;
    WriteLog(qstrlogInfo);
    if(m_CameraResult == NULL)
    {
        QString qstrlogInfo = QString("RecordInfoEnd - the Reuslt is NULL -end1- dwCarID = %1").arg(dwCarID);
        qDebug() << qstrlogInfo;
        WriteLog(qstrlogInfo);
        return 0;
    }
    if(m_CameraResult->dwCarID == dwCarID)
    {
        m_bResultComplete = true;

        if(NULL != m_CameraResult)
        {
            ///TODO
        }
    }
    else
    {
        WriteLog("The carID is different.");
    }
    QString qstrlogInfo2 = QString("RecordInfoEnd  -end- dwCarID = %1").arg(dwCarID);
    qDebug()<<qstrlogInfo2;
    WriteLog(qstrlogInfo2);
    return 0;
}

int Camera6467::RecordInfoPlate(DWORD dwCarID, LPCSTR pcPlateNo, LPCSTR pcAppendInfo, DWORD /*dwRecordType*/, DWORD64 dw64TimeMS)
{
    m_bResultComplete = false;

    if (NULL == m_CameraResult)
    {
        return -1;
    }
    QString qstrlogInfo = QString("RecordInfoPlate -begin- dwCarID = %1").arg(dwCarID);
    qDebug() << qstrlogInfo;
    WriteLog(qstrlogInfo);
    if (m_CameraResult->dwCarID == dwCarID)
    {
        m_CameraResult->dw64TimeMS = dw64TimeMS;
        sprintf(m_CameraResult->chPlateNO, "%s",pcPlateNo);
        //sprintf(m_CameraResult->chPlateNO, "%s",pcPlateNo+3);
        memset(m_CameraResult->pcAppendInfo, 0, sizeof(m_CameraResult->pcAppendInfo));
        HVAPIUTILS_ParsePlateXmlStringEx(pcAppendInfo, m_CameraResult->pcAppendInfo, 2048);

        QString plateNo = QString::fromLocal8Bit(m_CameraResult->chPlateNO);
        qDebug()<<plateNo;
        WriteLog(plateNo);

        if(m_iTotalPlateCount <= 5)
        {
            m_iTotalPlateCount++;
            if(plateNo.contains("无"))
            {
                m_iUnlisensePlateCount++;
            }
        }
        else
        {
            QMutexLocker Locker(&m_ResultMutex);
            float fRate = (float)m_iUnlisensePlateCount/(float)m_iTotalPlateCount;

            //Reset the Rate
            m_iTotalPlateCount = 1;
            m_iUnlisensePlateCount = 0;
            if(plateNo.contains("无"))
            {
                m_iUnlisensePlateCount++;
            }

            m_CamInfo.fUnlicensedPlateRatio = fRate;

            qstrlogInfo.clear();
            qstrlogInfo = QString("current Unlicense plate rate = %1").arg(QString::number(fRate, 'f', 2));
            WriteLog(qstrlogInfo);
        }

        QString qstrPlateTime = QString("the plate time is %1").arg(dw64TimeMS);
        qDebug()<<qstrPlateTime;
        WriteLog(qstrPlateTime);

        AnalysisAppendInfo(m_CameraResult);
    }
    QString qstrlogInfo2 = QString("RecordInfoPlate -end- dwCarID = %1").arg(dwCarID);
    qDebug()<<qstrlogInfo2;
    WriteLog(qstrlogInfo2);
    return 0;
}

int Camera6467::RecordInfoSmallImage(DWORD dwCarID, WORD wWidth, WORD wHeight, PBYTE pbPicData, DWORD /*dwImgDataLen*/, DWORD /*dwRecordType*/, DWORD64 /*dw64TimeMS*/)
{

    m_bResultComplete = false;
    if (NULL == m_CameraResult)
    {
        return -1;
    }
    QString qstrlogInfo = QString("RecordInfoSmallImage  -begin- dwCarID = %1").arg(dwCarID);
    qDebug() << qstrlogInfo;
    WriteLog(qstrlogInfo);
    int iBuffLen = 100 * 1024;
    if (m_CameraResult->dwCarID == dwCarID)
    {
        if (NULL != m_CameraResult->CIMG_PlateImage.pbImgData)
        {
            delete[] m_CameraResult->CIMG_PlateImage.pbImgData;
            m_CameraResult->CIMG_PlateImage.pbImgData = NULL;
        }
        m_CameraResult->CIMG_PlateImage.pbImgData = new BYTE[iBuffLen];
        if (m_CameraResult->CIMG_PlateImage.pbImgData != NULL)
        {
            memset(m_CameraResult->CIMG_PlateImage.pbImgData, 0 ,102400);
            HRESULT Hr = HVAPIUTILS_SmallImageToBitmapEx(pbPicData,
                                                         wWidth,
                                                         wHeight,
                                                         m_CameraResult->CIMG_PlateImage.pbImgData,
                                                         &iBuffLen);
            if ( Hr == S_OK)
            {
                m_CameraResult->CIMG_PlateImage.wImgWidth = wWidth;
                m_CameraResult->CIMG_PlateImage.wImgHeight = wHeight;
                m_CameraResult->CIMG_PlateImage.dwImgSize = iBuffLen;
            }
        }
    }
    QString qstrlogInfo2 = QString("RecordInfoSmallImage  -end- dwCarID = %1").arg(dwCarID);
    qDebug()<<qstrlogInfo2;
    WriteLog(qstrlogInfo2);
    return 0;
}

int Camera6467::RecordInfoBinaryImage(DWORD dwCarID, WORD wWidth, WORD wHeight, PBYTE pbPicData, DWORD dwImgDataLen, DWORD /*dwRecordType*/, DWORD64 /*dw64TimeMS*/)
{
    m_bResultComplete = false;

    if (NULL == m_CameraResult)
    {
        return -1;
    }
    QString qstrlogInfo = QString("RecordInfoBinaryImage -begin- dwCarID = %1").arg(dwCarID);
    qDebug() << qstrlogInfo;
    WriteLog(qstrlogInfo);

    if (m_CameraResult->dwCarID == dwCarID)
    {
        CopyDataToIMG(m_CameraResult->CIMG_BinImage, pbPicData, wWidth, wHeight, dwImgDataLen, 0);
    }
    QString qstrlogInfo2 = QString("RecordInfoBinaryImage -end- dwCarID = %1").arg(dwCarID);
    qDebug()<<qstrlogInfo2;
    WriteLog(qstrlogInfo2);
    return 0;
}


int Camera6467::RecordInfoBigImage(DWORD dwCarID,
                                   WORD  wImgType,
                                   WORD  wWidth,
                                   WORD  wHeight,
                                   PBYTE pbPicData,
                                   DWORD dwImgDataLen,
                                   DWORD /*dwRecordType*/,
                                   DWORD64 /*dw64TimeMS*/)
{
    m_bResultComplete = false;

    if (NULL == m_CameraResult)
    {
        return -1;
    }
    QString qstrlogInfo = QString("RecordInfoBigImage -begin- dwCarID = %1").arg(dwCarID);
    qDebug() << qstrlogInfo;
    WriteLog(qstrlogInfo);
    if (m_CameraResult->dwCarID == dwCarID)
    {
        if (wImgType == RECORD_BIGIMG_BEST_SNAPSHOT)
        {
            WriteLog("RecordInfoBigImage BEST_SNAPSHO  ");

            CopyDataToIMG(m_CameraResult->CIMG_BestSnapshot, pbPicData, wWidth, wHeight, dwImgDataLen, wImgType);
        }
        else if (wImgType == RECORD_BIGIMG_LAST_SNAPSHOT)
        {
            WriteLog("RecordInfoBigImage LAST_SNAPSHOT  ");

            CopyDataToIMG(m_CameraResult->CIMG_LastSnapshot, pbPicData, wWidth, wHeight, dwImgDataLen, wImgType);
        }
        else if (wImgType == RECORD_BIGIMG_BEGIN_CAPTURE)
        {
            WriteLog("RecordInfoBigImage BEGIN_CAPTURE  ");

            CopyDataToIMG(m_CameraResult->CIMG_BeginCapture, pbPicData, wWidth, wHeight, dwImgDataLen, wImgType);
        }
        else if (wImgType == RECORD_BIGIMG_BEST_CAPTURE)
        {
            WriteLog("RecordInfoBigImage BEST_CAPTURE  ");

            CopyDataToIMG(m_CameraResult->CIMG_BestCapture, pbPicData, wWidth, wHeight, dwImgDataLen, wImgType);
        }
        else if (wImgType == RECORD_BIGIMG_LAST_CAPTURE)
        {
            WriteLog("RecordInfoBigImage LAST_CAPTURE  ");

            CopyDataToIMG(m_CameraResult->CIMG_LastCapture, pbPicData, wWidth, wHeight, dwImgDataLen, wImgType);
        }
        else
        {
            WriteLog("RecordInfoBigImage other Img, put it to  LAST_CAPTURE .");
            CopyDataToIMG(m_CameraResult->CIMG_LastCapture, pbPicData, wWidth, wHeight, dwImgDataLen, wImgType);
        }
    }
    QString qstrlogInfo2 = QString("RecordInfoBigImage -end- dwCarID = %1").arg(dwCarID);
    qDebug()<<qstrlogInfo2;
    WriteLog(qstrlogInfo2);

    return 0;
}


