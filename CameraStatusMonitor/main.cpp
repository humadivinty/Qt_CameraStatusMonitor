#include "mainwindow.h"
#include <QApplication>
#include <QMetaType>
#include<QTextCodec>
#include<QTranslator>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator translator;
    translator.load(QString("CamStatus_zh-cn"));
    a.installTranslator(&translator);

    MainWindow w;
    w.show();
	//heihei
    return a.exec();
}
