#ifndef UNLICENSEPLATECHECK_H
#define UNLICENSEPLATECHECK_H
#include<QMutex>

#include"EventCheck/BaseEventCheck.h"
#include"DataModel/CustomTableModel.h"



class UnlicensePlateCheck : public BaseEventCheck
{
public:
    explicit UnlicensePlateCheck(QObject *parent = 0);

    ~UnlicensePlateCheck();
signals:

public slots:
    void slot_StartEvent();

private:
    bool m_bExitCheck;
    QMutex m_mutex;

    CustomTableModel* m_pDataModel;
public:
    void StopCheck();
    void SetDataModel(CustomTableModel* DataModel);
};

#endif // UNLICENSEPLATECHECK_H
