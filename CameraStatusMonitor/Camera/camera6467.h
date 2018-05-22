#ifndef CAMERA6467
#define CAMERA6467
#include <QString>
#include "HvDeviceAPI/HvDeviceNew.h"
#include "Camera/cameraresult.h"
#include <QMutex>
#include <list>


class CameraInfo
{
public:
    char chIP[128];                          //deviceIP
    char chDeviceID[128];
    char chStationID[128];
    char chLaneID[128];
    bool bLogEnable;
    bool bSynTimeEnable;
    bool bIsInit;
    int iDirection;
    int iConnectStatus;
    float fUnlicensedPlateRatio;    //ÎÞÅÆ³µ±ÈÀý
    CameraInfo()
    {
        memset(chIP, 0, 128);
        memset(chDeviceID, 0, 128);
        memset(chStationID, 0, 128);
        memset(chLaneID, 0, 128);
        bLogEnable = true;
        bSynTimeEnable = false;
        bIsInit = false;
        iDirection = 0;
        iConnectStatus = -1;
        fUnlicensedPlateRatio = 0.0;
    }
    CameraInfo(const CameraInfo& Info)
    {
        strcpy(chIP, Info.chIP);
        strcpy(chDeviceID,Info.chDeviceID);
        strcpy(chStationID, Info.chStationID);
        strcpy(chLaneID, Info.chLaneID);

        bLogEnable = Info.bLogEnable;
        bSynTimeEnable = Info.bSynTimeEnable;
        bIsInit = Info.bIsInit;
        iDirection = Info.iDirection;
        iConnectStatus = Info.iConnectStatus;
        fUnlicensedPlateRatio = Info.fUnlicensedPlateRatio;
    }

    CameraInfo &operator=(const CameraInfo &Info)
    {
        strcpy(chIP, Info.chIP);
        strcpy(chDeviceID,Info.chDeviceID);
        strcpy(chStationID, Info.chStationID);
        strcpy(chLaneID, Info.chLaneID);

        bLogEnable = Info.bLogEnable;
        bSynTimeEnable = Info.bSynTimeEnable;
        bIsInit = Info.bIsInit;
        iDirection = Info.iDirection;
        iConnectStatus = Info.iConnectStatus;
        fUnlicensedPlateRatio = Info.fUnlicensedPlateRatio;

        return *this;
    }
};

struct SaveModeInfo
{
    int iSafeModeEnable;
    char chBeginTime[260];
    char chEndTime[260];
    int iIndex;
    int iDataType;
    SaveModeInfo()
    {
        iSafeModeEnable = 0;
        iIndex = 0;
        iDataType = 0;
        memset(chBeginTime, 0, 260);
        memset(chEndTime, 0, 260);
    }
};

class Camera6467
{
public:
    Camera6467();
    Camera6467(const char* chIP);
    ~Camera6467();


    bool SetCameraInfo(CameraInfo& camInfo);

    int GetCamStatus();
    QString GetStationID();
    QString GetDeviceID();
    QString GetLaneID();
    std::string GetCameraIP();

    bool ConnectToCamera();
    void ReadConfig();
    bool WriteLog(const char* chlog);
    bool WriteLog(const QString& qstrLogBuff);
    bool TakeCapture();
    bool SynTime();
    void AnalysisAppendInfo(CameraResult* CamResult);
    bool IsFirstConnect();
    CameraInfo& GetCameraInfo();

    void SetOutPutList(void* outList);


    CameraResult* m_CameraResult;

private:
    void* m_hHvHandle;
    std::string m_strIP;
    std::string m_strDeviceID;
    int m_iConnectStatus;

    char m_chDeviceID[3];
    char m_chStationID[7];
    char m_chLaneID[3];
    bool m_bLogEnable;
    bool m_bSynTime;
    bool m_bResultComplete;
    bool m_bFirstConnect;
    DWORD m_dwConnectStatus;
    int m_iCompressQuality;
    int m_iDirection;
    int m_iTotalPlateCount;
    int m_iUnlisensePlateCount;

    QString m_qstrBackupPath;
    QString m_qstrRootPath;
    QString m_qstrTextCodeName;
    SaveModeInfo m_SaveModelInfo;
    CameraInfo m_CamInfo;
    QMutex m_ResultMutex;

    void* g_pOutputList;

    void ReadHistoryInfo();
    void WriteHistoryInfo(SaveModeInfo& SaveInfo);

    bool SaveImgToDisk( QString chImgPath, BYTE* pImgData, DWORD dwImgSize);
    bool SaveImgToDiskByQt(CameraIMG &camImageStruct);

