#pragma once

#include <QFrame>
#include <QString>

namespace Ui {
    class AlbumCover;
}

class AlbumCover : public QFrame {
    Q_OBJECT

public:
    explicit AlbumCover(const QString& title, const QString& artist, const QString& coverPath, QWidget* parent = nullptr);

    ~AlbumCover();

private:
    Ui::AlbumCover* ui;
};
