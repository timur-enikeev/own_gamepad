#include "soundsignals.h"
#include <QUrl>
#include <QCoreApplication>
#include <QDebug>
SoundSignals::SoundSignals(QObject *parent) : QObject(parent), mPlayer(this, QMediaPlayer::LowLatency)
{
    startSound.setSource(QUrl::fromLocalFile(QCoreApplication::applicationDirPath()+"/sounds/start.wav"));
    resetSound.setSource(QUrl::fromLocalFile(QCoreApplication::applicationDirPath()+"/sounds/reset.wav"));
    startSound.setLoopCount(1);
    resetSound.setLoopCount(1);
    playlist.setPlaybackMode(QMediaPlaylist::CurrentItemOnce);
    playlist.setPlaybackMode(QMediaPlaylist::Sequential);
    addToPlaylist("answer");
    addToPlaylist("answers-player");
    addToPlaylist("false-start");
    resetReminderPlaylist.setPlaybackMode(QMediaPlaylist::Loop);
    resetReminderPlaylist.addMedia(QUrl::fromLocalFile(QCoreApplication::applicationDirPath()+"/sounds/reset-reminder.wav"));
//    mPlayer.setAudioRole(QAudio::GameRole);
    mPlayer.setPlaylist(&currentPlaylist);
    connect(&currentPlaylist, &QMediaPlaylist::currentIndexChanged, [this](int i){if (i==-1) this->currentPlaylist.clear();});
}

void SoundSignals::addPlayerNumberVoices(int numberOfPlayers)
{
    for (int i = 1; i <= numberOfPlayers; i++)
        addToPlaylist(QString::number(i));
}

void SoundSignals::addToPlaylist(QString name)
{
    if (!sounds.contains(name))
    {
        bool success = playlist.addMedia(QUrl::fromLocalFile(QCoreApplication::applicationDirPath()+"/sounds/"+ name +".wav"));
        if (success)
        {
            sounds[name] = playlist.mediaCount() - 1;
        }
    }
}

void SoundSignals::answer(int player, bool shortForm)
{
    if (player != -1)
    {
        QString playerString = QString::number(player+1);
        if (sounds.contains(playerString))
        {
            //        playlist.setCurrentIndex(sounds[playerString]);
            if (!shortForm)
                currentPlaylist.addMedia(playlist.media(sounds["answers-player"]));
            currentPlaylist.addMedia(playlist.media(sounds[playerString]));
        }
        else
        {
            currentPlaylist.addMedia(playlist.media(sounds["answer"]));
        }
    }
    else {

        currentPlaylist.addMedia(playlist.media(sounds["answer"]));}
    mPlayer.play();
}


void SoundSignals::falseStart(int player)
{
    QString playerString = QString::number(player+1);
    if (sounds.contains(playerString))
    {
        //        playlist.setCurrentIndex(sounds[playerString]);
        currentPlaylist.addMedia(playlist.media(sounds["false-start"]));
        currentPlaylist.addMedia(playlist.media(sounds[playerString]));
        mPlayer.play();
    }
/*    else
    {
        currentPlaylist.addMedia(playlist.media(sounds["answer"]));
    }
    mPlayer.play();*/
}

void SoundSignals::roundReset(){
    currentPlaylist.clear();
    resetSound.play();
}
void SoundSignals::roundStart(){
    currentPlaylist.clear();
    startSound.play();
}

void SoundSignals::resetReminder(bool state)
{
    if (state)
    {
        mPlayer.stop();
        mPlayer.setPlaylist(&resetReminderPlaylist);
        mPlayer.play();
    }
    else
    {
        mPlayer.stop();
        mPlayer.setPlaylist(&currentPlaylist);
    }
}

void SoundSignals::changeVolume(int volume)
{
    qreal realVolume = volume/100.0;
    resetSound.setVolume(realVolume);
    startSound.setVolume(realVolume);
    mPlayer.setVolume(volume);
}
