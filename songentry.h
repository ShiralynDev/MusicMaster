#pragma once

#include "musicFinder.hpp"
#include <QFrame>
#include <QString>

namespace Ui {
    class SongEntry;
}

class SongEntry : public QFrame {
    Q_OBJECT

public:
    explicit SongEntry(const Song* song, QWidget* parent = nullptr);

    ~SongEntry();

signals:
    // void onPress(const Album* album);

protected:
    void mousePressEvent(QMouseEvent* event) override;

private:
    Ui::SongEntry* ui;
    const Song* song;
};
