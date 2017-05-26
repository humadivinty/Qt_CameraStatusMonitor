#ifndef ALARMMODULE_H
#define ALARMMODULE_H

#include <QObject>
#include<QMap>
#include<QString>
#include <QMetaType>

#define ALARM_EVENT_NORMAL 0        //���ӶϿ�
#define ALARM_EVENT_DISCONNECT 1        //���ӶϿ�
#define ALARM_EVENT_UNLICENSE    2           //���޳��ơ���������


struct AlarmMessage
{
    int iType;          //����
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
};

#endif // ALARMMODULE_H
