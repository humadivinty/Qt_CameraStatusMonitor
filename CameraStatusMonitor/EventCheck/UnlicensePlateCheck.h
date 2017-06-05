#ifndef UNLICENSEPLATECHECK_H
#define UNLICENSEPLATECHECK_H
#include<QMutex>

#include"EventCheck/BaseEventCheck.h"
#include"DataModel/CustomTableModel.h"
#include"AlarmModule/AlarmModule.h"


class UnlicensePlateCheck : public BaseEventCheck
{
    Q_OBJECT
public:
    explicit UnlicensePlateCheck(QObject *parent = 0);

    ~UnlicensePlateCheck();
signals:

    void signal_SendAlarm(AlarmMessage);
public slots:
    void slot_StartEvent();

private:
    bool m_bExitCheck;
    int m_iRate;
    QMutex m_mutex;

    CustomTableModel* m_pDataModel;
public:
    void StopCheck();
    void SetDataModel(CustomTableModel* DataModel);
    void SetUnlicenseRate(int Rate);
};

#endif // UNLICENSEPLATECHECK_H
