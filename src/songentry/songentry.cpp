#include "songentry.hpp"
#include "ui_songentry.h"
#include <qdatetime.h>
#include <qevent.h>

SongEntry::SongEntry(const Song* song, QWidget* parent)
  : QFrame(parent)
  , song(song)
  , ui(new Ui::SongEntry) {
    ui->setupUi(this);

    ui->index->setText(QString::number(song->songInfo.trackNumber));
    ui->songName->setText(song->songInfo.title);
    ui->artist->setText(song->songInfo.artist);
    ui->duration->setText(QTime(0, 0).addSecs(song->songInfo.duration).toString("m:ss"));
}

SongEntry::~SongEntry() { delete ui; }

void SongEntry::mousePressEvent(QMouseEvent* event) {
    emit onPress(song, event->button());

    QFrame::mousePressEvent(event);
}