#ifndef STATUSTCHECK_H
#define STATUSTCHECK_H

#include <QObject>
#include"DataModel/CustomTableModel.h"

class StatustCheck : public QObject
{
    Q_OBJECT
public:
    explicit StatustCheck(QObject *parent = 0);
    ~StatustCheck();
signals:

public slots:
    void StartCheck();
private:


public:
    void SetDataModel(CustomTableModel* DataModel);
};

#endif // STATUSTCHECK_H
