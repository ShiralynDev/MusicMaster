#include "musicFinder.hpp"

#include <QDir>
#include <QDirIterator>
#include <QFileInfo>
#include <QStandardPaths>
#include <iostream>
#include <qdiriterator.h>
#include <qhashfunctions.h>
#include <qiodevicebase.h>
#include <qobject.h>
#include <qstandardpaths.h>
#include <taglib/attachedpictureframe.h>
#include <taglib/fileref.h>
#include <taglib/flacfile.h>
#include <taglib/flacpicture.h>
#include <taglib/mpegfile.h>
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

QString findEmbeddedCoverArt(const QString& path) {
    TagLib::FLAC::File file(path.toUtf8().constData());
    TagLib::List<TagLib::FLAC::Picture*> pictures = file.pictureList();

    if (pictures.isEmpty())
        return QString();

    TagLib::FLAC::Picture* usedPicture = nullptr;
    for (auto* picture : pictures) {
        if (picture->type() == TagLib::FLAC::Picture::FrontCover) {
            usedPicture = picture;
            break;
        }
    }

    if (!usedPicture)
        usedPicture = pictures.front();

    QByteArray imageData(usedPicture->data().data(), static_cast<qsizetype>(usedPicture->data().size()));

    QString extension = ".jpg";
    if (usedPicture->mimeType() == "image/png")
        extension = ".png";

    QString outputPath = QDir::temp().filePath(QString::fromStdString(file.tag()->title().to8Bit(true)) + extension);
    QFile output(outputPath);
    if (!output.open(QIODeviceBase::WriteOnly))
        return QString();
    output.write(imageData);
    output.close();

    return outputPath;
}

bool compareTrackNumbers(Song a, Song b) {
    return a.songInfo.trackNumber < b.songInfo.trackNumber;
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
            if (file.isNull())
                continue;
            TagLib::Tag* tag = file.tag();

            if (album.filePath.isEmpty() || album.filePath != info.dir().path()) {
                if (!album.filePath.isEmpty()) {
                    std::sort(album.songs.begin(), album.songs.end(), compareTrackNumbers);
                    albums.push_back(album);
                    album = Album{};
                }

                album.filePath = info.dir().path();
                album.fileName = info.dir().dirName();
                QString albumArtCandidate = findCoverArt(info.dir());
                if (albumArtCandidate != QString())
                    album.coverPath = albumArtCandidate;
                else
                    album.coverPath = findEmbeddedCoverArt(path);
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

    std::sort(album.songs.begin(), album.songs.end(), compareTrackNumbers);
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