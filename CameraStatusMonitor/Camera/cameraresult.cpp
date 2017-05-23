#include"Camera/cameraresult.h"
#include <QString>
#include <QDir>
#include <QSettings>

CameraIMG::CameraIMG()
{
    wImgWidth = 0;
    wImgHeight = 0;
    pbImgData = NULL;
    dwImgSize = 0;

    for (unsigned int i = 0; i< sizeof(chSavePath); i++)
    {
        chSavePath[i] = 0;
    }
}

CameraIMG::CameraIMG( const CameraIMG& CaIMG )
{
    wImgWidth = CaIMG.wImgWidth;
    wImgHeight = CaIMG.wImgHeight;
    dwImgSize = CaIMG.dwImgSize;
    wImgType = CaIMG.wImgType;
    for (unsigned int i = 0; i< sizeof(chSavePath); i++)
    {
        chSavePath[i] = CaIMG.chSavePath[i];
    }
    if (NULL != CaIMG.pbImgData)
    {
        pbImgData = new BYTE[CaIMG.dwImgSize];
        for (DWORD j = 0; j< dwImgSize; j++)
        {
            pbImgData[j] = CaIMG.pbImgData[j];
        }
    }
    else
    {
        pbImgData = NULL;
    }
}

CameraIMG::~CameraIMG()
{
    wImgWidth = 0;
    wImgHeight = 0;
    if (NULL != pbImgData)
    {
        delete[] pbImgData;
        pbImgData = NULL;
    }
    dwImgSize = 0;
    memset(chSavePath, 0, sizeof(chSavePath));
}

CameraIMG& CameraIMG::operator = (const CameraIMG& CaIMG)
{
    if (this != &CaIMG)
    {
        wImgWidth = CaIMG.wImgWidth;
        wImgHeight = CaIMG.wImgHeight;
        dwImgSize = CaIMG.dwImgSize;
        wImgType = CaIMG.wImgType;
        for (unsigned int i = 0; i< sizeof(chSavePath); i++)
        {
            chSavePath[i] = CaIMG.chSavePath[i];
        }
        if (NULL != CaIMG.pbImgData)
        {
            pbImgData = new BYTE[CaIMG.dwImgSize];
            for (DWORD j = 0; j< dwImgSize; j++)
            {
                pbImgData[j] = CaIMG.pbImgData[j];
            }
        }
        else
        {
            pbImgData = NULL;
        }
    }
    return *this;
}

CameraResult::CameraResult()
{
    dwCarID = 0;
    dw64TimeMS =0;
    iLaneNo = 0;
    iPlateColor = 0;
    iVehTypeNo = 0;
    iDirection = 0;
    iAreaNo = 0;
    iVehBodyColorNo = 0;
    iPlateTypeNo = 0;
    iVehBodyDeepNo = 0;
    iDeviceID = 0;
    iRoadNo = 0;
    iSpeed = 0;
    iVehLenth = 0;
    iResultNo =0;

    //pcAppendInfo = NULL;
    for (unsigned int i = 0; i < sizeof(pcAppendInfo); i++)
    {
        pcAppendInfo[i] = 0;
    }

    for (unsigned int i = 0; i < sizeof(chDeviceIp); i++)
    {
        chDeviceIp[i] = 0;
    }

    for (unsigned int i = 0; i < sizeof(chPlateNO); i++)
    {
        chPlateNO[i] = 0;
    }

    for (unsigned int i = 0; i < sizeof(chListNo); i++)
    {
        chListNo[i] = 0;
    }

    for (unsigned int i = 0; i < sizeof(chPlateTime); i++)
    {
        chPlateTime[i] = 0;
    }

    for (unsigned int i = 0; i < sizeof(chSignStationID); i++)
    {
        chSignStationID[i] = 0;
    }

    for (unsigned int i = 0; i < sizeof(chSignStationName); i++)
    {
        chSignStationName[i] = 0;
    }

    for (unsigned int i = 0; i < sizeof(chLaneID); i++)
    {
        chLaneID[i] = 0;
    }

    for (unsigned int i = 0; i < sizeof(chDeviceID); i++)
    {
        chDeviceID[i] = 0;
    }

}

