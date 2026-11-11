#pragma once
#include <QMouseEvent>
#include <QSlider>

class ClickableSlider : public QSlider {
    Q_OBJECT

public:
    using QSlider::QSlider;

protected:
    void mousePressEvent(QMouseEvent* event) override {
        if (event->button() == Qt::LeftButton) {
            int value = minimum() + (maximum() - minimum()) * event->position().x() / width();
            setValue(value);
            emit sliderMoved(value);
        }

        QSlider::mousePressEvent(event);
    }
};