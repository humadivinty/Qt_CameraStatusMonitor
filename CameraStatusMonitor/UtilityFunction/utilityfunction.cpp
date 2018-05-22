#include"utilityfunction.h"
#include<QFile>
#include<QTextStream>

bool GetStringListFromFile(const QString fileName, QStringList &strList)
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

    QTextStream in(&file);
    QString line = in.readLine();
    while (!line.isNull()) {
        strList.push_back(line);
        line = in.readLine();
    }
    return true;
}