CameraResult::CameraResult( const CameraResult& CaRESULT )
{
    dwCarID = CaRESULT.dwCarID;
    dw64TimeMS = CaRESULT.dw64TimeMS;
    iLaneNo = CaRESULT.iLaneNo;
    iPlateColor = CaRESULT.iPlateColor;
    iVehTypeNo = CaRESULT.iVehTypeNo;
    iDirection = CaRESULT.iDirection;
    iAreaNo = CaRESULT.iAreaNo;
    iVehBodyColorNo = CaRESULT.iVehBodyColorNo;
    iPlateTypeNo = CaRESULT.iPlateTypeNo;
    iVehBodyDeepNo = CaRESULT.iVehBodyDeepNo;
    iDeviceID = CaRESULT.iDeviceID;
    iRoadNo = CaRESULT.iRoadNo;
    iSpeed = CaRESULT.iSpeed;
    iVehLenth = CaRESULT.iVehLenth;
    iResultNo = CaRESULT.iResultNo;

    //pcAppendInfo = NULL;
    for (unsigned int i = 0; i < sizeof(pcAppendInfo); i++)
    {
        pcAppendInfo[i] = 0;
    }

    for (unsigned int i = 0; i < sizeof(chDeviceIp); i++)
    {
        chDeviceIp[i] = CaRESULT.chDeviceIp[i];
    }

    for (unsigned int i = 0; i < sizeof(chPlateNO); i++)
    {
        chPlateNO[i] = CaRESULT.chPlateNO[i];
    }

    for (unsigned int i = 0; i < sizeof(chListNo); i++)
    {
        chListNo[i] = CaRESULT.chListNo[i];
    }

    for (unsigned int i = 0; i < sizeof(chPlateTime); i++)
    {
        chPlateTime[i] = CaRESULT.chPlateTime[i];
    }

    for (unsigned int i = 0; i < sizeof(chSignStationID); i++)
    {
        chSignStationID[i] = CaRESULT.chSignStationID[i];
    }

    for (unsigned int i = 0; i < sizeof(chSignStationName); i++)
    {
        chSignStationName[i] = CaRESULT.chSignStationName[i];
    }

    for (unsigned int i = 0; i < sizeof(chLaneID); i++)
    {
        chLaneID[i] = CaRESULT.chLaneID[i];
    }

    for (unsigned int i = 0; i < sizeof(chDeviceID); i++)
    {
        chDeviceID[i] = CaRESULT.chDeviceID[i];
    }

    CIMG_BestCapture = CaRESULT.CIMG_BestCapture;
    CIMG_BeginCapture = CaRESULT.CIMG_BeginCapture;
    CIMG_LastCapture = CaRESULT.CIMG_LastCapture;
    CIMG_BestSnapshot = CaRESULT.CIMG_BestSnapshot;
    CIMG_LastSnapshot = CaRESULT.CIMG_LastSnapshot;
    CIMG_PlateImage = CaRESULT.CIMG_PlateImage;
    CIMG_BinImage = CaRESULT.CIMG_BinImage;
}

CameraResult::~CameraResult()
{
    dwCarID =0;
    dw64TimeMS =0;
    iLaneNo = 0;
    iVehTypeNo = 0;
    iPlateColor = -1;
    iDirection = 0;
    iVehBodyColorNo = 0;
    iPlateTypeNo = 0;
    iVehBodyDeepNo = 0;
    iDeviceID = 0;
    iRoadNo = 0;
    iSpeed = 0;
    iVehLenth = 0;
    iResultNo =0;

    memset(chDeviceIp, 0, sizeof(chDeviceIp));
    memset(chPlateNO, 0, sizeof(chPlateNO));
    memset(chListNo, 0, sizeof(chListNo));
    memset(chPlateTime, 0, sizeof(chPlateTime));
    memset(chSignStationID, 0, sizeof(chSignStationID));
    memset(chSignStationName, 0, sizeof(chSignStationName));
    memset(pcAppendInfo, 0, sizeof(pcAppendInfo));
    memset(chLaneID, 0, sizeof(chLaneID));
    memset(chDeviceID, 0, sizeof(chDeviceID));

//    if (NULL != pcAppendInfo)
//    {
//        delete[] pcAppendInfo;
//        pcAppendInfo = NULL;
//    }
}

