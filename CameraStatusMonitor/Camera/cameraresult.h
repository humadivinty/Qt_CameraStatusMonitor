#ifndef CAMERARESULT
#define CAMERARESULT
#include"HvDeviceAPI/HvDeviceBaseType.h"
#include<string.h>

//typedef struct CameraIMG :public CObject
class CameraIMG
{
public:
    CameraIMG();
    CameraIMG(const CameraIMG& CaIMG);
    ~CameraIMG();
    WORD wImgWidth;
    WORD wImgHeight;
    DWORD dwImgSize;
    WORD  wImgType;
    char chSavePath[256];
    PBYTE pbImgData;

    CameraIMG& operator = (const CameraIMG& CaIMG);
};

//typedef struct CameraResult :public CObject
class CameraResult
{
public:

    CameraResult();
    CameraResult(const CameraResult& CaRESULT);
    ~CameraResult();

    DWORD dwCarID;
    int iDeviceID;
    int iPlateColor;
    int iPlateTypeNo;
    DWORD64 dw64TimeMS;
    int iSpeed;
    int iResultNo;
    int iVehTypeNo;
    int iVehBodyColorNo;
    int iVehBodyDeepNo;
    int iVehLenth;
    int iAreaNo;
    int iRoadNo;
    int iLaneNo;
    int iDirection;


    char chDeviceIp[32];
    char chPlateNO[32];
    char chListNo[256];
    char chPlateTime[256];
    char chSignStationID[50];
    char chSignStationName[50];
    char pcAppendInfo[2048];

    char chDeviceID[3];
    char chLaneID[3];


    CameraIMG CIMG_BestSnapshot;	/**< 最清晰识别图 */
    CameraIMG CIMG_LastSnapshot;	/**< 最后识别图 */
    CameraIMG CIMG_BeginCapture;	/**< 开始抓拍图 */
    CameraIMG CIMG_BestCapture;		/**< 最清晰抓拍图 */
    CameraIMG CIMG_LastCapture;		/**< 最后抓拍图 */
    CameraIMG CIMG_PlateImage;		/**< 车牌小图 */
    CameraIMG CIMG_BinImage;			/**< 车牌二值图 */

    CameraResult& operator = (const CameraResult& CaRESULT);

    friend bool SerializationResultToDisk(const char* chFilePath, const CameraResult& CaResult);
    friend bool SerializationFromDisk(const char* chFilePath, CameraResult& CaResult);

    friend bool SerializationAsConfigToDisk(const char* chFilePath, const CameraResult& CaResult);
    friend bool SerializationAsConfigFromDisk(const char* chFilePath, CameraResult& CaResult);
};


typedef struct _tagSafeModeInfo
{
    int iEableSafeMode;
    char chBeginTime[256];
    char chEndTime[256];
    int index;
    int DataInfo;
    _tagSafeModeInfo()
    {
        iEableSafeMode = 0;
        memset(chBeginTime, 0, sizeof(chBeginTime));
        memset(chEndTime, 0, sizeof(chEndTime));
        index = 0;
        DataInfo = 0;
    }
}_tagSafeModeInfo;

typedef struct ParamValue
{
    char szRoadName[256];
    char szRoadDirection[256];
}ParamValue;
#endif // CAMERARESULT

