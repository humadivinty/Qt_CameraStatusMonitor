#include "textspeaker.h"
#include"LogModel/glogmodel.h"
#include"data_commomdef.h"
#include<QFile>
#include <QDomDocument>



TextSpeaker::TextSpeaker(QObject *parent) : QObject(parent)
{
    m_speecher.setLocale(QLocale("zh_CN"));

    PrintInfo();
    ReadConfig();
}

TextSpeaker::~TextSpeaker()
{
    //WriteConfig();
}

void TextSpeaker::SetVocale(const QString name)
{
    QString qstrLog;
    qstrLog = QString("SetVocale, name = %1").arg(name);
    GLogModel::GetInstant()->WriteLog("TextSpeaker",qstrLog );
    m_speecher.setLocale(QLocale(name));
}

void TextSpeaker::playMusic(const QString fileName)
{
    QString qstrLog;
    qstrLog = QString("playMusic, fileName = %1").arg(fileName);
    GLogModel::GetInstant()->WriteLog("TextSpeaker",qstrLog );
    if(fileName.isEmpty())
        return;

    m_audioPlayer.setMedia(QUrl::fromLocalFile(fileName));
    m_audioPlayer.play();
}

void TextSpeaker::PlayText(const QString text)
{
    QString qstrLog;
    qstrLog = QString("playMusic, PlayText = %1").arg(text);
    GLogModel::GetInstant()->WriteLog("TextSpeaker",qstrLog );

    m_speecher.say(text);
}

void TextSpeaker::PlayTextByIndex(int iType)
{
    QString qstrLog;
    qstrLog = QString("PlayTextByIndex, iType = %1").arg(iType);
    GLogModel::GetInstant()->WriteLog("TextSpeaker",qstrLog );

    QString qstrKey, qstrValue;
    QString qstrDefaultValue;
    switch (iType)
    {
    case 0://ALARM_EVENT_NORMAL

        break;
    case 1://ALARM_EVENT_DISCONNECT_TIMEOUT
        playMusic(POLICE_SIREN_SHORT);
        break;
    case 2: //ALARM_EVENT_UNLICENSE
        playMusic(POLICE_SIREN_SHORT);
        break;
    case 3:// ALARM_EVENT_BALCKLIST
        qstrKey = QString("BlackList");
        qstrDefaultValue = tr("Attention please, attention please,this car is in the black list.");
        qstrValue = m_txtMap.value(qstrKey, qstrDefaultValue);
        PlayText(qstrValue);
        break;
    case 4://ALARM_EVENT_WHITELIST
        qstrKey = QString("WhiteList");
        qstrDefaultValue = tr("Attention please, attention please,this car is in the white list.");
        qstrValue = m_txtMap.value(qstrKey, qstrDefaultValue);
        PlayText(qstrValue);
        break;
    default:
        break;
    }
    if(qstrValue == qstrDefaultValue)
    {
        m_txtMap.insert(qstrKey, qstrValue);
    }
}

void TextSpeaker::ReadConfig()
{
    QString qstrLog;
    QString xmlfileName(MEDIAPLAY_FILE_NAME);
    QFile file(xmlfileName);
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        qstrLog.clear();
        qstrLog = tr("Get Media List ,Cannot read file %1:\n%2.").arg(xmlfileName).arg(file.errorString());
        GLogModel::GetInstant()->WriteLog("TextSpeaker",qstrLog);
        return;
    }

    QString errorStr;
    int errorLine;
    int errorColumn;

    QDomDocument domDocument;
    if (!domDocument.setContent(&file, true, &errorStr, &errorLine,
                                &errorColumn))
    {
        qstrLog.clear();
        qstrLog = tr("Get Media List  ,Parse error at line %1, column %2:\n%3").arg(errorLine).arg(errorColumn).arg(errorStr);
        GLogModel::GetInstant()->WriteLog("TextSpeaker",qstrLog);
        return ;
    }
    file.close();

    QDomNodeList nodeList = domDocument.elementsByTagName("TextInfo");
    QDomNode tempNode;
    QString qstrCMDName, qstrCMDText;
    for(int i = 0; i< nodeList.count(); i++)
    {
        tempNode = nodeList.at(i);
        qstrCMDName = tempNode.toElement().firstChildElement("Name").text();
        qstrCMDText = tempNode.toElement().firstChildElement("text").text();

        m_txtMap.insert(qstrCMDName, qstrCMDText);
    }
}

