#ifndef CAMCONNECTCONTRL_H
#define CAMCONNECTCONTRL_H

#include <QObject>
#include <QThread>
#include <QMutex>

#include"Camera/camera6467.h"
#include"DataModel/CustomTableModel.h"


class CamConnectContrl : public QObject
{
    Q_OBJECT
public:
    explicit CamConnectContrl(QObject *parent = 0);
    ~CamConnectContrl();

    bool addCameraByIPaddress(QString ipaddress, QString CameraName);
    void RemoveCamerFromIPaddress(QString ipaddress);
    void RemoveAllCamera();
    void SetDataModel(CustomTableModel* DataModel);
    void SetOutPutResultList(void* dataList);

    void StarUpLoad();
    void ExitUpLoad();

signals:
    //void signal_AddCameAddressFinish();
    void signal_UpLoadCamerStatus();

public slots:
    void slot_ConnectCamer();
    void slot_UpLoadCamerStatus();

private:
    QThread uploadWorkerThread;

    QList<Camera6467*> m_pCamList;
    CustomTableModel* m_pDataModel;

    bool m_bUpLoadExit;

    void* g_pDataList;

    QMutex m_mutex;

private:
    void PushDataToModel(CameraInfo& info);
    void UpLoadData();
    bool GetExitStatus();
};

#endif // CAMCONNECTCONTRL_H
