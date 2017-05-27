#include "mainwindow.h"
#include "ui_mainwindow.h"
#include"LogModel/glogmodel.h"
#include<QDebug>
#include<QDateTime>
#include<QPicture>
#include <QDir>
#include <QSettings>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->pushButton->setVisible(false);

    QPixmap pixmap("./greenLight.png");
    ui->label->setPixmap(pixmap);
    ui->label->show();

    m_pTableModel = NULL;
    m_pCamController = NULL;
    m_pStatusEventCheck = NULL;
    m_pUnlicensePlateCheck = NULL;
    m_pAlarmModel = NULL;

    m_pTableModel = new CustomTableModel();
    ui->StatusTableView->setModel(m_pTableModel);

    ReadConfig();
    InitAlarmModel();

    InitConnectModel();
    InitEventCheckModel();

}

MainWindow::~MainWindow()
{
    delete ui;

    UnInitEventCheckModel();
    UnInitAlarmModel();
    UnInitConnectModel();

    if(m_pTableModel)
    {
        delete m_pTableModel;
        m_pTableModel = NULL;
    }
}

void MainWindow::on_pushButton_clicked()
{
//    AlarmMessage MyMessage;
//    MyMessage.iType =1;
//    MyMessage.qstrContent = QString("success!");
//    emit signal_SendAlarm(MyMessage);
    QModelIndex index = this->ui->StatusTableView->currentIndex();
    QStringList tempList = index.data().toStringList();
    qDebug()<<tempList;

}

void MainWindow::slot_Receive_AlarmMsessage(AlarmMessage Msg)
{
    qDebug()<<"Receive Message";

    static int iTimes = 0;
   QPixmap pixmap;
    if(iTimes++ > 100)
    {
        iTimes = 0;
        this->ui->listWidget->clear();
    }
    QString qstrItem = QString(" [%1] : %2 ").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss.zzz")).arg(Msg.qstrContent);
    this->ui->listWidget->insertItem(0, qstrItem);

    switch (Msg.iType)
    {
    case ALARM_EVENT_NORMAL:
        pixmap.load("./GreenLight.png");
        ui->label->setPixmap(pixmap);
        break;

    case ALARM_EVENT_DISCONNECT_TIMEOUT:
        pixmap.load("./RetLight.png");
        ui->label->setPixmap(pixmap);
        break;

    case ALARM_EVENT_UNLICENSE:
        pixmap.load("./RetLight.png");
        ui->label->setPixmap(pixmap);
        break;

    default:
        break;
    }
}

void MainWindow::ReadConfig()
{
    QString qstrCurrentPath = QDir::currentPath();
    QString qstrFilePath = qstrCurrentPath+"/XLW_Config.ini";
    QSettings App_cfg(qstrFilePath,QSettings::IniFormat );

    m_iUnLicenseRate = App_cfg.value("ApplicationInfo/UnLicenseRate", 30).toInt();
    App_cfg.setValue("ApplicationInfo/UnLicenseRate", m_iUnLicenseRate);

    m_iDisconnectCount = App_cfg.value("ApplicationInfo/DisconnectCount",5 ).toInt();
    App_cfg.setValue("ApplicationInfo/DisconnectCount",m_iDisconnectCount );

    m_iCheckTimeRange = App_cfg.value("ApplicationInfo/CheckTimeRange",10 ).toInt();
    App_cfg.setValue("ApplicationInfo/CheckTimeRange",m_iCheckTimeRange );

    m_iDurationTime = App_cfg.value("ApplicationInfo/DurationTime",5 ).toInt();
    App_cfg.setValue("ApplicationInfo/DurationTime",m_iDurationTime );

    m_iCameraCount = App_cfg.value("ApplicationInfo/CameraCount",1 ).toInt();
    App_cfg.setValue("ApplicationInfo/CameraCount", m_iCameraCount);


    QString qstrCamIp, qstrStationID;
    for(int i= 0; i< m_iCameraCount; i++)
    {
        CameraInfo tempInfo;
        qstrCamIp = App_cfg.value(QString("Camera%1/IpAddress").arg(i+1), "127.0.0.1").toString();
        App_cfg.setValue(QString("Camera%1/IpAddress").arg(i+1), qstrCamIp);

        qstrStationID = App_cfg.value(QString("Camera%1/StationInfo").arg(i+1), "").toString();
        strcpy(tempInfo.chIP, qstrCamIp.toStdString().c_str());
        strcpy(tempInfo.chStationID, qstrStationID.toStdString().c_str());
        m_CamInfoList.append(tempInfo);
    }
}

