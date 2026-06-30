#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QComboBox>
#include <QSpinBox>
#include <QLineEdit>
#include <QPushButton>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QSettings>
#include <QDialogButtonBox>


class SettingsDialog : public QDialog
{
    Q_OBJECT
public:
    explicit SettingsDialog(QWidget *parent = nullptr);

signals:
    void settingsSaved(); // Ayarlar kaydedildiğinde MainWindow'a haber vereceğiz
protected:
    void closeEvent(QCloseEvent *event) override;
private slots:
    void saveSettings();

private:
    QComboBox *m_maxVolCombo;
    QSpinBox *m_stepSpin;
    QLineEdit *m_mixerEdit;
};


#endif // SETTINGSDIALOG_H