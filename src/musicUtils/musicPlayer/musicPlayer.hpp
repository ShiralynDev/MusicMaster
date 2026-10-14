#pragma once

#include <QAudioOutput>
#include <QMediaPlayer>
#include <QUrl>

class MusicPlayer {
private:
    QAudioOutput* audioOutput;

public:
    QMediaPlayer* player;

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
};