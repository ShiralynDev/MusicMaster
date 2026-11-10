#include "musicPlayer.hpp"

void MusicPlayer::playFile(const QString& file) {
    player->setSource(QUrl::fromLocalFile(file));
    player->play();
}

void MusicPlayer::playSong(const Song* song, int position) {
    songCurrentlyPlaying = song;
    playFile(song->filePath);
    player->setPosition(position);
}