#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include"DataModel/CustomTableModel.h"
#include"Controller/camconnectcontrl.h"
#include"EventCheck/StatustCheck.h"
#include"EventCheck/UnlicensePlateCheck.h"
#include"AlarmModule/AlarmModule.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    int m_iUnLicenseRate;          //无车牌所占比率
    int m_iCheckTimeRange;      //监测断开次数的时间段
    int m_iDisconnectCount;     //断开次数

    int m_iDurationTime;        //断开状态持续时间

    int m_iCameraCount;          //相机数
    QList<CameraInfo> m_CamInfoList;
public:
    CustomTableModel* m_pTableModel;
    CamConnectContrl* m_pCamController;
    StatustCheck* m_pStatusEventCheck;
    UnlicensePlateCheck* m_pUnlicensePlateCheck;
    AlarmModule* m_pAlarmModel;

signals:
    void signal_SendAlarm(AlarmMessage);
private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_StatusTableView_clicked(const QModelIndex &index);

    void on_pushButton_3_clicked();

    void on_pushButton_delCam_clicked();

public slots:
    void slot_Receive_AlarmMsessage(AlarmMessage);

private:
    void ReadConfig();
    void InitEventCheckModel();
    void InitAlarmModel();
    void InitConnectModel();

    void UnInitEventCheckModel();
    void UnInitAlarmModel();
    void UnInitConnectModel();

    void ShowLogToTab2(QString IPAddress);

    void deleteDir(const QString &d);

    void GetCameraList();
    void AddCamer();
    void AddIPAddressToFile(CameraInfo& info);
    void AddCameraToList(CameraInfo& info);

    void RemoveCameraFromFile(const QString IPAddress);
    void RemoveCameraFromList(const QString IPAddress);
};

#endif // MAINWINDOW_H
