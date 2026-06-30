#ifndef VOLUMESLIDER_H
#define VOLUMESLIDER_H

#include <QWidget>
#include <QSlider>
#include <QVBoxLayout>
#include <QFocusEvent>
#include <QLabel>

class VolumeSlider : public QWidget
{
    Q_OBJECT
public:
    explicit VolumeSlider(QWidget *parent = nullptr);
    QSlider *slider;
    void setSliderStyle(bool isOverboost);
    void setMaxVolume(int maxVol);
    QLabel *percentageLabel;
    void updatePercentage(int value);

protected:
    void focusOutEvent(QFocusEvent *event) override;

signals:
    void lostFocus(); // Odak kaybedildiğinde sinyal yollayalım

};

#endif // VOLUMESLIDER_H
