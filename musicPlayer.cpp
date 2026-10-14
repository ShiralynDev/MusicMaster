#include "musicPlayer.hpp"

void MusicPlayer::playFile(const QString& file) {
    player->setSource(QUrl::fromLocalFile(file));
    player->play();
}