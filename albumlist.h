#pragma once

#include "musicFinder.hpp"
#include "songentry.h"
#include <QWidget>

namespace Ui {
    class AlbumListPage;
}

class AlbumListPage : public QWidget {
    Q_OBJECT

public:
    explicit AlbumListPage(QWidget* parent = nullptr);
    void rebuildGrid(const Album* album = nullptr);
    ~AlbumListPage();

signals:
    void songPressed(const Song* song);

protected:
    void resizeEvent(QResizeEvent* event) override;

private:
    Album* lastAlbum = nullptr;
    QVector<SongEntry*> songEntrys;

    Ui::AlbumListPage* ui;
};
