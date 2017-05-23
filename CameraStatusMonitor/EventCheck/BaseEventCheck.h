#ifndef BASEEVENTCHECK_H
#define BASEEVENTCHECK_H

#include <QObject>
#include<QThread>

class BaseEventCheck : public QObject
{
    Q_OBJECT
public:
    explicit BaseEventCheck(QObject *parent = 0);
    virtual ~BaseEventCheck();
signals:
    void signal_StartEvent();
public slots:
    virtual void slot_StartEvent() = 0;

public:
    void StartEvent();

private:
    QThread WorkerThread;
};

#endif // BASEEVENTCHECK_H
