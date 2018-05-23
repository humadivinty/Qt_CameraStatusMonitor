#ifndef TTSFORMICROSOFT_H
#define TTSFORMICROSOFT_H

#include <QObject>
#include <QAxObject>
#include <QMediaPlayer>

class TTSforMicrosoft : public QObject
{
    Q_OBJECT
public:
    explicit TTSforMicrosoft(QObject *parent = 0);
    ~TTSforMicrosoft();

signals:

public slots:
    void TellStr(QString strSrc);

private:
    void SpeakStr(QString strSrc);
    void ReadConfig();
    void WriteConfig();

private:
    QAxObject* pSpeak;

    QMediaPlayer m_audioPlayer;
    QMap< QString, QString> m_txtMap;
public slots:
    void playMusic(const QString fileName);
    void PlayTextByIndex(int iType);
};

#endif // TTSFORMICROSOFT_H
