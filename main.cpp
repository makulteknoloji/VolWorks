#include "mainwindow.h"
#include <QApplication>
#include <QLocale>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        // "volworks_tr_TR" veya "volworks_en_US" gibi dosyaları arar
        const QString baseName = "volworks_" + QLocale(locale).name();

        // embed_translations özelliği sayesinde qm dosyaları :/i18n/ klasörüne sanal olarak gömülür
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }

    MainWindow w;
    // w.show(); tray uygulaması olduğu için show demiyoruz
    return a.exec();
}