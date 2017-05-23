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

    bool addCameraByIPaddress(QString ipaddress);
    void RemoveCamerFromIPaddress(QString ipaddress);
    void RemoveAllCamera();
    void SetDataModel(CustomTableModel* DataModel);

    void StarUpLoad();
    void ExitUpLoad();

signals:
    void signal_AddCameAddressFinish();
    void signal_UpLoadCamerStatus();

public slots:
    void slot_ConnectCamer();
    void slot_UpLoadCamerStatus();

private:
    QThread uploadWorkerThread;
    QThread ConnectWorkerThread;

    QList<Camera6467*> m_pCamList;
    CustomTableModel* m_pDataModel;

    bool m_bUpLoadExit;

    QMutex m_mutex;

private:
    void PushDataToModel(CameraInfo& info);
};

#endif // CAMCONNECTCONTRL_H
