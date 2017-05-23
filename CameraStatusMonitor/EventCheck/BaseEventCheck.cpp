#include "BaseEventCheck.h"
#include"LogModel/glogmodel.h"

BaseEventCheck::BaseEventCheck(QObject *parent) : QObject(parent)
{
    this->moveToThread(&WorkerThread);
    connect(this, signal_StartEvent, this, slot_StartEvent);
    WorkerThread.start();
}

BaseEventCheck::~BaseEventCheck()
{
    GLogModel::GetInstant()->WriteLog("BaseEventCheck", "Quit BaseEventCheck begin ...");

    disconnect(this, signal_StartEvent, this, slot_StartEvent);

    WorkerThread.quit();
    WorkerThread.wait();

    GLogModel::GetInstant()->WriteLog("BaseEventCheck", "Quit BaseEventCheck finish.");
}

void BaseEventCheck::StartEvent()
{
    emit signal_StartEvent();
}

