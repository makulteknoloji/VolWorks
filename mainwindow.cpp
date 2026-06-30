#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QIcon>
#include <QApplication>
#include <QProcess>
#include "volumeslider.h"
#include "settingsdialog.h"
#include <QSettings>
#include <QPainter>
#include <QPixmap>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    qApp->installEventFilter(this);

    m_isSliderDragging = false;

    m_trayIcon = new QSystemTrayIcon(this);
    m_trayIcon->setIcon(QIcon(":/volume-100.png"));
    m_trayIcon->setToolTip(tr("VolWorks"));

    // Ayarları oku
    QSettings settings("VolWorks", "Config");
    m_isPanelLight = settings.value("isPanelLight", false).toBool(); // Varsayılan Koyu Panel (Beyaz İkon)
    refreshIconCache();

    m_trayMenu = new QMenu(this);
    m_mixerAction = new QAction(tr("Mixer"), this);

    // YENİ EKLENEN KISIM: Checkbox'lı QAction
    m_iconThemeAction = new QAction(tr("Dark Icons (For Light Panels)"), this);
    m_iconThemeAction->setCheckable(true);
    m_iconThemeAction->setChecked(m_isPanelLight);


    m_settingsAction = new QAction(tr("Settings"), this);
    m_exitAction = new QAction(tr("Exit"), this);

    m_settingsAction = new QAction(tr("Settings"), this);
    m_exitAction = new QAction(tr("Exit"), this);

    m_trayMenu->addAction(m_mixerAction);
    m_trayMenu->addAction(m_iconThemeAction);
    m_trayMenu->addAction(m_settingsAction);
    m_trayMenu->addSeparator();
    m_trayMenu->addAction(m_exitAction);
    m_trayIcon->setContextMenu(m_trayMenu);

    m_sliderWidget = new VolumeSlider();
    m_sliderWidget->setMaxVolume(m_maxVolume); // YENİ: Başlangıç sınırını belirle
    m_sliderHideTimer = new QTimer(this);
    m_sliderHideTimer->setSingleShot(true);
    m_sliderHideTimer->setInterval(1500);
    loadSettings();

    connect(m_sliderHideTimer, &QTimer::timeout, this, &MainWindow::hideSlider);
    connect(m_sliderWidget->slider, &QSlider::valueChanged, this, &MainWindow::onSliderMoved);
    connect(m_sliderWidget, &VolumeSlider::lostFocus, this, &MainWindow::hideSlider);
    connect(m_exitAction, &QAction::triggered, this, &MainWindow::onExit);
    connect(m_trayIcon, &QSystemTrayIcon::activated, this, &MainWindow::onTrayIconActivated);
    connect(m_sliderWidget->slider, &QSlider::sliderPressed, this, &MainWindow::onSliderPressed);
    connect(m_sliderWidget->slider, &QSlider::sliderReleased, this, &MainWindow::onSliderReleased);
    connect(m_settingsAction, &QAction::triggered, this, &MainWindow::openSettings);
    connect(m_mixerAction, &QAction::triggered, this, &MainWindow::openMixerApp);
    connect(m_iconThemeAction, &QAction::triggered, this, &MainWindow::toggleIconTheme);

    m_trayIcon->show();
    // YENİ: Uygulamanın tam olarak ayağa kalkmasını ve Event Loop'un
    // başlamasını bekleyip (100 milisaniye) ondan sonra ilk okumayı yapıyoruz.
    QTimer::singleShot(100, this, &MainWindow::updateVolumeState);
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::Wheel) {
        if (m_trayIcon->geometry().contains(QCursor::pos())) {
            QWheelEvent *wheelEvent = static_cast<QWheelEvent*>(event);
            bool increase = wheelEvent->angleDelta().y() > 0;
            changeVolume(increase);
            showSlider();
            m_sliderHideTimer->start();
            return true;
        }
    }
    return QObject::eventFilter(obj, event);
}

void MainWindow::onExit()
{
    QApplication::quit();
}

void MainWindow::onTrayIconActivated(QSystemTrayIcon::ActivationReason reason)
{
    if (reason == QSystemTrayIcon::Trigger) { // Sol Tıklama
        if (m_sliderWidget->isVisible()) {
            hideSlider();
        } else {
            showSlider();
            m_sliderHideTimer->stop();
        }
    } else if (reason == QSystemTrayIcon::MiddleClick) { // Orta Tıklama
        toggleMute();
    }
}

