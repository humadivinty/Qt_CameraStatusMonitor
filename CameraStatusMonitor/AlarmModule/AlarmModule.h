#ifndef ALARMMODULE_H
#define ALARMMODULE_H

#include <QObject>
#include<QMap>
#include<QString>
#include <QMetaType>
#include<QMutex>

#define ALARM_EVENT_NORMAL 0        //��������
#define ALARM_EVENT_DISCONNECT_TIMEOUT 1        //���ӶϿ�
#define ALARM_EVENT_UNLICENSE    2           //���޳��ơ���������
#define ALARM_EVENT_BALCKLIST    3           //������
#define ALARM_EVENT_WHITELIST    4           //������


struct AlarmMessage
{
    int iType;          //����
    QString qstrDeviceIP;
    QString qstrContent;  //����
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
