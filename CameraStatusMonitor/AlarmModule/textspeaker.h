#ifndef TEXTSPEAKER_H
#define TEXTSPEAKER_H

#include <QObject>
#include<QMediaPlayer>

class TextSpeaker : public QObject
{
    Q_OBJECT
public:
    explicit TextSpeaker(QObject *parent = nullptr);
    ~TextSpeaker();

signals:

public slots:
    void SetVocale(const QString name);
    void playMusic(const QString fileName);
    void PlayText(const QString text);
    void PlayTextByIndex(int iType);
private:
    void ReadConfig();
    void WriteConfig();
    void PrintInfo();

private:
    QTextToSpeech m_speecher;
    QMediaPlayer m_audioPlayer;
    QMap< QString, QString> m_txtMap;
};

#endif // TEXTSPEAKER_H
