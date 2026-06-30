#include "settingsdialog.h"
#include <QLabel>
#include <QCloseEvent>

SettingsDialog::SettingsDialog(QWidget *parent) : QDialog(parent)
{
    setWindowTitle(tr("VolWorks Settings"));
    setFixedSize(350, 200);

    // Kayıtlı ayarları oku (Yoksa varsayılanları kullan)
    QSettings settings("VolWorks", "Config");
    int currentMaxVol = settings.value("maxVolume", 150).toInt();
    int currentStep = settings.value("scrollStep", 5).toInt();
    QString currentMixer = settings.value("mixerApp", "pavucontrol").toString();

    // Form Elemanlarını Oluştur
    m_maxVolCombo = new QComboBox(this);
    m_maxVolCombo->addItem("%100 (Standart)", 100);
    m_maxVolCombo->addItem("%150 (Overboost)", 150);
    m_maxVolCombo->setCurrentIndex(currentMaxVol == 150 ? 1 : 0);

    m_stepSpin = new QSpinBox(this);
    m_stepSpin->setRange(1, 25);
    m_stepSpin->setSuffix(" %");
    m_stepSpin->setValue(currentStep);

    m_mixerEdit = new QLineEdit(this);
    m_mixerEdit->setText(currentMixer);
    m_mixerEdit->setToolTip(tr("Note: For CLI apps like alsamixer, use a terminal command like 'xterm -e alsamixer'."));


    // Form Layout'a Ekle
    QFormLayout *formLayout = new QFormLayout();
    formLayout->addRow(tr("Maximum Volume Limit:"), m_maxVolCombo);
    formLayout->addRow(tr("Scroll Step:"), m_stepSpin);
    formLayout->addRow(tr("Mixer Command:"), m_mixerEdit);

    // Kaydet/İptal Butonları
    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Save | QDialogButtonBox::Cancel, this);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &SettingsDialog::saveSettings);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    // Ana Layout
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(formLayout);
    mainLayout->addStretch();
    mainLayout->addWidget(buttonBox);

    // Not: setFixedSize() kullandığımız için boyut değişmez, sadece konumu geri yükleriz.
    QByteArray geometry = settings.value("settingsDialogGeometry").toByteArray();
    if (!geometry.isEmpty()) {
        restoreGeometry(geometry);
    }
}

void SettingsDialog::saveSettings()
{
    QSettings settings("VolWorks", "Config");
    settings.setValue("maxVolume", m_maxVolCombo->currentData().toInt());
    settings.setValue("scrollStep", m_stepSpin->value());
    settings.setValue("mixerApp", m_mixerEdit->text());
    settings.setValue("settingsDialogGeometry", saveGeometry());
    emit settingsSaved(); // MainWindow'a ayarların değiştiğini bildir
    accept(); // Pencereyi kapat
}
void SettingsDialog::closeEvent(QCloseEvent *event)
{
    // Kullanıcı pencereyi çarpıdan veya iptal diyerek kapatsa bile pozisyonu hatırla
    QSettings settings("VolWorks", "Config");
    settings.setValue("settingsDialogGeometry", saveGeometry());

    QDialog::closeEvent(event);
}