#pragma once

#include <QList>
#include <QString>
#include <qcontainerfwd.h>
#include <qmap.h>
#include <qobject.h>

struct SongInfo {
    QString title;
    QString artist;
    QString album;
    QString albumArtist;
    int trackNumber;
    int release;
    int duration; // in seconds
};

struct Song {
    QString filePath;
    QString fileName;
    SongInfo songInfo;
};

struct Album {
    QVector<Song> songs;
    QString filePath;
    QString fileName;
    QString name;
    QString artist;
    QString coverPath;
    constexpr int songCount() { return songs.length(); };
};

class MusicFinder {
private:
    QVector<Album> albums;
    int songsFound = 0;

public:
    explicit MusicFinder(QObject* parent = nullptr);
    ~MusicFinder();

    int getAlbumCount() { return albums.length(); };
    QVector<Album>* getAlbums() { return &albums; };
    Album* getAlbum(QString filePath);
    void scanForSongs();
};