#include "glogmodel.h"
#include <QDir>
#include <QTextStream>
#include <QDate>
#include <QTime>
#include <QMutexLocker>
#include<QDebug>

QMutex g_mutex;

GLogModel::GLogModel(QObject *parent) : QObject(parent)
{
    WriteLog("this", "create GLogModel.");
}

GLogModel::~GLogModel()
{
    WriteLog("this", "release GLogModel");
}

bool GLogModel::WriteLog(const QString& PartName, const QString& LogContent)
{
    QMutexLocker locker(&m_mutexLog);

    if(LogContent.isEmpty())
        return false;
    QDir tempDir;
    QString qstrCurrentPath = QDir::currentPath();
    qstrCurrentPath.append("//SNW_log");
    if(!tempDir.exists(qstrCurrentPath))
    {
        tempDir.mkdir(qstrCurrentPath);
    }
    QDate currentDate = QDate::currentDate();
    QTime currentTime = QTime::currentTime();
    QString qstrFileName = QString(qstrCurrentPath+"//Monitor_log_%1.log").arg(currentDate.toString("yyyy-MM-dd"));

    QFile logFile(qstrFileName);
    if(!logFile.open(QIODevice::Append | QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug()<<"open log File failed";
        return false;
    }
    QString qstrLog = QString("%1 %2: [%3] %4").arg(currentDate.toString("yyyy-MM-dd")).arg(currentTime.toString("hh:mm:ss zzz")).arg(PartName).arg(LogContent);

    QTextStream qStreamLog(&logFile);
    qStreamLog<<qstrLog<<endl;
    qDebug()<<qstrLog;

    logFile.close();
    return true;
}

GLogModel *GLogModel::GetInstant()
{
    QMutexLocker locker(&g_mutex);

    static GLogModel pInstant;
    return &pInstant;
}
