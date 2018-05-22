#ifndef ALARMMODULE_H
#define ALARMMODULE_H

#include <QObject>
#include<QMap>
#include<QString>
#include <QMetaType>
#include<QMutex>

#define ALARM_EVENT_NORMAL 0        //连接正常
#define ALARM_EVENT_DISCONNECT_TIMEOUT 1        //连接断开
#define ALARM_EVENT_UNLICENSE    2           //“无车牌”数量过多
#define ALARM_EVENT_BALCKLIST    3           //黑名单
#define ALARM_EVENT_WHITELIST    4           //黑名单


struct AlarmMessage
{
    int iType;          //类型
    QString qstrDeviceIP;
    QString qstrContent;  //内容
};

Q_DECLARE_METATYPE(AlarmMessage)

class AlarmModule : public QObject
{
    Q_OBJECT
public:
    explicit AlarmModule(QObject *parent = 0);

signals:
    void signal_Send_AlarmMessage(AlarmMessage);
public slots:
    void slot_Receive_AlarmMsessage(AlarmMessage);

private:
    QMutex m_mutexLog;
    bool WriteAlarmLog(QString fileName, QString LogText);
};

#endif // ALARMMODULE_H
