#include "statusbar.hpp"
#include "ui_statusbar.h"
#include <QEasingCurve>
#include <QMediaMetaData>
#include <QPropertyAnimation>
#include <qdatetime.h>
#include <qevent.h>
#include <qicon.h>
#include <qmediametadata.h>
#include <qmediaplayer.h>
#include <taglib/fileref.h>
#include <taglib/flacfile.h>
#include <taglib/flacpicture.h>

StatusBar::StatusBar(QWidget* parent, MusicPlayer* musicPlayer)
  : QFrame(parent)
  , musicPlayer(musicPlayer)
  , ui(new Ui::StatusBar) {
    ui->setupUi(this);

    connect(musicPlayer->player, &QMediaPlayer::durationChanged, this, [this](qint64 duration) {
        ui->seekSlider->setRange(0, static_cast<int>(duration));
    });

    connect(musicPlayer->player, &QMediaPlayer::positionChanged, this, [this](qint64 position) {
        if (!ui->seekSlider->isSliderDown()) {
            ui->seekSlider->setValue(static_cast<int>(position));
        }
    });

    connect(ui->seekSlider, &QSlider::sliderMoved, this, [this](int position) {
        this->musicPlayer->player->setPosition(position);
    });

    connect(musicPlayer->player, &QMediaPlayer::playbackStateChanged, this, [this]() {
        switch (this->musicPlayer->player->playbackState()) {
            case QMediaPlayer::StoppedState:
                this->animatedHide(); // [taskig] it currently hides when u press a new song, fix this...
                break;
            case QMediaPlayer::PlayingState:
                ui->togglePlaying->setIcon(QIcon::fromTheme("media-playback-pause"));
                break;
            case QMediaPlayer::PausedState:
                ui->togglePlaying->setIcon(QIcon::fromTheme("media-playback-start"));
                break;
        }
    });

    connect(ui->togglePlaying, &QPushButton::pressed, this, [this]() {
        if (this->musicPlayer->player->isPlaying())
            this->musicPlayer->player->pause();
        else
            this->musicPlayer->player->play();
    });

    connect(ui->volume, &QSlider::valueChanged, this, [this](int position) {
        this->musicPlayer->player->audioOutput()->setVolume(position / 100.0);
    });
}

StatusBar::~StatusBar() { delete ui; }

void StatusBar::mousePressEvent(QMouseEvent* event) {

    QFrame::mousePressEvent(event);
}

QImage StatusBar::getAlbumArt(const QString& path) {
    TagLib::FLAC::File file(path.toUtf8().constData());

    if (!file.isValid())
        return {};

    auto pictures = file.pictureList();
    const auto* picture = pictures.front();

    QByteArray data(picture->data().data(), picture->data().size());
    QImage image;
    image.loadFromData(data);

    return image;
}

void StatusBar::rebuild() {
    // stupid shit, fuck QT for not providing the metadata cover image like it should, FUCKING QT FOR NOT PROVIDING ANY METADATA EVEN THO FFMPEG READS IT! FFMPEG more like FF MPREG
    TagLib::FLAC::File file(musicPlayer->songCurrentlyPlaying->filePath.toUtf8().constData());
    TagLib::Tag* tag = file.tag();

    auto pictures = file.pictureList();
    const auto* picture = pictures.front();

    QByteArray albumArtData(picture->data().data(), picture->data().size());
    QImage albumArt;
    albumArt.loadFromData(albumArtData);

    if (!albumArt.isNull()) {
        ui->cover->setPixmap(
          QPixmap::fromImage(albumArt).scaled(
            ui->cover->size(),
            Qt::KeepAspectRatio,
            Qt::SmoothTransformation));
    }

    ui->songName->setText(QString::fromStdString(tag->title().to8Bit(true)));
    ui->artist->setText(QString::fromStdString(tag->artist().to8Bit(true)));
}

void StatusBar::animatedHide() {
    QPropertyAnimation* animation = new QPropertyAnimation(this, "pos");
    animation->setDuration(500);
    animation->setStartValue(this->pos());
    animation->setEndValue(this->pos() + QPoint(0, this->height()));
    animation->setEasingCurve(QEasingCurve::InCubic);

    connect(animation, &QPropertyAnimation::finished, this, &QWidget::hide);
    animation->start(QAbstractAnimation::DeleteWhenStopped);
}