#include "mainwindow.h"
#include <QApplication>
#include <QMetaType>
#include<QTextCodec>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTextCodec *codec = QTextCodec::codecForName("GBK"); //GBK gbk
    QTextCodec::setCodecForLocale(codec);


    MainWindow w;
    w.show();

    return a.exec();
}
