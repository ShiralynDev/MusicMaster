#pragma once

#include "../musicUtils/musicFinder/musicFinder.hpp"
#include <QFrame>
#include <QString>
#include <qnamespace.h>

namespace Ui {
    class SongEntry;
}

class SongEntry : public QFrame {
    Q_OBJECT

public:
    explicit SongEntry(const Song* song, const Album* album, QWidget* parent = nullptr);

    ~SongEntry();

signals:
    void onPress(const Song* song, const Album* album, const Qt::MouseButton button);

protected:
    void mousePressEvent(QMouseEvent* event) override;

private:
    Ui::SongEntry* ui;
    const Song* song;
    const Album* album;
};