void TextSpeaker::WriteConfig()
{
    QString qstrLog;
    QString strFileName(MEDIAPLAY_FILE_NAME);

    QDomDocument domDocument;
    QDomElement root;
    if(QFile::exists(strFileName))
    {
        //如果文件存在，则打开文件，向其添加相机信息节点
        QFile file(strFileName);
        if (!file.open(QFile::ReadOnly | QFile::Text))
        {
            qstrLog.clear();
            qstrLog = tr("Add Alarm Info ToFile ,Cannot ReadOnly file %1:\n%2.").arg(strFileName).arg(file.errorString());
            GLogModel::GetInstant()->WriteLog("TextSpeaker",qstrLog);
            return;
        }
        domDocument.setContent(&file);
        file.close();

        root = domDocument.firstChildElement("List");
    }
    else
    {
        //如果文件不存在，则先创建 xml文档 ，最后保存成文件
        QString qstrHeaderdata = "version=\"1.0\" encoding=\"GB2312\" standalone=\"yes\"";
        domDocument.appendChild( domDocument.createProcessingInstruction("xml", qstrHeaderdata) );

        root = domDocument.createElement("List");
        domDocument.appendChild(root);
    }

    QMap<QString, QString>::ConstIterator i= m_txtMap.cbegin();

    while (i != m_txtMap.cend())
    {
        QDomElement Elimemt0 = domDocument.createElement("TextInfo");

        QDomElement Elimemt1 = domDocument.createElement("Name");
        QDomText Elimemt1Txt = domDocument.createTextNode(i.key());
        Elimemt1.appendChild(Elimemt1Txt);

        QDomElement Elimemt2 = domDocument.createElement("text");
    //    QTextCodec* codec = QTextCodec::codecForLocale();
    //    QString destString = codec->toUnicode(qstrStationName);
        QDomText Elimemt2Txt = domDocument.createTextNode(i.value());
        Elimemt2.appendChild(Elimemt2Txt);

        root.appendChild(Elimemt0);
        Elimemt0.appendChild(Elimemt1);
        Elimemt0.appendChild(Elimemt2);

        ++i;
    }

    QFile file(strFileName);
    if (!file.open(QFile::WriteOnly | QFile::Text))
    {
        qstrLog.clear();
        qstrLog = tr("AddIPAddressToFile ,Cannot WriteOnly file %1:\n%2.").arg(strFileName).arg(file.errorString());
        GLogModel::GetInstant()->WriteLog("MainWindow",qstrLog);
        return;
    }

    QTextStream outFile(&file);
    domDocument.save(outFile, 4);
    file.close();
}

void TextSpeaker::PrintInfo()
{
    GLogModel::GetInstant()->WriteLog("TextSpeaker", "TextSpeaker, avaliEngines =");
    QStringList avaliEngines = m_speecher.availableEngines();
    qDebug()<< "avaliEngines";
    for(QString x: avaliEngines)
    {
        GLogModel::GetInstant()->WriteLog("TextSpeaker", x);
    }

    QVector<QLocale> Locales = m_speecher.availableLocales();
    GLogModel::GetInstant()->WriteLog("TextSpeaker", "TextSpeaker, availableLocales =");
    for(QLocale x: Locales)
    {
        GLogModel::GetInstant()->WriteLog("TextSpeaker", x.name());
    }

    QVector<QVoice>  voices= m_speecher.availableVoices();
    GLogModel::GetInstant()->WriteLog("TextSpeaker", "TextSpeaker, availableVoices =");
    for(QVoice x: voices)
    {
        GLogModel::GetInstant()->WriteLog("TextSpeaker", x.name());
    }
}