CameraResult& CameraResult::operator=( const CameraResult& CaRESULT )
{
    dwCarID = CaRESULT.dwCarID;
    dw64TimeMS = CaRESULT.dw64TimeMS;
    iLaneNo = CaRESULT.iLaneNo;
    iPlateColor = CaRESULT.iPlateColor;
    iVehTypeNo = CaRESULT.iVehTypeNo;
    iDirection = CaRESULT.iDirection;
    iAreaNo = CaRESULT.iAreaNo;
    iVehBodyColorNo = CaRESULT.iVehBodyColorNo;
    iPlateTypeNo = CaRESULT.iPlateTypeNo;
    iVehBodyDeepNo = CaRESULT.iVehBodyDeepNo;
    iDeviceID = CaRESULT.iDeviceID;
    iRoadNo = CaRESULT.iRoadNo;
    iSpeed = CaRESULT.iSpeed;
    iVehLenth = CaRESULT.iVehLenth;
    iResultNo = CaRESULT.iResultNo;

    //pcAppendInfo = NULL;

    for(unsigned int i =0; i<sizeof(pcAppendInfo); i++)
    {
        pcAppendInfo[i] = 0;
    }

    for (unsigned int i = 0; i < sizeof(chDeviceIp); i++)
    {
        chDeviceIp[i] = CaRESULT.chDeviceIp[i];
    }

    for (unsigned int i = 0; i < sizeof(chPlateNO); i++)
    {
        chPlateNO[i] = CaRESULT.chPlateNO[i];
    }

    for (unsigned int i = 0; i < sizeof(chListNo); i++)
    {
        chListNo[i] = CaRESULT.chListNo[i];
    }

    for (unsigned int i = 0; i< sizeof(chPlateTime); i++ )
    {
        chPlateTime[i] = CaRESULT.chPlateTime[i];
    }

    for (unsigned int i = 0; i < sizeof(chSignStationID); i++)
    {
        chSignStationID[i] = CaRESULT.chSignStationID[i];
    }

    for (unsigned int i = 0; i < sizeof(chSignStationName); i++)
    {
        chSignStationName[i] = CaRESULT.chSignStationName[i];
    }

    for (unsigned int i = 0; i < sizeof(chLaneID); i++)
    {
        chLaneID[i] = CaRESULT.chLaneID[i];
    }

    for (unsigned int i = 0; i < sizeof(chDeviceID); i++)
    {
        chDeviceID[i] = CaRESULT.chDeviceID[i];
    }

    CIMG_BestCapture = CaRESULT.CIMG_BestCapture;
    CIMG_BeginCapture = CaRESULT.CIMG_BeginCapture;
    CIMG_LastCapture = CaRESULT.CIMG_LastCapture;
    CIMG_BestSnapshot = CaRESULT.CIMG_BestSnapshot;
    CIMG_LastSnapshot = CaRESULT.CIMG_LastSnapshot;
    CIMG_PlateImage = CaRESULT.CIMG_PlateImage;
    CIMG_BinImage = CaRESULT.CIMG_BinImage;

    return *this;
}