// YENİ FONKSİYONLAR
void MainWindow::loadSettings()
{
    QSettings settings("VolWorks", "Config");
    m_maxVolume = settings.value("maxVolume", 150).toInt();
    m_scrollStep = settings.value("scrollStep", 5).toInt();
    m_mixerCommand = settings.value("mixerApp", "pavucontrol").toString();

    // Eğer slider zaten oluşturulmuşsa sınırını anında güncelle
    if (m_sliderWidget) {
        m_sliderWidget->setMaxVolume(m_maxVolume);
    }
}

void MainWindow::openSettings()
{
    SettingsDialog dlg(this);
    connect(&dlg, &SettingsDialog::settingsSaved, this, &MainWindow::loadSettings);
    dlg.exec(); // Pencereyi aç ve bekle
}

void MainWindow::changeVolume(bool increase)
{
    QString stepStr = QString::number(m_scrollStep) + "%";
    QString change = increase ? (stepStr + "+") : (stepStr + "-");

    // LIMIT AYARI: 150 ise 1.50, 100 ise 1.00 olacak şekilde ayarlar.
    QString limitStr = QString::number(m_maxVolume / 100.0, 'f', 2);

    // wpctl'ye -l (limit) parametresini de yolluyoruz
    QProcess::startDetached("wpctl", QStringList() << "set-volume" << "-l" << limitStr << "@DEFAULT_AUDIO_SINK@" << change);
    QTimer::singleShot(50, this, &MainWindow::updateVolumeState);
}


void MainWindow::setVolume(int percentage)
{
    QProcess::startDetached("wpctl", QStringList() << "set-volume" << "@DEFAULT_AUDIO_SINK@" << QString::number(percentage) + "%");
    QTimer::singleShot(50, this, &MainWindow::updateVolumeState);
}

void MainWindow::toggleMute()
{
    // EĞER sistem sessizde (mute) DEĞİLSE, ama ses 0 ise:
    if (!m_isMuted && m_currentVolume == 0) {
        // Kullanıcı ikonu çarpı gördü ve sesi açmak için orta tıkladı.
        // Sadece mute/unmute yaparsak ses 0 kalır.
        // Bunun yerine ona yardımcı olup sesi açalım (Adım miktarının 2 katı, örn: %10).
        int jumpVolume = (m_scrollStep > 0) ? (m_scrollStep * 2) : 10;
        setVolume(jumpVolume);
    }
    // EĞER normal bir durumdaysa (Ses %50'de vs.) klasik toggle yap:
    else {
        QProcess::startDetached("wpctl", QStringList() << "set-mute" << "@DEFAULT_AUDIO_SINK@" << "toggle");
        QTimer::singleShot(50, this, &MainWindow::updateVolumeState);
    }
}

void MainWindow::updateVolumeState()
{
    if (m_isSliderDragging) return;

    QProcess process;
    process.start("wpctl", QStringList() << "get-volume" << "@DEFAULT_AUDIO_SINK@");
    process.waitForFinished(500); // 300 yerine 500 ms yaptık (İlk açılışlar için biraz daha toleranslı)
    QString output = process.readAllStandardOutput();

    // GÜVENLİK AĞI: Eğer çıktı boş dönerse (Sistem hazır değilse),
    // 0 yapmak yerine 500ms sonra tekrar denemesini sağla.
    if (output.trimmed().isEmpty()) {
        QTimer::singleShot(500, this, &MainWindow::updateVolumeState);
        return;
    }

    bool isMuted = output.contains("[MUTED]");
    int volumePercent = 0;

    QString volumeStr = output.split(' ').last();
    volumeStr.remove("[MUTED]\n");
    volumeStr = volumeStr.trimmed(); // Boşlukları temizle garanti olsun
    volumePercent = qRound(volumeStr.toDouble() * 100);

    // Durumu hafızada tut (Akıllı Mute özelliği için)
    m_currentVolume = volumePercent;
    m_isMuted = isMuted;

    updateUi(volumePercent, isMuted);
}


