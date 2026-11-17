#pragma once

#include "../musicUtils/musicPlayer/musicPlayer.hpp"
#include "src/musicUtils/musicFinder/musicFinder.hpp"
#include <QFrame>
#include <QString>

namespace Ui {
    class StatusBar;
}

class StatusBar : public QFrame {
    Q_OBJECT

public:
    explicit StatusBar(QWidget* parent = nullptr, MusicPlayer* musicPlayer = nullptr);
    ~StatusBar();

    void rebuild();
    void animatedHide();

protected:
    void mousePressEvent(QMouseEvent* event) override;

private:
    Ui::StatusBar* ui;
    MusicPlayer* musicPlayer;
    const Song* lastSong;
    QImage getAlbumArt(const QString& path);
};
