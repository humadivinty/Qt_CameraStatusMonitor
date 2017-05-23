#ifndef STATUSTCHECK_H
#define STATUSTCHECK_H

#include <QObject>
#include<QMutex>
#include"DataModel/CustomTableModel.h"
#include"EventCheck/BaseEventCheck.h"

class StatustCheck : public BaseEventCheck
{
public:
    explicit StatustCheck(QObject *parent = 0);
    ~StatustCheck();
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

#endif // STATUSTCHECK_H
