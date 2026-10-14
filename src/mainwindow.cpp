#include "mainwindow.hpp"
#include "albumlist/albumlist.hpp"
#include "library/library/library.hpp"
#include "musicUtils/musicFinder/musicFinder.hpp"
#include "musicUtils/musicPlayer/musicPlayer.hpp"
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

    connect(albumListPage, &AlbumListPage::songPressed, this, [this, musicPlayer](const Song* song, const Qt::MouseButton button) {
        if (button == Qt::MouseButton::LeftButton)
            musicPlayer->playFile(song->filePath);
    });
}

MainWindow::~MainWindow() { delete ui; }
