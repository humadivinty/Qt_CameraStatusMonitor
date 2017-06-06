#include "AlarmModule.h"
#include<QDebug>
#include<QMutexLocker>
#include<QDir>
#include<QDateTime>

AlarmModule::AlarmModule(QObject *parent) : QObject(parent)
{
    qRegisterMetaType<AlarmMessage>("AlarmMessage");
    qRegisterMetaType<QVector<int> >("QVector<int>");
}

void AlarmModule::slot_Receive_AlarmMsessage(AlarmMessage Message)
{
    qDebug()<<"type ="<<Message.iType<<"Values ="<<Message.qstrContent;

    WriteAlarmLog(Message.qstrDeviceIP, Message.qstrContent);
    emit signal_Send_AlarmMessage(Message);
}

bool AlarmModule::WriteAlarmLog(QString fileName, QString LogContent)
{
    QMutexLocker locker(&m_mutexLog);

    if(LogContent.isEmpty())
        return false;
    QDir tempDir;
    QString qstrCurrentPath = QDir::currentPath();
    qstrCurrentPath.append("//SNW_log//AlarmLog");
    if(!tempDir.exists(qstrCurrentPath))
    {
        tempDir.mkdir(qstrCurrentPath);
    }
    QDate currentDate = QDate::currentDate();
    QTime currentTime = QTime::currentTime();
    QString qstrFileName = QString(qstrCurrentPath+"//%1.log").arg(fileName);

    QFile logFile(qstrFileName);
    if(!logFile.open(QIODevice::Append | QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug()<<"open log File failed";
        return false;
    }
    QString qstrLog = QString("%1 %2:  %3").arg(currentDate.toString("yyyy-MM-dd")).arg(currentTime.toString("hh:mm:ss zzz")).arg(LogContent);

    QTextStream qStreamLog(&logFile);
    qStreamLog<<qstrLog<<endl;
    //qDebug()<<qstrLog;

    logFile.close();
    return true;
}

