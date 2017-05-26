#ifndef ALARMMODULE_H
#define ALARMMODULE_H

#include <QObject>
#include<QMap>
#include<QString>
#include <QMetaType>

#define ALARM_EVENT_NORMAL 0        //连接断开
#define ALARM_EVENT_DISCONNECT 1        //连接断开
#define ALARM_EVENT_UNLICENSE    2           //“无车牌”数量过多


struct AlarmMessage
{
    int iType;          //类型
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
};

#endif // ALARMMODULE_H
