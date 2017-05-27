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
    int m_iUnLicenseRate;          //�޳�����ռ����
    int m_iCheckTimeRange;      //���Ͽ�������ʱ���
    int m_iDisconnectCount;     //�Ͽ�����

    int m_iDurationTime;        //�Ͽ�״̬����ʱ��

    int m_iCameraCount;          //�����
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
};

#endif // MAINWINDOW_H