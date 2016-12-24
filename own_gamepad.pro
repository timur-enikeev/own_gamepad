#-------------------------------------------------
#
# Project created by QtCreator 2016-11-25T20:47:43
#
#-------------------------------------------------

QT       += core gui multimedia
CONFIG += c++11
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = own_gamepad
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    assignwizard.cpp \
    playerwizardpage.cpp \
    numberofplayerspage.cpp \
    gamepads.cpp \
    soundsignals.cpp

HEADERS  += mainwindow.h \
    assignwizard.h \
    playerwizardpage.h \
    numberofplayerspage.h \
    gamepads.h \
    soundsignals.h

FORMS    += mainwindow.ui \
    assignwizard.ui \
    playerwizardpage.ui \
    numberofplayerspage.ui

unix: {
    INCLUDEPATH += /usr/include/SDL2
    LIBS += -lSDL2
}

win32: {
# Change to your path
    INCLUDEPATH += "C:\SDL2-2.0.5\include"
    LIBS += -LC:\sdl -lSDL2 -lSDL2main
}

soundfiles.commands = $(COPY_DIR) $$PWD/sounds $$OUT_PWD
first.depends = $(first) soundfiles
export(first.depends)
export(soundfiles.commands)
QMAKE_EXTRA_TARGETS += first soundfiles

DISTFILES += \
    README.md \
    sounds/1.wav \
    sounds/2.wav \
    sounds/3.wav \
    sounds/4.wav \
    sounds/answer.wav \
    sounds/answers-player.wav \
    sounds/false-start.wav \
    sounds/reset.wav \
    sounds/start.wav
