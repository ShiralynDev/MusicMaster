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
    const Song* songCurrentlyPlaying = nullptr;
    Album albumUsedForQueue;
    std::vector<Song> songQueue;
    int queueIndex = 0;
    bool changingSong = false;

    MusicPlayer(QObject* parent = nullptr)
      : player(new QMediaPlayer())
      , audioOutput(new QAudioOutput()) {
        player->setAudioOutput(audioOutput);

        connect(player, &QMediaPlayer::playbackStateChanged, this, [this](QMediaPlayer::PlaybackState state) {
            if (state == QMediaPlayer::StoppedState && !changingSong) {
                if (!songQueue.empty() && queueIndex <= songQueue.size())
                    playNextInQueue();
                else
                    songCurrentlyPlaying = nullptr;
            }
        });
    }

    ~MusicPlayer() {
        delete player;
        delete audioOutput;
    }

    void playSong(const Song* song, int position = 0);
    void addToQueue(Song song);
    void playNextInQueue();
    void playLastInQueue();
    void playQueue();
};