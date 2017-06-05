#include"DataModel/CustomTableModel.h"
#include<QDebug>

CustomTableModel::CustomTableModel(QObject *parent)
    :QStandardItemModel(parent)
{

}

QVariant CustomTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role == Qt::DisplayRole)
    {
        if(orientation == Qt::Horizontal)
        {
            switch (section)
            {
//            case 0:
//                return QString::fromLocal8Bit("设备名称");
//                break;
            case 0:
                //return QString::fromLocal8Bit("设备IP");
                return tr("Device IP Address");
                break;
            case 1:
                return tr("Device Connect Status");
                break;
            case 2:
                return tr("Rates of Unlicense");
                break;
            default:
                break;
            }
        }
    }
    return QVariant();
}


//QVariant CustomTableModel::data(const QModelIndex &index, int role) const
//{
//    int row = index.row();
//    int col = index.column();
//    // generate a log message when this method gets called

//    //qDebug() << QString("row %1, col%2, role %3").arg(row).arg(col).arg(role);
////    QModelIndex iIndex = this->index(row, col);
////    QString strData = iIndex.data().toString();
////    qDebug()<<QString("%1").arg(strData);

//    switch(role){
//        case Qt::FontRole:
//            if ( col == 2) //change font only for cell(0,0)
//            {
//                QFont boldFont;
//                boldFont.setBold(true);

//                return boldFont;
//            }
//    case Qt::TextColorRole:
//        if(col == 2)
//        {
//            if(index.data().toString() == QString("DisConnect"))
//            {
//                QBrush redFontColor(Qt::red);
//                return redFontColor;
//            }
//        }
////    case Qt::DisplayRole:
////            return QString("1");
//    }
//    return QVariant();
//}

//清除所有行
void CustomTableModel::removeAll()
{
    this->removeRows(0, this->rowCount());
}
//按指定列文本清除行
void CustomTableModel::removeRowFromQString(int iColumn, const QString strText)
{
    QList<int> listDelet;
    for(int i = 0; i < this->rowCount(); i++)
    {
        QModelIndex iIndex = this->index(i, iColumn);
        QString strData = iIndex.data().toString();
        if(strText == strData)
        {
            listDelet.append(i);
        }

    }

    int iVernier = 0;
    while(!listDelet.isEmpty())
    {
        this->removeRow(listDelet.takeFirst() - iVernier);
        iVernier++;
    }
}

void CustomTableModel::InsertRowFromQStringList(const QStringList strList)
{
    if(strList.isEmpty())
        return;

    QList<QStandardItem*> iTempList;
    for(int i = 0; i < strList.count(); i++)
    {
        //由于文档中说 在整个model被摧毁的时候，它的item项也会被摧毁，所以这里new出来之后不用人工去delete释放？
        QStandardItem* pItem = new QStandardItem();
        pItem->setTextAlignment(Qt::AlignCenter);
        pItem->setText(strList.at(i));
        iTempList.append(pItem);
    }
    beginResetModel();
    this->appendRow(iTempList);
    endResetModel();
}

// 按文本列表更新某行
void CustomTableModel::UpdataRowFromQStringList(int iRow, const QStringList strList)
{
    if(iRow < 0 || strList.isEmpty())
        return;

    beginResetModel();
    for(int i = 0; i < strList.count(); i ++)
    {
        this->item(iRow, i)->setText(strList.at(i));
    }
    endResetModel();
}

// 按文本更新某个格的内容
void CustomTableModel::UpdataTextFromQString(int iRow, int iColumn, const QString strText)
{
    if (iRow < 0 || iColumn < 0) {
        return ;
    }
    beginResetModel();
    this->item(iRow, iColumn)->setText(strText);
    endResetModel();
}

//查找列值符合的对应行
int CustomTableModel::FindRowFromQString(int iColumn, const QString strText)
{
    if (iColumn < 0) {
        return -1;
    }
    QString qstrValue ;
    for (int i = 0; i < this->rowCount(); i++)
    {
        //qstrValue = this->item(i, iColumn)->text();
        qstrValue = this->index(i, iColumn).data().toString();
        if (qstrValue == strText) {
            return i;
        }
    }

    return -1;
}

int CustomTableModel::GetRowCount()
{
    return this->rowCount();
}

int CustomTableModel::GetColCount()
{
    return this->columnCount();
}

//查找指定行列的值
QString CustomTableModel::FindColTextFromRow(int iRow, int iColumn)
{
    int rowCount = this->rowCount();
    int columnCount = this->columnCount();

    if(iRow > rowCount || iColumn > columnCount)
        return QString("");

    QString qstrValue;
    qstrValue = this->index(iRow, iColumn).data().toString();
    //qstrValue = this->item(iRow, iColumn)->text();    //这样使用会崩溃
    return qstrValue;
}
