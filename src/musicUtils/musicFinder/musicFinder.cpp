#include "musicFinder.hpp"

#include <QDir>
#include <QDirIterator>
#include <QFileInfo>
#include <QStandardPaths>
#include <iostream>
#include <qdiriterator.h>
#include <qobject.h>
#include <qstandardpaths.h>
#include <taglib/fileref.h>
#include <taglib/tag.h>

MusicFinder::MusicFinder(QObject* parent) {}

MusicFinder::~MusicFinder() {}

QString findCoverArt(const QDir& dir) {
    static const QStringList coverNames = { "cover.jpg", "cover.jpeg", "cover.png" };

    for (const QString& name : coverNames) {
        QString candidate = dir.filePath(name);
        if (QFileInfo::exists(candidate)) {
            return candidate;
        }
    }
    return QString();
}

void MusicFinder::scanForSongs() {
    albums.clear();

    QStringList filters = { "*.mp3", "*.flac", "*.wav", "*.m4a", "*.ogg", "*.aac" };
    QDirIterator it(QStandardPaths::writableLocation(QStandardPaths::MusicLocation),
                    filters,
                    QDir::Files,
                    QDirIterator::Subdirectories | QDirIterator::FollowSymlinks);

    Album album;

    while (it.hasNext()) {
        QString path = it.next();
        QFileInfo info(path);

        try {
            TagLib::FileRef file(path.toUtf8().constData());
            TagLib::Tag* tag = file.tag(); // lacks album artist and duration

            if (album.filePath.isEmpty() || album.filePath != info.dir().path()) {
                if (!album.filePath.isEmpty()) {
                    albums.push_back(album);
                    album = Album{};
                }

                album.filePath = info.dir().path();
                album.fileName = info.dir().dirName();
                album.coverPath = findCoverArt(info.dir());
                if (!tag->album().isEmpty())
                    album.name = QString::fromStdString(tag->album().to8Bit(true));
                else
                    album.name = QString::fromStdString(tag->title().to8Bit(true));
                album.artist = album.fileName.split(" - ").at(0); // should use from album artist metadata
            }

            Song song;
            song.filePath = path;
            song.fileName = info.fileName();

            song.songInfo.artist = QString::fromStdString(tag->artist().to8Bit(true));
            song.songInfo.album = QString::fromStdString(tag->album().to8Bit(true));
            song.songInfo.title = QString::fromStdString(tag->title().to8Bit(true));
            song.songInfo.genre = QString::fromStdString(tag->genre().to8Bit(true));
            song.songInfo.release = tag->year();
            song.songInfo.trackNumber = tag->track();
            song.songInfo.duration = file.audioProperties()->lengthInSeconds();

            album.songs.push_back(song);
        } catch (...) {
            std::cout << "Error loading: " << path.toStdString();
        }
    }

    albums.push_back(album);
}

Album* MusicFinder::getAlbum(QString filePath) {
    for (auto& album : albums) {
        if (album.filePath == filePath) {
            return &album;
        }
    }
    return nullptr;
}