#include "mainwindow.h"
#include "albumlist.h"
#include "library.h"
#include "musicFinder.hpp"
#include "ui_mainwindow.h"
#include <qaction.h>

MainWindow::MainWindow(QWidget* parent)
  : QMainWindow(parent)
  , ui(new Ui::MainWindow) {
    ui->setupUi(this);

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

    connect(libraryPage, &LibraryPage::albumPressed, this, [this, albumListPage](const Album* album) {
        albumListPage->rebuildGrid(album);
        ui->stackedWidget->setCurrentWidget(albumListPage);
    });
}

MainWindow::~MainWindow() { delete ui; }