bool SerializationAsConfigToDisk(const char *chFilePath, const CameraResult &CaResult)
{
    if(NULL == chFilePath)
        return false;

    QSettings App_cfg(chFilePath,QSettings::IniFormat );

    App_cfg.setValue("iDeviceID", CaResult.iDeviceID);
    App_cfg.setValue("iPlateColor", CaResult.iPlateColor);
    App_cfg.setValue("iPlateTypeNo", CaResult.iPlateTypeNo);
    App_cfg.setValue("dw64TimeMS", CaResult.dw64TimeMS);
    App_cfg.setValue("iSpeed", CaResult.iSpeed);
    App_cfg.setValue("iResultNo", CaResult.iResultNo);
    App_cfg.setValue("iVehTypeNo", CaResult.iVehTypeNo);
    App_cfg.setValue("iVehBodyColorNo", CaResult.iVehBodyColorNo);
    App_cfg.setValue("iVehBodyDeepNo", CaResult.iVehBodyDeepNo);
    App_cfg.setValue("iVehLenth", CaResult.iVehLenth);
    App_cfg.setValue("iAreaNo", CaResult.iAreaNo);
    App_cfg.setValue("iRoadNo", CaResult.iRoadNo);
    App_cfg.setValue("iLaneNo", CaResult.iLaneNo);
    App_cfg.setValue("iDirection", CaResult.iDirection);

    App_cfg.setValue("chDeviceIp", QString(CaResult.chDeviceIp));
    App_cfg.setValue("chPlateNO", QString::fromLocal8Bit(CaResult.chPlateNO));
    App_cfg.setValue("chListNo", QString(CaResult.chListNo));
    App_cfg.setValue("chPlateTime", QString(CaResult.chPlateTime));
    App_cfg.setValue("chSignStationID", QString(CaResult.chSignStationID));
    App_cfg.setValue("chSignStationName", QString(CaResult.chSignStationName));
    App_cfg.setValue("pcAppendInfo", QString(CaResult.pcAppendInfo));
    App_cfg.setValue("chDeviceID", QString(CaResult.chDeviceID));
    App_cfg.setValue("chLaneID", QString(CaResult.chLaneID));

    //CIMG_SpecialImage
    App_cfg.setValue("CIMG_SpecialImage/wImgWidth", CaResult.CIMG_BestCapture.wImgWidth);
    App_cfg.setValue("CIMG_SpecialImage/wImgHeight",CaResult.CIMG_BestCapture.wImgHeight);
    App_cfg.setValue("CIMG_SpecialImage/dwImgSize", WORD(CaResult.CIMG_BestCapture.dwImgSize));
    App_cfg.setValue("CIMG_SpecialImage/wImgType", CaResult.CIMG_BestCapture.wImgType);
    App_cfg.setValue("CIMG_SpecialImage/chSavePath", QString::fromLocal8Bit(CaResult.CIMG_BestCapture.chSavePath));
    //CIMG_FullImage
    App_cfg.setValue("CIMG_FullImage/wImgWidth", CaResult.CIMG_LastCapture.wImgWidth);
    App_cfg.setValue("CIMG_FullImage/wImgHeight", CaResult.CIMG_LastCapture.wImgHeight);
    App_cfg.setValue("CIMG_FullImage/dwImgSize", WORD(CaResult.CIMG_LastCapture.dwImgSize));
    App_cfg.setValue("CIMG_FullImage/wImgType", CaResult.CIMG_LastCapture.wImgType);
    App_cfg.setValue("CIMG_FullImage/chSavePath", QString::fromLocal8Bit(CaResult.CIMG_LastCapture.chSavePath));
    //CIMG_PlateImage
    App_cfg.setValue("CIMG_PlateImage/wImgWidth", CaResult.CIMG_PlateImage.wImgWidth);
    App_cfg.setValue("CIMG_PlateImage/wImgHeight", CaResult.CIMG_PlateImage.wImgHeight);
    App_cfg.setValue("CIMG_PlateImage/dwImgSize", WORD(CaResult.CIMG_PlateImage.dwImgSize));
    App_cfg.setValue("CIMG_PlateImage/wImgType", CaResult.CIMG_PlateImage.wImgType);
    App_cfg.setValue("CIMG_PlateImage/chSavePath", QString::fromLocal8Bit(CaResult.CIMG_PlateImage.chSavePath));
    //CIMG_BinImage
    App_cfg.setValue("CIMG_BinImage/wImgWidth", CaResult.CIMG_BinImage.wImgWidth);
    App_cfg.setValue("CIMG_BinImage/wImgHeight", CaResult.CIMG_BinImage.wImgHeight);
    App_cfg.setValue("CIMG_BinImage/dwImgSize", WORD(CaResult.CIMG_BinImage.dwImgSize));
    App_cfg.setValue("CIMG_BinImage/wImgType", CaResult.CIMG_BinImage.wImgType);
    App_cfg.setValue("CIMG_BinImage/chSavePath", QString::fromLocal8Bit(CaResult.CIMG_BinImage.chSavePath));

    return true;
}