void MainWindow::updateUi(int volume, bool isMuted)
{
    QIcon trayIcon;
    QString tooltipText = isMuted ? tr("Muted") : QString::number(volume) + "%";

    // ARTIK BOYAMA YAPMIYORUZ, HAFIZADAN ÇEKİYORUZ
    if (isMuted || volume == 0) {
        trayIcon = m_iconCache["muted"];
    } else if (volume <= 33) {
        trayIcon = m_iconCache["low"];
    } else if (volume <= 66) {
        trayIcon = m_iconCache["medium"];
    } else if (volume <= 100) {
        trayIcon = m_iconCache["high"];
    } else {
        // YENİ: Ses 100'den büyükse Turuncu ikonu göster
        trayIcon = m_iconCache["overboost"];
    }

    // Doğrudan oluşturduğumuz ikonları atıyoruz
    m_trayIcon->setIcon(trayIcon);
    m_trayIcon->setToolTip(QString(tr("Vol: %1").arg(tooltipText)));

    m_sliderWidget->slider->blockSignals(true);
    m_sliderWidget->slider->setValue(isMuted ? 0 : volume);

    m_sliderWidget->setSliderStyle(volume > 100);
    m_sliderWidget->updatePercentage(isMuted ? 0 : volume);
    m_sliderWidget->slider->blockSignals(false);
}

void MainWindow::showSlider()
{
    updateVolumeState();
    QRect trayRect = m_trayIcon->geometry();
    QSize sliderSize = m_sliderWidget->sizeHint();

    int x = trayRect.x() + (trayRect.width() - sliderSize.width()) / 2;
    int y = trayRect.y() - sliderSize.height() - 10; // Biraz daha yukarı aldık (padding)

    m_sliderWidget->move(x, y);
    m_sliderWidget->show();
    m_sliderWidget->activateWindow();
}

void MainWindow::hideSlider()
{
    m_sliderWidget->hide();
}

void MainWindow::onSliderMoved(int value)
{
    QString limitStr = QString::number(m_maxVolume / 100.0, 'f', 2);

    QProcess::startDetached("wpctl", QStringList() << "set-volume" << "-l" << limitStr << "@DEFAULT_AUDIO_SINK@" << QString::number(value) + "%");
    updateUi(value, false);
}

void MainWindow::onSliderReleased()
{
    m_isSliderDragging = false;
    updateVolumeState();
}

void MainWindow::onSliderPressed()
{
    m_isSliderDragging = true;
}

void MainWindow::openMixerApp()
{
    if (!m_mixerCommand.isEmpty()) {
        QStringList commandParts = QProcess::splitCommand(m_mixerCommand);
        if (!commandParts.isEmpty()) {
            QString program = commandParts.takeFirst();
            QProcess::startDetached(program, commandParts);
        }
    }
}

// Menüdeki seçeneğe tıklandığında çalışır
void MainWindow::toggleIconTheme()
{
    m_isPanelLight = m_iconThemeAction->isChecked();

    // Tercihi kalıcı olarak kaydet
    QSettings settings("VolWorks", "Config");
    settings.setValue("isPanelLight", m_isPanelLight);

    // YENİ: Tema değiştiği için ikonları yeni renkle tekrar boyayıp hafızaya al
    refreshIconCache();

    // İkonları anında güncellemek için sistemi tetikle
    updateVolumeState();
}

QIcon MainWindow::colorizeIcon(const QString &iconPath, QColor customColor)
{
    QPixmap pixmap(iconPath);

    if (!pixmap.isNull()) {
        QPainter painter(&pixmap);
        painter.setCompositionMode(QPainter::CompositionMode_SourceIn);

        // Eğer dışarıdan geçerli bir özel renk gönderilmişse onu kullan
        // Gönderilmemişse kullanıcının temasına göre siyah veya beyaz yap
        QColor fillColor;
        if (customColor.isValid()) {
            fillColor = customColor;
        } else {
            fillColor = m_isPanelLight ? Qt::black : Qt::white;
        }

        painter.fillRect(pixmap.rect(), fillColor);
        painter.end();
    }

    return QIcon(pixmap);
}

void MainWindow::refreshIconCache()
{
    m_iconCache.clear();
    m_iconCache.insert("muted", colorizeIcon(":/volume-muted.png"));
    m_iconCache.insert("low", colorizeIcon(":/volume-33.png"));
    m_iconCache.insert("medium", colorizeIcon(":/volume-65.png"));
    m_iconCache.insert("high", colorizeIcon(":/volume-100.png"));

    // YENİ: Overboost durumu için Slider'daki aynı turuncu renkle (#e67e22) boyanmış ekstra ikon
    m_iconCache.insert("overboost",
                       colorizeIcon(":/volume-100.png", QColor(250, 150, 55)));
}