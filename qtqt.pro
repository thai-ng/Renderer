#-------------------------------------------------
#
# Project created by QtCreator 2016-09-21T01:38:20
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qtqt
TEMPLATE = app


SOURCES += main.cpp\
    client.cpp \
    window361.cpp \
    renderarea361.cpp

HEADERS  += \
    drawable.h \
    pageturner.h \
    client.h \
    window361.h \
    renderarea361.h \
    lineRenderer.h \
    polygonRenderer.h \
    primitives.h \
    colorUtils.h \
    assignment1.h


win32: LIBS += -lshell32

win32: LIBS += -L$$PWD/'../../../../../Program Files (x86)/Windows Kits/8.1/Lib/winv6.3/um/x64/' -lshell32

INCLUDEPATH += $$PWD/'../../../../../Program Files (x86)/Windows Kits/8.1/Lib/winv6.3/um/x64'
DEPENDPATH += $$PWD/'../../../../../Program Files (x86)/Windows Kits/8.1/Lib/winv6.3/um/x64'

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/'../../../../../Program Files (x86)/Windows Kits/8.1/Lib/winv6.3/um/x64/shell32.lib'
else:win32-g++: PRE_TARGETDEPS += $$PWD/'../../../../../Program Files (x86)/Windows Kits/8.1/Lib/winv6.3/um/x64/libshell32.a'
