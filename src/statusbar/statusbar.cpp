#include "statusbar.hpp"
#include "src/musicUtils/musicPlayer/musicPlayer.hpp"
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

    connect(musicPlayer->player, &QMediaPlayer::mediaStatusChanged, this, [this](QMediaPlayer::MediaStatus status) {
        if (status == QMediaPlayer::EndOfMedia && this->musicPlayer->queueIndex >= this->musicPlayer->songQueue.size() - 1)
            animatedHide();
    });

    connect(musicPlayer, &MusicPlayer::songChanged, this, [this](const Song* song) {
        rebuild(song);
        if (!isVisible())
            show();
    });

    connect(musicPlayer->player, &QMediaPlayer::playbackStateChanged, this, [this](QMediaPlayer::PlaybackState state) {
        switch (state) {
            case QMediaPlayer::StoppedState:
                break;
            case QMediaPlayer::PlayingState:
                ui->togglePlaying->setIcon(QIcon::fromTheme("media-playback-pause"));
                if (!isVisible())
                    show();
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

    connect(ui->lastSong, &QPushButton::pressed, this, [this]() {
        this->musicPlayer->playLastInQueue(); // lowk bad naming but previouse is to hard to spell
    });

    connect(ui->nextSong, &QPushButton::pressed, this, [this]() {
        this->musicPlayer->playNextInQueue();
    });

    connect(ui->volume, &QSlider::valueChanged, this, [this](int position) {
        this->musicPlayer->player->audioOutput()->setVolume(position / 100.0);
    });
}

StatusBar::~StatusBar() { delete ui; }

void StatusBar::mousePressEvent(QMouseEvent* event) {

    QFrame::mousePressEvent(event);
}

void StatusBar::rebuild(const Song* song) {
    // stupid shit, fuck QT for not providing the metadata cover image like it should, FUCKING QT FOR NOT PROVIDING ANY METADATA EVEN THO FFMPEG READS IT! FFMPEG more like FF MPREG
    TagLib::FLAC::File file(song->filePath.toUtf8().constData());
    TagLib::Tag* tag = file.tag();

    QImage albumArt; // maybe save art in song info to avoid getting it every time
    auto pictures = file.pictureList();
    if (!pictures.isEmpty()) {
        const auto* picture = pictures.front();
        QByteArray albumArtData(picture->data().data(), picture->data().size());
        albumArt.loadFromData(albumArtData);
    }

    if (!albumArt.isNull()) {
        ui->cover->setPixmap(
          QPixmap::fromImage(albumArt).scaled(
            ui->cover->size(),
            Qt::KeepAspectRatio,
            Qt::SmoothTransformation));
    } else {
        ui->cover->setText("Cover");
    }

    ui->songName->setText(song->songInfo.title);
    ui->artist->setText(song->songInfo.artist);
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