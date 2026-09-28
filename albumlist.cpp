#include "albumlist.h"
#include "musicFinder.hpp"
#include "songentry.h"
#include "ui_albumlist.h"

#include <QHBoxLayout>
#include <QResizeEvent>
#include <QTimer>
#include <QVBoxLayout>
#include <QWidget>
#include <qcontainerfwd.h>
#include <qlayout.h>

AlbumListPage::AlbumListPage(QWidget* parent)
  : QWidget(parent)
  , ui(new Ui::AlbumListPage) {
    ui->setupUi(this);

    auto* layout = new QVBoxLayout(ui->scrollAreaWidgetContents);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
}

bool compare(Song a, Song b) {
    return a.songInfo.trackNumber < b.songInfo.trackNumber;
}

void AlbumListPage::rebuildGrid(const Album* album) {
    constexpr int entryHeight = 60;

    if (album != nullptr) {
        lastAlbum = const_cast<Album*>(album);
        qDeleteAll(songEntrys);
        songEntrys.clear();
    }
    if (!lastAlbum)
        return;
    Album* usedAlbum = lastAlbum;

    std::sort(usedAlbum->songs.begin(), usedAlbum->songs.end(), compare);

    if (!usedAlbum->coverPath.isEmpty()) {
        QPixmap pix(usedAlbum->coverPath);
        if (!pix.isNull()) {
            ui->coverimage->setPixmap(pix.scaled(ui->coverimage->size(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
        }
    }
    ui->AlbumName->setText(usedAlbum->name);
    QFont font = ui->AlbumName->font();
    font.setPointSize(40);
    ui->AlbumName->setFont(font);
    ui->SmallDetails->setText(usedAlbum->artist + " - " + QString::number(usedAlbum->songCount()) + " songs");

    QLayout* layout = ui->scrollAreaWidgetContents->layout();
    while (layout->count() > 0) {
        QLayoutItem* item = layout->takeAt(0);

        if (QWidget* widget = item->widget()) {
            widget->setParent(nullptr);
        }

        delete item;
    }

    QWidget* row = nullptr;
    QHBoxLayout* rowLayout = nullptr;

    for (int i = 0; i < usedAlbum->songs.length(); ++i) {

        row = new QWidget(ui->scrollAreaWidgetContents);
        row->setFixedHeight(entryHeight);
        rowLayout = new QHBoxLayout(row);
        rowLayout->setContentsMargins(0, 0, 0, 0);
        rowLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
        layout->addWidget(row);

        SongEntry* entry;
        if (i < songEntrys.size()) {
            entry = songEntrys.at(i);
            entry->setParent(row);
        } else {
            const Song& song = album->songs.at(i);
            entry = new SongEntry(&song, row);
            entry->setFixedSize(500, entryHeight);
            songEntrys.append(entry);
        }

        rowLayout->addWidget(entry);
        entry->show();
    }
}

void AlbumListPage::resizeEvent(QResizeEvent* event) {
    QWidget::resizeEvent(event);

    if (!lastAlbum)
        return;

    QTimer::singleShot(0, this, [this]() { rebuildGrid(nullptr); });
}

AlbumListPage::~AlbumListPage() { delete ui; }