    void SetImgPath(CameraResult* camResult);

public:
    static int  RecordInfoBeginCallBack(PVOID pUserData, DWORD dwCarID)
    {
        if ( pUserData == NULL )
            return 0;

        Camera6467* pThis = (Camera6467*)pUserData;
        return pThis->RecordInfoBegin(dwCarID);
    };
    int RecordInfoBegin(DWORD dwCarID);


    static int  RecordInfoEndCallBack(PVOID pUserData, DWORD dwCarID)
    {
        if ( pUserData == NULL )
            return 0;

        Camera6467* pThis = (Camera6467*)pUserData;
        return pThis->RecordInfoEnd(dwCarID);
    };
    int RecordInfoEnd(DWORD dwCarID);


    static int  RecordInfoPlateCallBack(PVOID pUserData,
        DWORD dwCarID,
        LPCSTR pcPlateNo,
        LPCSTR pcAppendInfo,
        DWORD dwRecordType,
        DWORD64 dw64TimeMS)
    {
        if (pUserData == NULL)
            return 0;

        Camera6467* pThis = (Camera6467*)pUserData;
        return pThis->RecordInfoPlate(dwCarID, pcPlateNo, pcAppendInfo, dwRecordType, dw64TimeMS);
    }
    int RecordInfoPlate(DWORD dwCarID,
        LPCSTR pcPlateNo,
        LPCSTR pcAppendInfo,
        DWORD dwRecordType,
        DWORD64 dw64TimeMS);


    static int  RecordInfoBigImageCallBack(PVOID pUserData,
        DWORD dwCarID,
        WORD  wImgType,
        WORD  wWidth,
        WORD  wHeight,
        PBYTE pbPicData,
        DWORD dwImgDataLen,
        DWORD dwRecordType,
        DWORD64 dw64TimeMS)
    {
        if (pUserData == NULL)
            return 0;

        Camera6467* pThis = (Camera6467*)pUserData;
        return pThis->RecordInfoBigImage(dwCarID, wImgType, wWidth, wHeight, pbPicData, dwImgDataLen, dwRecordType, dw64TimeMS);
    }
    int RecordInfoBigImage(DWORD dwCarID,
                           WORD  wImgType,
                           WORD  wWidth,
                           WORD  wHeight,
                           PBYTE pbPicData,
                           DWORD dwImgDataLen,
                           DWORD dwRecordType,
                           DWORD64 dw64TimeMS);



    static int  RecordInfoSmallImageCallBack(PVOID pUserData,
        DWORD dwCarID,
        WORD wWidth,
        WORD wHeight,
        PBYTE pbPicData,
        DWORD dwImgDataLen,
        DWORD dwRecordType,
        DWORD64 dw64TimeMS)
    {
        if (pUserData == NULL)
            return 0;

        Camera6467* pThis = (Camera6467*)pUserData;
        return pThis->RecordInfoSmallImage(dwCarID, wWidth, wHeight, pbPicData, dwImgDataLen, dwRecordType, dw64TimeMS);
    }
    int RecordInfoSmallImage(DWORD dwCarID,
        WORD wWidth,
        WORD wHeight,
        PBYTE pbPicData,
        DWORD dwImgDataLen,
        DWORD dwRecordType,
        DWORD64 dw64TimeMS);


    static int  RecordInfoBinaryImageCallBack(PVOID pUserData,
        DWORD dwCarID,
        WORD wWidth,
        WORD wHeight,
        PBYTE pbPicData,
        DWORD dwImgDataLen,
        DWORD dwRecordType,
        DWORD64 dw64TimeMS)
    {
        if (pUserData == NULL)
            return 0;

        Camera6467* pThis = (Camera6467*)pUserData;
        return pThis->RecordInfoBinaryImage(dwCarID, wWidth, wHeight, pbPicData, dwImgDataLen, dwRecordType, dw64TimeMS);
    }
    int RecordInfoBinaryImage(DWORD dwCarID,
        WORD wWidth,
        WORD wHeight,
        PBYTE pbPicData,
        DWORD dwImgDataLen,
        DWORD dwRecordType,
        DWORD64 dw64TimeMS);

    inline void CopyDataToIMG(CameraIMG& DestImg, PBYTE pImgData, unsigned long width, unsigned long height, unsigned long dataLenth, WORD wImgType)
    {
        if(NULL != DestImg.pbImgData)
        {
            delete[] DestImg.pbImgData;
            DestImg.pbImgData = NULL;
        }

        DestImg.pbImgData = new(std::nothrow) BYTE[dataLenth];
        DestImg.dwImgSize = 0;
        if (  NULL != DestImg.pbImgData)
        {
            memcpy(DestImg.pbImgData, pImgData, dataLenth);
            DestImg.wImgWidth = width;
            DestImg.wImgHeight = height;
            DestImg.dwImgSize = dataLenth;
            DestImg.wImgType = wImgType;
        }
    }
};
#endif // CAMERA6467

