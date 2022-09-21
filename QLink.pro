QT += core gui
QT += core testlib

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    block.cpp \
    game.cpp \
    main.cpp \
    test.cpp \
    widget.cpp

HEADERS += \
    block.h \
    game.h \
    test.h \
    widget.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    img/0.png \
    img/1.png \
    img/2.png \
    img/3.png \
    img/4.png \
    img/5.png

RESOURCES += \
    img.qrc
