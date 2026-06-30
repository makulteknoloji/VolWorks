#include "volumeslider.h"

VolumeSlider::VolumeSlider(QWidget *parent)
    : QWidget{parent}
{
    // Pencere özelliklerini ayarla
    setWindowFlags(Qt::FramelessWindowHint | Qt::Popup | Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_TranslucentBackground);

    // 1. PENCERE GENİŞLİĞİNİ SABİTLE
    // 60px dikey bir slider ve altındaki metin için idealdir.
    setFixedWidth(50);

    // Slider oluştur
    slider = new QSlider(Qt::Vertical, this);
    slider->setRange(0, 150);
    // Slider'ın genişliğini de sabitleyelim ki sağa sola kaçmasın
    slider->setFixedWidth(30);

    // Yüzde etiketi oluştur
    percentageLabel = new QLabel("%0", this);
    // 2. ETİKETİ VE METNİ SABİTLE
    percentageLabel->setAlignment(Qt::AlignCenter); // Metni kutu içinde ortala
    percentageLabel->setFixedWidth(45); // Kutunun genişliğini sabitle (150% sığacak kadar)

    // Şık bir stil (Metin kutusu için)
    percentageLabel->setStyleSheet(
        "color: white; "
        "font-weight: bold; "
        "font-size: 11px; "
        "background: #333; "
        "border-radius: 4px; "
        "padding: 3px 0px;" // Üst-alt 3px, sağ-sol 0
        );

    // LAYOUT AYARLARI
    QVBoxLayout *layout = new QVBoxLayout(this);

    // 3. HER ŞEYİ ORTALA
    layout->addWidget(slider, 0, Qt::AlignCenter); // Slider'ı layout içinde ortala
    layout->addSpacing(5); // Slider ile yazı arasına 5px boşluk
    layout->addWidget(percentageLabel, 0, Qt::AlignCenter); // Yazıyı layout içinde ortala

    layout->setContentsMargins(5, 10, 5, 10);
    setLayout(layout);

    setSliderStyle(false);
}


void VolumeSlider::focusOutEvent(QFocusEvent *event)
{
    // Pencere odağını kaybettiğinde (başka yere tıklandığında)
    // MainWindow'a haber vermek için sinyal yollayalım.
    emit lostFocus();
    QWidget::focusOutEvent(event);
}

// YENİ: %100'ü geçince rengi değiştiren QSS fonksiyonu
void VolumeSlider::setSliderStyle(bool isOverboost)
{
    QString baseStyle =
        "QSlider::groove:vertical {"
        "    border: 1px solid #999999;"
        "    width: 6px;"
        "    background: #444;"
        "    border-radius: 3px;"
        "    margin: 8px 0px 3px 0px;" /* YENİ EKLENDİ: Üstten ve alttan 8 piksel boşluk bırakır, yuvarlak asla kesilmez */
        "}"
        "QSlider::handle:vertical {"
        "    background: white;"
        "    border: 1px solid #5c5c5c;"
        "    height: 14px;"
        "    margin: -4px -5px;"
        "    border-radius: 7px;"
        "}";

    QString chunkStyle;
    if (isOverboost) {
        // %100 üzeri: Turuncu-Kırmızı arası tatlı bir geçiş
        chunkStyle = "QSlider::add-page:vertical { background: qlineargradient(x1:0, y1:1, x2:0, y2:0, stop:0 #e67e22, stop:1 #e74c3c); border-radius: 3px; }";
    } else {
        // Normal: Standart şık bir mavi
        chunkStyle = "QSlider::add-page:vertical { background: #3498db; border-radius: 3px; }";
    }

    slider->setStyleSheet(baseStyle + chunkStyle);
}

void VolumeSlider::setMaxVolume(int maxVol)
{
    slider->setRange(0, maxVol);
    // Eğer o anki slider değeri yeni max'tan büyükse, stili baştan değerlendir
    setSliderStyle(slider->value() > 100);
}

// Dosyanın en altına şu yeni fonksiyonu ekle:
void VolumeSlider::updatePercentage(int value)
{
    percentageLabel->setText(QString("%%1").arg(value));
}