#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>
#include <QProcess>
#include <QWheelEvent>
#include <QTimer>
#include <QMap>

#include "volumeslider.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

private slots:
    void onExit();
    void onTrayIconActivated(QSystemTrayIcon::ActivationReason reason);
    void onSliderMoved(int value);
    void hideSlider();
    void onSliderPressed();
    void onSliderReleased();
    void openSettings();
    void loadSettings();
    void toggleIconTheme();


private:
    void changeVolume(bool increase);
    void toggleMute();
    void updateVolumeState();
    void updateUi(int volume, bool isMuted);
    void setVolume(int percentage);
    void showSlider();
    void openMixerApp();
    void refreshIconCache(); // İkonları boyayıp hafızaya alacak fonksiyon
    QMap<QString, QIcon> m_iconCache; // Boyanmış ikonları tutacağımız sözlük

    QAction *m_iconThemeAction;
    bool m_isPanelLight; // True ise siyah ikon, False ise beyaz ikon çizeceğiz.
    QIcon colorizeIcon(const QString &iconPath, QColor customColor = QColor());

    int m_currentVolume = 0;
    bool m_isMuted = false;

    int m_maxVolume;
    int m_scrollStep;
    QString m_mixerCommand;

    Ui::MainWindow *ui;
    QSystemTrayIcon *m_trayIcon;
    QMenu *m_trayMenu;
    QAction *m_settingsAction;
    QAction *m_exitAction;
    QAction *m_mixerAction;

    QProcess *m_process = nullptr;
    VolumeSlider *m_sliderWidget = nullptr;
    QTimer *m_sliderHideTimer = nullptr;
    bool m_isSliderDragging;
};
#endif // MAINWINDOW_H