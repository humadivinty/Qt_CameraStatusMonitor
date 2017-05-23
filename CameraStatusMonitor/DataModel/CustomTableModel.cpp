#include"DataModel/CustomTableModel.h"
#include<QDebug>

CustomTableModel::CustomTableModel(QObject *parent)
    :QStandardItemModel(parent)
{

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

//���������
void CustomTableModel::removeAll()
{
    this->removeRows(0, this->rowCount());
}
//��ָ�����ı������
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
        //�����ĵ���˵ ������model���ݻٵ�ʱ������item��Ҳ�ᱻ�ݻ٣���������new����֮�����˹�ȥdelete�ͷţ�
        QStandardItem* pItem = new QStandardItem();
        pItem->setTextAlignment(Qt::AlignCenter);
        pItem->setText(strList.at(i));
        iTempList.append(pItem);
    }
    this->appendRow(iTempList);
}

// ���ı��б����ĳ��
void CustomTableModel::UpdataRowFromQStringList(int iRow, const QStringList strList)
{
    if(iRow < 0 || strList.isEmpty())
        return;

    for(int i = 0; i < strList.count(); i ++)
    {
        this->item(iRow, i)->setText(strList.at(i));
    }
}

// ���ı�����ĳ���������
void CustomTableModel::UpdataTextFromQString(int iRow, int iColumn, const QString strText)
{
    if (iRow < 0 || iColumn < 0) {
        return ;
    }
    this->item(iRow, iColumn)->setText(strText);
}

int CustomTableModel::FindRowFromQString(int iColumn, const QString strText)
{
    if (iColumn < 0) {
        return -1;
    }
    QString qstrValue ;
    for (int i = 0; i < this->rowCount(); i++)
    {
        qstrValue = this->item(i, iColumn)->text();
        if (qstrValue == strText) {
            return i;
        }
    }

    return -1;
}
