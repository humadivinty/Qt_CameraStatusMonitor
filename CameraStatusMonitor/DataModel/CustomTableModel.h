#ifndef CAMSTATUSMODEL
#define CAMSTATUSMODEL
#include<QStandardItemModel>

class CustomTableModel : public QStandardItemModel
{
    Q_OBJECT
public:
    explicit CustomTableModel(QObject* parent = 0 );

public:
    //QVariant data(const QModelIndex &index, int role) const;

public:
    void removeAll();
    void removeRowFromQString(int iColumn, const QString strText);
    void InsertRowFromQStringList(const QStringList strList);
    void UpdataRowFromQStringList(int iRow, const QStringList strList);
    void UpdataTextFromQString(int iRow, int iColumn, const QString strText);

    int FindRowFromQString(int iColumn, const QString strText);
    int GetRowCount();
    int GetColCount();
    QString FindColTextFromRow(int iRow, int iColumn);
};

#endif // CAMSTATUSMODEL
