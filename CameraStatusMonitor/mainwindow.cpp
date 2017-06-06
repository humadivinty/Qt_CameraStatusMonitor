#include "mainwindow.h"
#include "ui_mainwindow.h"
#include"LogModel/glogmodel.h"
#include<QDebug>
#include<QDateTime>
#include<QPicture>
#include <QDir>
#include <QSettings>
#include <QTextCodec>
#include<QInputDialog>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle(tr("CamerStatuseMonitor"));

    ui->pushButton->setVisible(false);
    ui->StatusTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->StatusTableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->StatusTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    emit  ui->tabWidget->setCurrentIndex(0);

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
    bool ok;
    QString text = QInputDialog::getText(this, tr("Input your Device Ip address."),
                                         tr("IP Address:"), QLineEdit::Normal,
                                         "192.168.0.1", &ok);
    if (ok && !text.isEmpty())
    {
        qDebug()<<text;
        if(m_pCamController)
        {
            GLogModel::GetInstant()->WriteLog("MainWindow","Init ConnectModel.");
            int iIndex = -1;
            for(int i= 0; i< m_CamInfoList.count(); i++)
            {
                if(text == QString(m_CamInfoList[i].chIP))
                {
                    iIndex = i;
                    break;
                }
            }
            if(iIndex == -1)
            {
                //如果在原队列中没有这个设备IP，就添加进去
                CameraInfo tempInfo;
                strcpy(tempInfo.chIP, text.toStdString().c_str());
                m_CamInfoList.append(tempInfo);

                m_pCamController->addCameraByIPaddress(tempInfo.chIP, tempInfo.chStationID);
            }
        }
    }
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
    App_cfg.setIniCodec(QTextCodec::codecForLocale());

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
//        if(m_pTableModel)
//        {
//            connect(this, signal_SendAlarm, m_pTableModel, &(m_pTableModel->slot_Receive_AlarmMsessage));
//            connect(m_pAlarmModel, &(m_pAlarmModel->signal_Send_AlarmMessage), m_pTableModel,&(m_pTableModel->slot_Receive_AlarmMsessage));
//        }
    }
}

void MainWindow::InitConnectModel()
{
    if(!m_pCamController)
    {
        GLogModel::GetInstant()->WriteLog("MainWindow","Init ConnectModel.");

        m_pCamController = new CamConnectContrl();
        m_pCamController->SetDataModel(m_pTableModel);
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

//       if(m_pTableModel)
//       {
//           disconnect(this, signal_SendAlarm, m_pTableModel, &(m_pTableModel->slot_Receive_AlarmMsessage));
//           disconnect(m_pAlarmModel, &(m_pAlarmModel->signal_Send_AlarmMessage), m_pTableModel,&(m_pTableModel->slot_Receive_AlarmMsessage));
//       }

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

//读取日志文件并显示到界面中
void MainWindow::ShowLogToTab2(QString IPAddress)
{
    ui->tabWidget->setTabText(1, IPAddress);
    emit  ui->tabWidget->setCurrentIndex(1);

    QString qstrCurrentPath = QDir::currentPath();
    qstrCurrentPath.append("//SNW_log//AlarmLog//");
    qstrCurrentPath.append(IPAddress);

    QFile file(QString("%1.log").arg(qstrCurrentPath));
     if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
         return;

     ui->listWidget_2->clear();
     QTextStream in(&file);
     QString line = in.readLine();
     while (!line.isNull()) {
         ui->listWidget_2->insertItem(0, line);
         line = in.readLine();
     }
}

void MainWindow::on_pushButton_2_clicked()
{
    AlarmMessage MyMessage;
    MyMessage.iType =ALARM_EVENT_NORMAL;
    MyMessage.qstrDeviceIP=tr("MainWindow");
    MyMessage.qstrContent = tr("Dismiss current alert");
    emit signal_SendAlarm(MyMessage);
}

void MainWindow::on_StatusTableView_clicked(const QModelIndex &index)
{
    QModelIndex MyIndex = index.sibling(index.row(), 0);
    qDebug()<<MyIndex.data().toString();

    ShowLogToTab2(MyIndex.data().toString());
}