bool SerializationAsConfigFromDisk(const char *chFilePath,  CameraResult &CaResult)
{
    if(NULL == chFilePath)
        return false;

    QSettings App_cfg(chFilePath,QSettings::IniFormat );

    CaResult.dwCarID = App_cfg.value("dwCarID").toInt();
    CaResult.iDeviceID = App_cfg.value("iDeviceID").toInt();
    CaResult.iPlateColor = App_cfg.value("iPlateColor").toInt();
    CaResult.iPlateTypeNo = App_cfg.value("iPlateTypeNo").toInt();
    CaResult.dw64TimeMS = App_cfg.value("dw64TimeMS").toInt();
    CaResult.iSpeed = App_cfg.value("iSpeed").toInt();
    CaResult.iResultNo = App_cfg.value("iResultNo").toInt();
    CaResult.iVehTypeNo = App_cfg.value("iVehTypeNo").toInt();
    CaResult.iVehBodyColorNo = App_cfg.value("iVehBodyColorNo").toInt();
    CaResult.iVehBodyDeepNo = App_cfg.value("iVehBodyDeepNo").toInt();
    CaResult.iVehLenth = App_cfg.value("iVehLenth").toInt();
    CaResult.iAreaNo = App_cfg.value("iAreaNo").toInt();
    CaResult.iRoadNo = App_cfg.value("iRoadNo").toInt();
    CaResult.iLaneNo = App_cfg.value("iLaneNo").toInt();
    CaResult.iDirection = App_cfg.value("iDirection").toInt();

    QString qstrTemp = QString("");
    qstrTemp = App_cfg.value("chDeviceIp").toString();
    sprintf(CaResult.chDeviceIp, "%s", qstrTemp.toStdString().data());

    qstrTemp = App_cfg.value("chPlateNO").toString();
    sprintf(CaResult.chPlateNO, "%s", qstrTemp.toLocal8Bit().data());

    qstrTemp = App_cfg.value("chListNo").toString();
    sprintf(CaResult.chListNo, "%s", qstrTemp.toStdString().c_str());

    qstrTemp = App_cfg.value("chPlateTime").toString();
    sprintf(CaResult.chPlateTime, "%s", qstrTemp.toStdString().c_str());

    qstrTemp = App_cfg.value("chSignStationID").toString();
    sprintf(CaResult.chSignStationID, "%s", qstrTemp.toStdString().c_str());

    qstrTemp = App_cfg.value("chSignStationName").toString();
    sprintf(CaResult.chSignStationName, "%s", qstrTemp.toStdString().c_str());

    qstrTemp = App_cfg.value("pcAppendInfo").toString();
    sprintf(CaResult.pcAppendInfo, "%s", qstrTemp.toLocal8Bit().data());

    qstrTemp = App_cfg.value("chDeviceID").toString();
    sprintf(CaResult.chDeviceID, "%s", qstrTemp.toStdString().c_str());

    qstrTemp = App_cfg.value("chLaneID").toString();
    sprintf(CaResult.chLaneID, "%s", qstrTemp.toStdString().c_str());

    CaResult.CIMG_BestCapture.wImgWidth = App_cfg.value("CIMG_SpecialImage/wImgWidth").toInt();
    CaResult.CIMG_BestCapture.wImgHeight = App_cfg.value("CIMG_SpecialImage/wImgHeight").toInt();
    CaResult.CIMG_BestCapture.dwImgSize = App_cfg.value("CIMG_SpecialImage/dwImgSize").toInt();
    CaResult.CIMG_BestCapture.wImgType = App_cfg.value("CIMG_SpecialImage/wImgType").toInt();
    qstrTemp = App_cfg.value("CIMG_SpecialImage/chSavePath").toString();
    sprintf(CaResult.CIMG_BestCapture.chSavePath, "%s", qstrTemp.toStdString().c_str());

    CaResult.CIMG_LastCapture.wImgWidth = App_cfg.value("CIMG_FullImage/wImgWidth").toInt();
    CaResult.CIMG_LastCapture.wImgHeight = App_cfg.value("CIMG_FullImage/wImgHeight").toInt();
    CaResult.CIMG_LastCapture.dwImgSize = App_cfg.value("CIMG_FullImage/dwImgSize").toInt();
    CaResult.CIMG_LastCapture.wImgType = App_cfg.value("CIMG_FullImage/wImgType").toInt();
    qstrTemp = App_cfg.value("CIMG_FullImage/chSavePath").toString();
    sprintf(CaResult.CIMG_LastCapture.chSavePath, "%s", qstrTemp.toStdString().c_str());

    CaResult.CIMG_PlateImage.wImgWidth = App_cfg.value("CIMG_PlateImage/wImgWidth").toInt();
    CaResult.CIMG_PlateImage.wImgHeight = App_cfg.value("CIMG_PlateImage/wImgHeight").toInt();
    CaResult.CIMG_PlateImage.dwImgSize = App_cfg.value("CIMG_PlateImage/dwImgSize").toInt();
    CaResult.CIMG_PlateImage.wImgType = App_cfg.value("CIMG_PlateImage/wImgType").toInt();
    qstrTemp = App_cfg.value("CIMG_PlateImage/chSavePath").toString();
    sprintf(CaResult.CIMG_PlateImage.chSavePath, "%s", qstrTemp.toStdString().c_str());

    CaResult.CIMG_BinImage.wImgWidth = App_cfg.value("CIMG_BinImage/wImgWidth").toInt();
    CaResult.CIMG_BinImage.wImgHeight = App_cfg.value("CIMG_BinImage/wImgHeight").toInt();
    CaResult.CIMG_BinImage.dwImgSize = App_cfg.value("CIMG_BinImage/dwImgSize").toInt();
    CaResult.CIMG_BinImage.wImgType = App_cfg.value("CIMG_BinImage/wImgType").toInt();
    qstrTemp = App_cfg.value("CIMG_BinImage/chSavePath").toString();
    sprintf(CaResult.CIMG_BinImage.chSavePath, "%s", qstrTemp.toStdString().c_str());

    return true;
}

