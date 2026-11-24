#pragma once

#include "../../musicUtils/musicFinder/musicFinder.hpp"
#include "../albumcover/albumcover.hpp"
#include <QWidget>

namespace Ui {
    class LibraryPage;
}

class LibraryPage : public QWidget {
    Q_OBJECT

public:
    explicit LibraryPage(QWidget* parent = nullptr, MusicFinder* musicFinder = nullptr);
    void rebuild();
    ~LibraryPage();

    QVector<AlbumCover*> albumCards;

signals:
    void albumPressed(const Album* album, const Qt::MouseButton button);

protected:
    void resizeEvent(QResizeEvent* event) override;

private:
    Ui::LibraryPage* ui;
    MusicFinder* musicFinder;
};
