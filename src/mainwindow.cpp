#include "mainwindow.hpp"
#include "albumlist/albumlist.hpp"
#include "library/library/library.hpp"
#include "musicUtils/musicFinder/musicFinder.hpp"
#include "musicUtils/musicPlayer/musicPlayer.hpp"
#include "statusbar/statusbar.hpp"
#include "ui_mainwindow.h"
#include <qaction.h>
#include <qnamespace.h>
#include <qslider.h>

MainWindow::MainWindow(QWidget* parent)
  : QMainWindow(parent)
  , ui(new Ui::MainWindow) {
    ui->setupUi(this);

    MusicPlayer* musicPlayer = new MusicPlayer(this);
    MusicFinder* musicFinder = new MusicFinder(this);
    musicFinder->scanForSongs();
    LibraryPage* libraryPage = new LibraryPage(this, musicFinder);
    AlbumListPage* albumListPage = new AlbumListPage(this);
    StatusBar* statusBar = new StatusBar(this, musicPlayer);

    auto* layout = qobject_cast<QVBoxLayout*>(ui->centralwidget->layout());
    if (!layout) {
        layout = new QVBoxLayout(ui->centralwidget);
    }
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addWidget(ui->stackedWidget, 1);
    layout->addWidget(statusBar, 0);
    statusBar->setVisible(musicPlayer->player->hasAudio());

    ui->stackedWidget->addWidget(libraryPage);
    ui->stackedWidget->addWidget(albumListPage);

    ui->stackedWidget->setCurrentWidget(libraryPage);

    connect(ui->playerButton, &QPushButton::clicked, this, [this, libraryPage]() {
        ui->stackedWidget->setCurrentWidget(nullptr);
    });

    connect(ui->libraryButton, &QPushButton::clicked, this, [this, libraryPage, musicFinder]() {
        musicFinder->scanForSongs();
        ui->stackedWidget->setCurrentWidget(libraryPage);
    });

    connect(libraryPage, &LibraryPage::albumPressed, this, [this, albumListPage](const Album* album, const Qt::MouseButton button) {
        if (button == Qt::MouseButton::LeftButton) {
            albumListPage->rebuildGrid(album);
            ui->stackedWidget->setCurrentWidget(albumListPage);
        }
    });

    connect(albumListPage, &AlbumListPage::songPressed, this, [this, musicPlayer, statusBar](const Song* song, const Album* album, const Qt::MouseButton button) {
        if (button == Qt::MouseButton::LeftButton) {
            musicPlayer->songQueue.clear();
            musicPlayer->albumUsedForQueue = *(album); // this is prob a super bad way of doing it since we are making copy of songs, please fix
            for (int i = 0; i < musicPlayer->albumUsedForQueue.songs.size(); i++) {
                musicPlayer->addToQueue(&musicPlayer->albumUsedForQueue.songs.at(i));
                if (&musicPlayer->albumUsedForQueue.songs.at(i) == song) {
                    musicPlayer->playSong(song);
                    musicPlayer->queueIndex = i;
                }
            }
            statusBar->rebuild();
            statusBar->setVisible(true);
        }
    });
}

MainWindow::~MainWindow() { delete ui; }
