#pragma once

#include "albumcover.h"
#include "musicFinder.hpp"
#include <QWidget>

namespace Ui {
    class LibraryPage;
}

class LibraryPage : public QWidget {
    Q_OBJECT

public:
    explicit LibraryPage(QWidget* parent = nullptr, MusicFinder* musicFinder = nullptr);
    ~LibraryPage();

    QVector<AlbumCover*> albumCards;

protected:
    void resizeEvent(QResizeEvent* event) override;

private:
    void rebuildGrid();

    Ui::LibraryPage* ui;
    MusicFinder* musicFinder;
};
