#pragma once

#include "../../musicUtils/musicFinder/musicFinder.hpp"
#include <QFrame>
#include <QString>

namespace Ui {
    class AlbumCover;
}

class AlbumCover : public QFrame {
    Q_OBJECT

public:
    explicit AlbumCover(const Album* album, QWidget* parent = nullptr);

    ~AlbumCover();

signals:
    void onPress(const Album* album);

protected:
    void mousePressEvent(QMouseEvent* event) override;

private:
    Ui::AlbumCover* ui;
    const Album* album;
};
