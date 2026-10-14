#include "library.hpp"
#include "../../musicUtils/musicFinder/musicFinder.hpp"
#include "../albumcover/albumcover.hpp"
#include "ui_library.h"

#include <QHBoxLayout>
#include <QResizeEvent>
#include <QTimer>
#include <QVBoxLayout>
#include <QWidget>
#include <qcontainerfwd.h>

LibraryPage::LibraryPage(QWidget* parent, MusicFinder* musicFinder)
  : QWidget(parent)
  , ui(new Ui::LibraryPage)
  , musicFinder(musicFinder) {
    ui->setupUi(this);

    ui->libraryLayout->setSpacing(12);
    ui->libraryLayout->setContentsMargins(16, 16, 16, 16);
}

void LibraryPage::rebuildGrid() {
    constexpr int cardWidth = 140;
    constexpr int cardHeight = 190;
    constexpr int spacing = 12;

    int availableWidth = ui->scrollArea->viewport()->width();
    availableWidth -= ui->libraryLayout->contentsMargins().left();
    availableWidth -= ui->libraryLayout->contentsMargins().right();

    int columns = qMax(1, (availableWidth + spacing) / (cardWidth + spacing));
    QVector<Album>* albums = musicFinder->getAlbums();

    while (ui->libraryLayout->count() > 0) {
        QLayoutItem* item = ui->libraryLayout->takeAt(0);

        if (QWidget* widget = item->widget()) {
            widget->setParent(nullptr);
        }

        delete item;
    }

    QWidget* row = nullptr;
    QHBoxLayout* rowLayout = nullptr;

    for (int i = 0; i < albums->length(); ++i) {

        if (i % columns == 0) {
            row = new QWidget(ui->scrollAreaWidgetContents);
            row->setFixedHeight(cardHeight);

            rowLayout = new QHBoxLayout(row);
            rowLayout->setContentsMargins(0, 0, 0, 0);
            rowLayout->setSpacing(spacing);
            rowLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);

            ui->libraryLayout->addWidget(row);
        }

        AlbumCover* card;
        if (i < albumCards.size()) {
            card = albumCards.at(i);
            card->setParent(row);
        } else {
            const Album& album = albums->at(i);
            card = new AlbumCover(&album, row);
            connect(card, &AlbumCover::onPress, this, &LibraryPage::albumPressed);
            card->setFixedSize(cardWidth, cardHeight);
            albumCards.append(card);
        }

        rowLayout->addWidget(card);
        card->show();
    }

    ui->libraryLayout->addStretch();
}

void LibraryPage::resizeEvent(QResizeEvent* event) {
    QWidget::resizeEvent(event);
    QTimer::singleShot(0, this, [this]() { rebuildGrid(); });
}

LibraryPage::~LibraryPage() { delete ui; }
