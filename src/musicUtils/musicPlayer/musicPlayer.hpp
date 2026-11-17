#pragma once

#include "../musicFinder/musicFinder.hpp"
#include <QAudioOutput>
#include <QMediaPlayer>
#include <QUrl>
#include <vector>

class MusicPlayer : QObject {
private:
    QAudioOutput* audioOutput;

    void playFile(const QString& file);

public:
    QMediaPlayer* player;
    const Song* songCurrentlyPlaying;
    Album albumUsedForQueue;
    std::vector<const Song*> songQueue;
    int queueIndex = 0;
    bool changingSong = false;

    MusicPlayer(QObject* parent = nullptr)
      : player(new QMediaPlayer())
      , audioOutput(new QAudioOutput()) {
        player->setAudioOutput(audioOutput);

        connect(player, &QMediaPlayer::playbackStateChanged, this, [this](QMediaPlayer::PlaybackState state) {
            if (state == QMediaPlayer::StoppedState && !songQueue.empty() && queueIndex <= songQueue.size() && !changingSong) {
                playNextInQueue();
            }
        });
    }

    ~MusicPlayer() {
        delete player;
        delete audioOutput;
    }

    void playSong(const Song* song, int position = 0);
    void addToQueue(const Song* song);
    void playNextInQueue();
    void playLastInQueue();
    void playQueue();
};