bool SerializationFromDisk(const char* chFilePath, CameraResult &CaResult)
{
    FILE* tempFile = NULL;
    tempFile = fopen(chFilePath, "rb");
    if(NULL == tempFile)
        return false;
    size_t pos1 = 0;
    fread(&pos1, 1, sizeof(size_t), tempFile);
    CaResult.dwCarID = (DWORD)pos1;
    fread(&pos1, 1,sizeof(size_t), tempFile);
    CaResult.iDeviceID = (int)pos1;
    fread(&pos1, 1, sizeof(size_t), tempFile);
    CaResult.iPlateColor = (int)pos1;
    fread(&pos1, 1, sizeof(size_t), tempFile);
    CaResult.iPlateTypeNo = (int)pos1;
    fread(&pos1, 1, sizeof(size_t), tempFile);
    CaResult.dw64TimeMS = (DWORD)pos1;
    fread(&pos1, 1, sizeof(size_t), tempFile);
    CaResult.iSpeed = (int)pos1;
    fread(&pos1, 1, sizeof(size_t), tempFile);
    CaResult.iResultNo = (int)pos1;
    fread(&pos1, 1, sizeof(size_t), tempFile);
    CaResult.iVehTypeNo = (int)pos1;
    fread(&pos1, 1, sizeof(size_t), tempFile);
    CaResult.iVehBodyColorNo = (int)pos1;
    fread(&pos1, 1, sizeof(size_t), tempFile);
    CaResult.iVehBodyDeepNo = (int)pos1;
    fread(&pos1, 1, sizeof(size_t), tempFile);
    CaResult.iVehLenth = (int)pos1;
    fread(&pos1, 1, sizeof(size_t), tempFile);
    CaResult.iAreaNo = (int)pos1;
    fread(&pos1, 1, sizeof(size_t), tempFile);
    CaResult.iRoadNo = (int)pos1;
    fread(&pos1, 1, sizeof(size_t), tempFile);
    CaResult.iLaneNo = (int)pos1;
    fread(&pos1, 1, sizeof(size_t), tempFile);
    CaResult.iDirection = (int)pos1;

    fread(CaResult.chDeviceIp, 1, 32, tempFile);
    fread(CaResult.chPlateNO, 1, 32, tempFile );
    fread(CaResult.chListNo, 1, 256, tempFile );
    fread(CaResult.chPlateTime, 1, 256, tempFile );
    fread(CaResult.chSignStationID, 1, 50, tempFile );
    fread(CaResult.chSignStationName, 1, 50, tempFile );
    fread(CaResult.pcAppendInfo, 1, 2048, tempFile );
    fread(CaResult.chDeviceID, 1, 3, tempFile );
    fread(CaResult.chLaneID, 1, 3, tempFile );

    //SpecialImage
    size_t pos2 = 0;
    fread(&pos2, 1, sizeof(size_t), tempFile);
    CaResult.CIMG_BestCapture.dwImgSize = (DWORD)pos2;
    fread(&pos2, 1, sizeof(size_t), tempFile);
    CaResult.CIMG_BestCapture.wImgWidth = (WORD)pos2;
    fread(&pos2, 1, sizeof(size_t), tempFile);
    CaResult.CIMG_BestCapture.wImgHeight = (WORD)pos2;
    fread(&pos2, 1, sizeof(size_t), tempFile);
    CaResult.CIMG_BestCapture.wImgType = (WORD)pos2;
    if(NULL != CaResult.CIMG_BestCapture.pbImgData)
    {
        delete[] CaResult.CIMG_BestCapture.pbImgData;
        CaResult.CIMG_BestCapture.pbImgData = NULL;
    }
    CaResult.CIMG_BestCapture.pbImgData = new unsigned char[CaResult.CIMG_BestCapture.dwImgSize];
    fread(CaResult.CIMG_BestCapture.pbImgData, 1,CaResult.CIMG_BestCapture.dwImgSize, tempFile);
    fread(CaResult.CIMG_BestCapture.chSavePath, 1, 256, tempFile);

    //FullImage
    size_t pos3 = 0;
    fread(&pos3, 1, sizeof(size_t), tempFile);
    CaResult.CIMG_LastCapture.dwImgSize = (DWORD)pos3;
    fread(&pos3, 1, sizeof(size_t), tempFile);
    CaResult.CIMG_LastCapture.wImgWidth = (WORD)pos3;
    fread(&pos3, 1, sizeof(size_t), tempFile);
    CaResult.CIMG_LastCapture.wImgHeight = (WORD)pos3;
    fread(&pos3, 1, sizeof(size_t), tempFile);
    CaResult.CIMG_LastCapture.wImgType = (WORD)pos3;
    if(NULL != CaResult.CIMG_LastCapture.pbImgData)
    {
        delete[] CaResult.CIMG_LastCapture.pbImgData;
        CaResult.CIMG_LastCapture.pbImgData = NULL;
    }
    CaResult.CIMG_LastCapture.pbImgData = new unsigned char[CaResult.CIMG_LastCapture.dwImgSize];
    fread(CaResult.CIMG_LastCapture.pbImgData, 1,CaResult.CIMG_LastCapture.dwImgSize, tempFile);
    fread(CaResult.CIMG_LastCapture.chSavePath, 1, 256, tempFile);

    //PlateImage
    size_t pos4 = 0;
    fread(&pos4, 1, sizeof(size_t), tempFile);
    CaResult.CIMG_PlateImage.dwImgSize = (DWORD)pos4;
    fread(&pos4, 1, sizeof(size_t), tempFile);
    CaResult.CIMG_PlateImage.wImgWidth = (WORD)pos4;
    fread(&pos4, 1, sizeof(size_t), tempFile);
    CaResult.CIMG_PlateImage.wImgHeight = (WORD)pos4;
    fread(&pos4, 1, sizeof(size_t), tempFile);
    CaResult.CIMG_PlateImage.wImgType = (WORD)pos4;
    if(NULL != CaResult.CIMG_PlateImage.pbImgData)
    {
        delete[] CaResult.CIMG_PlateImage.pbImgData;
        CaResult.CIMG_PlateImage.pbImgData = NULL;
    }
    CaResult.CIMG_PlateImage.pbImgData = new unsigned char[CaResult.CIMG_PlateImage.dwImgSize];
    fread(CaResult.CIMG_PlateImage.pbImgData, 1,CaResult.CIMG_PlateImage.dwImgSize, tempFile);
    fread(CaResult.CIMG_PlateImage.chSavePath, 1, 256, tempFile);

    //BinImage
    size_t pos5 = 0;
    fread(&pos5, 1, sizeof(size_t), tempFile);
    CaResult.CIMG_BinImage.dwImgSize = (DWORD)pos5;
    fread(&pos5, 1, sizeof(size_t), tempFile);
    CaResult.CIMG_BinImage.wImgWidth = (WORD)pos5;
    fread(&pos5, 1, sizeof(size_t), tempFile);
    CaResult.CIMG_BinImage.wImgHeight = (WORD)pos5;
    fread(&pos5, 1, sizeof(size_t), tempFile);
    CaResult.CIMG_BinImage.wImgType = (WORD)pos5;
    if(NULL != CaResult.CIMG_BinImage.pbImgData)
    {
        delete[] CaResult.CIMG_BinImage.pbImgData;
        CaResult.CIMG_BinImage.pbImgData = NULL;
    }
    CaResult.CIMG_BinImage.pbImgData = new unsigned char[CaResult.CIMG_BinImage.dwImgSize];
    fread(CaResult.CIMG_BinImage.pbImgData, 1,CaResult.CIMG_BinImage.dwImgSize, tempFile);
    fread(CaResult.CIMG_BinImage.chSavePath, 1, 256, tempFile);

    fclose(tempFile);
    tempFile = NULL;
    return true;
}

