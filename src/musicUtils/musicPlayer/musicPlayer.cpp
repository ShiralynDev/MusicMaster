#include "musicPlayer.hpp"

void MusicPlayer::playFile(const QString& file) {
    player->setSource(QUrl::fromLocalFile(file));
    player->play();
}

void MusicPlayer::playSong(const Song* song, int position) {
    changingSong = true;
    songCurrentlyPlaying = song;
    playFile(song->filePath);
    player->setPosition(position);
}

void MusicPlayer::addToQueue(const Song* song) {
    songQueue.push_back(song);
}

void MusicPlayer::playNextInQueue() {
    queueIndex++;
    if (queueIndex >= songQueue.size()) {
        queueIndex--;
        return;
    }
    playSong(songQueue.at(queueIndex));
}

void MusicPlayer::playLastInQueue() {
    queueIndex--;
    if (queueIndex < 0) {
        queueIndex++;
        return;
    }
    playSong(songQueue.at(queueIndex));
}