void MainWindow::InitEventCheckModel()
{
    if(!m_pStatusEventCheck)
    {
        GLogModel::GetInstant()->WriteLog("MainWindow","Init StatusEventCheck Model.");
        m_pStatusEventCheck = new StatustCheck();

        connect(m_pStatusEventCheck,  &(m_pStatusEventCheck->signal_SendAlarm), m_pAlarmModel, &(m_pAlarmModel->slot_Receive_AlarmMsessage));

        m_pStatusEventCheck->SetDataModel(m_pTableModel);
        m_pStatusEventCheck->SetDisConTimeRangeAndCount(m_iCheckTimeRange, m_iDisconnectCount);
        m_pStatusEventCheck->SetCheckDisconnectTimeOut(m_iDurationTime);
        m_pStatusEventCheck->StartEvent();
    }

    if(!m_pUnlicensePlateCheck)
    {
        GLogModel::GetInstant()->WriteLog("MainWindow","Init UnlicensePlateCheck Model.");
        m_pUnlicensePlateCheck = new UnlicensePlateCheck();

        connect(m_pUnlicensePlateCheck, &(m_pUnlicensePlateCheck->signal_SendAlarm), m_pAlarmModel, &(m_pAlarmModel->slot_Receive_AlarmMsessage));

        m_pUnlicensePlateCheck->SetDataModel(m_pTableModel);
        m_pUnlicensePlateCheck->SetUnlicenseRate(m_iUnLicenseRate);
        m_pUnlicensePlateCheck->StartEvent();
    }
}

void MainWindow::InitAlarmModel()
{
    if(!m_pAlarmModel)
    {
        GLogModel::GetInstant()->WriteLog("MainWindow","Init AlarmModel Model.");

        m_pAlarmModel = new AlarmModule();

        connect(this, signal_SendAlarm, m_pAlarmModel, &(m_pAlarmModel->slot_Receive_AlarmMsessage));
        connect(m_pAlarmModel, &(m_pAlarmModel->signal_Send_AlarmMessage), this,slot_Receive_AlarmMsessage);
    }
}

void MainWindow::InitConnectModel()
{
    if(!m_pCamController)
    {
        GLogModel::GetInstant()->WriteLog("MainWindow","Init ConnectModel.");

        m_pCamController = new CamConnectContrl();
        m_pCamController->SetDataModel(m_pTableModel);
        //m_pCamController->addCameraByIPaddress("172.18.80.166");
        for(int i= 0; i< m_CamInfoList.count(); i++)
        {
            m_pCamController->addCameraByIPaddress(m_CamInfoList[i].chIP, m_CamInfoList[i].chStationID);
        }
        //m_pCamController->addCameraByIPaddress("172.18.80.65", "第一个");
        m_pCamController->StarUpLoad();
    }
}

void MainWindow::UnInitEventCheckModel()
{
    if(m_pStatusEventCheck)
    {
        GLogModel::GetInstant()->WriteLog("MainWindow","UnInit StatusEventCheck.");

        disconnect(m_pStatusEventCheck,  &(m_pStatusEventCheck->signal_SendAlarm), m_pAlarmModel, &(m_pAlarmModel->slot_Receive_AlarmMsessage));

        delete m_pStatusEventCheck;
        m_pStatusEventCheck = NULL;
    }

    if(m_pUnlicensePlateCheck)
    {
        GLogModel::GetInstant()->WriteLog("MainWindow","UnInit UnlicensePlateCheck.");

        disconnect(m_pUnlicensePlateCheck, &(m_pUnlicensePlateCheck->signal_SendAlarm), m_pAlarmModel, &(m_pAlarmModel->slot_Receive_AlarmMsessage));

        delete m_pUnlicensePlateCheck;
        m_pUnlicensePlateCheck = NULL;
    }
}

void MainWindow::UnInitAlarmModel()
{
    if(m_pAlarmModel)
    {
       GLogModel::GetInstant()->WriteLog("MainWindow","UnInit Alarm Model.");

       disconnect(this, signal_SendAlarm, m_pAlarmModel, &(m_pAlarmModel->slot_Receive_AlarmMsessage));
       disconnect(m_pAlarmModel, &(m_pAlarmModel->signal_Send_AlarmMessage), this,slot_Receive_AlarmMsessage);

        delete m_pAlarmModel;
        m_pAlarmModel = NULL;
    }
}

void MainWindow::UnInitConnectModel()
{
    if(m_pCamController)
    {
        GLogModel::GetInstant()->WriteLog("MainWindow","UnInit Connect Model.");
        delete m_pCamController;
        m_pCamController = NULL;
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    AlarmMessage MyMessage;
    MyMessage.iType =ALARM_EVENT_NORMAL;
    MyMessage.qstrContent = QString::fromLocal8Bit("解除当前警报");
    emit signal_SendAlarm(MyMessage);
}