bool SerializationResultToDisk(const char* chFilePath, const CameraResult &CaResult)
{
    QString qstrSavePath(chFilePath);
    QString qstrTempPath = qstrSavePath.mid(0, qstrSavePath.lastIndexOf("/"));
    QDir tempDir;
    tempDir.mkpath(qstrTempPath);

    FILE* tempFile = NULL;
    tempFile = fopen(chFilePath, "wb+");
    if(NULL == tempFile)
        return false;
    fwrite(&(CaResult.dwCarID), 1, sizeof(size_t), tempFile);
    fwrite(&(CaResult.iDeviceID), 1, sizeof(size_t), tempFile);
    fwrite(&(CaResult.iPlateColor), 1, sizeof(size_t), tempFile);
    fwrite(&(CaResult.iPlateTypeNo), 1, sizeof(size_t), tempFile);
    fwrite(&(CaResult.dw64TimeMS), 1, sizeof(size_t), tempFile);
    fwrite(&(CaResult.iSpeed), 1, sizeof(size_t), tempFile);
    fwrite(&(CaResult.iResultNo), 1, sizeof(size_t), tempFile);
    fwrite(&(CaResult.iVehTypeNo), 1, sizeof(size_t), tempFile);
    fwrite(&(CaResult.iVehBodyColorNo), 1, sizeof(size_t), tempFile);
    fwrite(&(CaResult.iVehBodyDeepNo), 1, sizeof(size_t), tempFile);
    fwrite(&(CaResult.iVehLenth), 1, sizeof(size_t), tempFile);
    fwrite(&(CaResult.iAreaNo), 1, sizeof(size_t), tempFile);
    fwrite(&(CaResult.iRoadNo), 1, sizeof(size_t), tempFile);
    fwrite(&(CaResult.iLaneNo), 1, sizeof(size_t), tempFile);
    fwrite(&(CaResult.iDirection), 1, sizeof(size_t), tempFile);

    fwrite(CaResult.chDeviceIp, 1, 32, tempFile);
    fwrite(CaResult.chPlateNO, 1, 32, tempFile );
    fwrite(CaResult.chListNo, 1, 256, tempFile );
    fwrite(CaResult.chPlateTime, 1, 256, tempFile );
    fwrite(CaResult.chSignStationID, 1, 50, tempFile );
    fwrite(CaResult.chSignStationName, 1, 50, tempFile );
    fwrite(CaResult.pcAppendInfo, 1, 2048, tempFile );
    fwrite(CaResult.chDeviceID, 1, 3, tempFile );
    fwrite(CaResult.chLaneID, 1, 3, tempFile );

    //SpecialImage
    fwrite(&(CaResult.CIMG_BestCapture.dwImgSize), 1, sizeof(size_t), tempFile);
    fwrite(&(CaResult.CIMG_BestCapture.wImgWidth), 1, sizeof(size_t), tempFile);
    fwrite(&(CaResult.CIMG_BestCapture.wImgHeight), 1, sizeof(size_t), tempFile);
    fwrite(&(CaResult.CIMG_BestCapture.wImgType), 1, sizeof(size_t), tempFile);
    fwrite(CaResult.CIMG_BestCapture.pbImgData, 1,CaResult.CIMG_BestCapture.dwImgSize, tempFile);
    fwrite(CaResult.CIMG_BestCapture.chSavePath, 1, 256, tempFile);

    //FullImage
    fwrite(&(CaResult.CIMG_LastCapture.dwImgSize), 1, sizeof(size_t), tempFile);
    fwrite(&(CaResult.CIMG_LastCapture.wImgWidth), 1, sizeof(size_t), tempFile);
    fwrite(&(CaResult.CIMG_LastCapture.wImgHeight), 1, sizeof(size_t), tempFile);
    fwrite(&(CaResult.CIMG_LastCapture.wImgType), 1, sizeof(size_t), tempFile);
    fwrite(CaResult.CIMG_LastCapture.pbImgData, 1,CaResult.CIMG_LastCapture.dwImgSize, tempFile);
    fwrite(CaResult.CIMG_LastCapture.chSavePath, 1, 256, tempFile);

    //PlateImage
    fwrite(&(CaResult.CIMG_PlateImage.dwImgSize), 1, sizeof(size_t), tempFile);
    fwrite(&(CaResult.CIMG_PlateImage.wImgWidth), 1, sizeof(size_t), tempFile);
    fwrite(&(CaResult.CIMG_PlateImage.wImgHeight), 1, sizeof(size_t), tempFile);
    fwrite(&(CaResult.CIMG_PlateImage.wImgType), 1, sizeof(size_t), tempFile);
    fwrite(CaResult.CIMG_PlateImage.pbImgData, 1,CaResult.CIMG_PlateImage.dwImgSize, tempFile);
    fwrite(CaResult.CIMG_PlateImage.chSavePath, 1, 256, tempFile);

    //BinImage
    fwrite(&(CaResult.CIMG_BinImage.dwImgSize), 1, sizeof(size_t), tempFile);
    fwrite(&(CaResult.CIMG_BinImage.wImgWidth), 1, sizeof(size_t), tempFile);
    fwrite(&(CaResult.CIMG_BinImage.wImgHeight), 1, sizeof(size_t), tempFile);
    fwrite(&(CaResult.CIMG_BinImage.wImgType), 1, sizeof(size_t), tempFile);
    fwrite(CaResult.CIMG_BinImage.pbImgData, 1,CaResult.CIMG_BinImage.dwImgSize, tempFile);
    fwrite(CaResult.CIMG_BinImage.chSavePath, 1, 256, tempFile);

    fclose(tempFile);
    tempFile = NULL;
    return true;
}

