#ifndef GLOGMODEL_H
#define GLOGMODEL_H

#include <QObject>
#include<QMutex>

class GLogModel : public QObject
{
    Q_OBJECT
private:
    explicit GLogModel(QObject *parent = 0);
    ~GLogModel();

public:

    bool WriteLog(const QString& PartName, const QString& LogContent);
private:
    QMutex m_mutexLog;

public:
    static GLogModel* GetInstant();
};



#endif // GLOGMODEL_H
