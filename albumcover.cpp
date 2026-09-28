#include "albumcover.h"
#include "musicFinder.hpp"
#include "ui_albumcover.h"

AlbumCover::AlbumCover(const Album* album, QWidget* parent)
  : QFrame(parent)
  , album(album)
  , ui(new Ui::AlbumCover) {
    ui->setupUi(this);

    setFixedSize(140, 140);

    ui->name->setText(album->name);
    ui->artist->setText(album->artist);

    if (!album->coverPath.isEmpty()) {
        QPixmap pix(album->coverPath);
        if (!pix.isNull()) {
            ui->cover->setPixmap(pix.scaled(ui->cover->size(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
        }
    }
}

AlbumCover::~AlbumCover() { delete ui; }

void AlbumCover::mousePressEvent(QMouseEvent* event) {
    emit onPress(album);

    QFrame::mousePressEvent(event);
}