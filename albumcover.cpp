#include "albumcover.h"
#include "ui_albumcover.h"

AlbumCover::AlbumCover(const QString& title, const QString& artist, const QString& coverPath, QWidget* parent)
  : QFrame(parent)
  , ui(new Ui::AlbumCover) {
    ui->setupUi(this);

    setFixedSize(140, 140);

    ui->name->setText(title);
    ui->artist->setText(artist);

    if (!coverPath.isEmpty()) {
        QPixmap pix(coverPath);
        if (!pix.isNull()) {
            ui->cover->setPixmap(pix.scaled(ui->cover->size(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
        }
    }
}

AlbumCover::~AlbumCover() { delete ui; }
