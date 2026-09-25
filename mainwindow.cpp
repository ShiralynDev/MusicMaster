#include "mainwindow.h"
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

    ui->stackedWidget->addWidget(libraryPage);

    connect(ui->playerButton, &QPushButton::clicked, this, [this, libraryPage]() {
        ui->stackedWidget->setCurrentWidget(nullptr);
    });

    connect(ui->libraryButton, &QPushButton::clicked, this, [this, libraryPage, musicFinder]() {
        musicFinder->scanForSongs();
        ui->stackedWidget->setCurrentWidget(libraryPage);
    });
}

MainWindow::~MainWindow() { delete ui; }
