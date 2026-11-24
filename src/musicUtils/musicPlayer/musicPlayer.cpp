#include "musicPlayer.hpp"

void MusicPlayer::playFile(const QString& file) {
    player->setSource(QUrl::fromLocalFile(file));
    player->play();
}

void MusicPlayer::playSong(const Song* song, int position) {
    changingSong = true;
    player->stop();
    songCurrentlyPlaying = song;
    emit songChanged(song);
    playFile(songCurrentlyPlaying->filePath);
    player->setPosition(position); // not used anymore I think, maybe remove it
    changingSong = false;
}

void MusicPlayer::addToQueue(Song song) {
    songQueue.push_back(song);
}

void MusicPlayer::playNextInQueue() {
    queueIndex++;
    if (queueIndex >= songQueue.size()) {
        queueIndex--;
        return;
    }
    playSong(&songQueue.at(queueIndex));
}

void MusicPlayer::playLastInQueue() {
    queueIndex--;
    if (queueIndex < 0) {
        queueIndex++;
        return;
    }
    playSong(&songQueue.at(queueIndex));
}

void MusicPlayer::playQueue() {
    playSong(&songQueue.at(queueIndex));
}