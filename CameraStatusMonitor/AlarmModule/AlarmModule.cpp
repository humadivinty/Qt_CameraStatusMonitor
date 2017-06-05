#include "AlarmModule.h"
#include<QDebug>

AlarmModule::AlarmModule(QObject *parent) : QObject(parent)
{
    qRegisterMetaType<AlarmMessage>("AlarmMessage");
    qRegisterMetaType<QVector<int> >("QVector<int>");
}

void AlarmModule::slot_Receive_AlarmMsessage(AlarmMessage Message)
{
    qDebug()<<"type ="<<Message.iType<<"Values ="<<Message.qstrContent;

    emit signal_Send_AlarmMessage(Message);
}

