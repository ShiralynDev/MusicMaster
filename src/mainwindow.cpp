#include "mainwindow.hpp"
#include "albumlist/albumlist.hpp"
#include "library/library/library.hpp"
#include "musicUtils/musicFinder/musicFinder.hpp"
#include "musicUtils/musicPlayer/musicPlayer.hpp"
#include "statusbar/statusbar.hpp"
#include "ui_mainwindow.h"
#include <QAction>
#include <QCursor>
#include <QMenu>
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

    connect(libraryPage, &LibraryPage::albumPressed, this, [this, musicPlayer, albumListPage](const Album* album, const Qt::MouseButton button) {
        if (button == Qt::MouseButton::LeftButton) {
            albumListPage->rebuildGrid(album);
            ui->stackedWidget->setCurrentWidget(albumListPage);
        } else if (button == Qt::MouseButton::RightButton) {
            QMenu menu;
            QAction* addToQueueAction = menu.addAction("Add to queue");
            QAction* addToPlaylistAction = menu.addAction("Add to playlist");
            QAction* selectedAction = menu.exec(QCursor::pos());

            if (selectedAction == addToQueueAction) {
                musicPlayer->albumUsedForQueue = *(album); // this is prob a super bad way of doing it since we are making copy of songs, please fix
                for (auto song : musicPlayer->albumUsedForQueue.songs) {
                    musicPlayer->addToQueue(song);
                }
                if (musicPlayer->player->playbackState() == QMediaPlayer::StoppedState) { // prob nicer way of writing this and should prob be handled by player
                    if (musicPlayer->songQueue.size() == album->songs.size())
                        musicPlayer->queueIndex = -1;
                    musicPlayer->queueIndex++;
                    musicPlayer->playQueue();
                }
            } else if (selectedAction == addToPlaylistAction) {
            }
        }
    });

    connect(albumListPage, &AlbumListPage::songPressed, this, [this, musicPlayer, statusBar](const Song* song, const Album* album, const Qt::MouseButton button) {
        if (button == Qt::MouseButton::LeftButton) {
            musicPlayer->songQueue.clear();
            musicPlayer->albumUsedForQueue = *(album); // this is prob a super bad way of doing it since we are making copy of songs, please fix
            for (int i = 0; i < musicPlayer->albumUsedForQueue.songs.size(); i++) {
                musicPlayer->addToQueue(musicPlayer->albumUsedForQueue.songs.at(i));
                if (&musicPlayer->albumUsedForQueue.songs.at(i) == song) {
                    musicPlayer->queueIndex = i;
                    musicPlayer->playQueue();
                }
            }
            statusBar->setVisible(true);
        } else if (button == Qt::MouseButton::RightButton) {
            QMenu menu;
            QAction* addToQueueAction = menu.addAction("Add to queue");
            QAction* addToPlaylistAction = menu.addAction("Add to playlist");
            QAction* selectedAction = menu.exec(QCursor::pos());

            if (selectedAction == addToQueueAction) {
                musicPlayer->addToQueue(*song);
                if (musicPlayer->player->playbackState() == QMediaPlayer::StoppedState) { // prob nicer way of writing this and should prob be handled by player
                    if (musicPlayer->songQueue.size() == 1)
                        musicPlayer->queueIndex = -1;
                    musicPlayer->queueIndex++;
                    musicPlayer->playQueue();
                }
            } else if (selectedAction == addToPlaylistAction) {
            }
        }
    });
}

MainWindow::~MainWindow() { delete ui; }
