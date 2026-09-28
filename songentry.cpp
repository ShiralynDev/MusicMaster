#include "songentry.h"
#include "ui_songentry.h"

SongEntry::SongEntry(const Song* song, QWidget* parent)
  : QFrame(parent)
  , song(song)
  , ui(new Ui::SongEntry) {
    ui->setupUi(this);

    setFixedSize(140, 140);

    ui->index->setText(QString::number(song->songInfo.trackNumber));
    ui->songName->setText(song->songInfo.title);
    ui->artist->setText(song->songInfo.artist);
    ui->duration->setText(QString::number(song->songInfo.duration));
}

SongEntry::~SongEntry() { delete ui; }

void SongEntry::mousePressEvent(QMouseEvent* event) {
    // emit onPress(album);

    QFrame::mousePressEvent(event);
}