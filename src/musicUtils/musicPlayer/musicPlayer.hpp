#pragma once

#include "../musicFinder/musicFinder.hpp"
#include <QAudioOutput>
#include <QMediaPlayer>
#include <QUrl>

class MusicPlayer {
private:
    QAudioOutput* audioOutput;

public:
    QMediaPlayer* player;
    const Song* songCurrentlyPlaying;

    MusicPlayer(QObject* parent = nullptr)
      : player(new QMediaPlayer())
      , audioOutput(new QAudioOutput()) {
        player->setAudioOutput(audioOutput);
    }

    ~MusicPlayer() {
        delete player;
        delete audioOutput;
    }

    void playFile(const QString& file);
    void playSong(const Song* song, int position = 0);
};