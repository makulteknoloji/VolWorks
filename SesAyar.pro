QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    settingsdialog.cpp \
    volumeslider.cpp \


HEADERS += \
    mainwindow.h \
    settingsdialog.h \
    volumeslider.h \


FORMS += \
    mainwindow.ui

TRANSLATIONS += \
    i18n/volworks_tr_TR.ts \
    i18n/volworks_en_US.ts

# lrelease aracını derleme sırasında otomatik çalıştırır ve .qm dosyalarını uygulamanın içine gömer
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /usr/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc
