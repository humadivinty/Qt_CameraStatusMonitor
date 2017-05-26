#ifndef STATUSTCHECK_H
#define STATUSTCHECK_H

#include <QObject>
#include<QMutex>
#include"DataModel/CustomTableModel.h"
#include"EventCheck/BaseEventCheck.h"
#include"AlarmModule/AlarmModule.h"

class StatustCheck : public BaseEventCheck
{
    Q_OBJECT
public:
    explicit StatustCheck(QObject *parent = 0);
    ~StatustCheck();
signals:
    void signal_SendAlarm(AlarmMessage);
public slots:
    void slot_StartEvent();

private:
    bool m_bExitCheck;
    int m_iCount;
    QMutex m_mutex;

    CustomTableModel* m_pDataModel;
public:
    void StopCheck();
    void SetDataModel(CustomTableModel* DataModel);
    void SetDisConCount(int Count);
};

#endif // STATUSTCHECK_H
