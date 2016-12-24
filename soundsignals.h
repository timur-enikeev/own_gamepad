#ifndef SOUNDSIGNALS_H
#define SOUNDSIGNALS_H

#include <QObject>
#include <QSoundEffect>
#include <QMediaPlayer>
#include <QMediaPlaylist>

class SoundSignals : public QObject
{
    Q_OBJECT
public:
    explicit SoundSignals(QObject *parent = 0);
    void roundReset();
    void roundStart();
    void answer(int player, bool shortForm); // numeration from 0
    void falseStart(int player);
    void addPlayerNumberVoices(int numberOfPlayers);
private:
    QSoundEffect startSound;
    QSoundEffect resetSound;
    QMediaPlayer mPlayer;
    QMediaPlaylist playlist;
    QMap<QString, int> sounds; // key is name of sound, value is index in playlist
    QMediaPlaylist currentPlaylist;
    void addToPlaylist(QString name);

signals:

public slots:
    void changeVolume(int volume); // from 0 to 100
};

#endif // SOUNDSIGNALS_H
