#pragma once

#include "../musicUtils/musicPlayer/musicPlayer.hpp"
#include <QFrame>
#include <QString>

namespace Ui {
    class StatusBar;
}

class StatusBar : public QFrame {
    Q_OBJECT

public:
    explicit StatusBar(QWidget* parent = nullptr, MusicPlayer* musicPlayer = nullptr);
    void rebuild();
    ~StatusBar();

protected:
    void mousePressEvent(QMouseEvent* event) override;

private:
    Ui::StatusBar* ui;
    MusicPlayer* musicPlayer;
    QImage getAlbumArt(const QString& path);
};
