#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include"DataModel/CustomTableModel.h"
#include"Controller/camconnectcontrl.h"
#include"EventCheck/StatustCheck.h"
#include"EventCheck/UnlicensePlateCheck.h"
#include"EventCheck/plateblacklistcheck.h"
#include"AlarmModule/AlarmModule.h"
#include"DataModel/custresultlist.h"
#include"AlarmModule/ttsformicrosoft.h"


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
    int m_iUnLicenseCheckEnable;
    int m_iConnectingCheckEnable;
    int m_iBlackListCheckEnable;
    int m_iUnLicenseRate;          //�޳�����ռ����
    int m_iCheckTimeRange;      //���Ͽ�������ʱ���
    int m_iDisconnectCount;     //�Ͽ�����

    int m_iDurationTime;        //�Ͽ�״̬����ʱ��

    int m_iCameraCount;          //�����
    QList<CameraInfo> m_CamInfoList;
    ThreadSafeList<CameraResult*> m_lsResult;
public:
    CustomTableModel* m_pTableModel;
    CamConnectContrl* m_pCamController;
    StatustCheck* m_pStatusEventCheck;
    UnlicensePlateCheck* m_pUnlicensePlateCheck;
    AlarmModule* m_pAlarmModel;
    PlateBlackListCheck* m_pPlateBlackListCheck;
    //TextSpeaker m_speaker;
    TTSforMicrosoft m_speaker;

signals:
    void signal_SendAlarm(AlarmMessage);
    void signal_SendVoiceSignal(int);
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
    void InitUISeting();
    void InitEventCheckModel();
    void InitAlarmModel();
    void InitConnectModel();

    void UnInitEventCheckModel();
    void UnInitAlarmModel();
    void UnInitConnectModel();
    void ClearResultList();

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
