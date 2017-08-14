#-------------------------------------------------
#
# Project created by QtCreator 2017-08-01T17:21:41
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ImageEnhanceDemo
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    enhance.cpp \
    filter.cpp \
    gray2color.cpp \
    mypiclabel.cpp \
    regionalenhance.cpp \
    convertimage.cpp \
    graybound.cpp \
    evaluationindex.cpp

HEADERS += \
        mainwindow.h \
    enhance.h \
    filter.h \
    gray2color.h \
    mypiclabel.h \
    regionalenhance.h \
    convertimage.h \
    graybound.h \
    evaluationindex.h

FORMS += \
        mainwindow.ui \
    regionalenhance.ui
INCLUDEPATH += G:/Qt/opencv/3.2.0/install/include \
               G:/Qt/opencv/3.2.0/install/include/opencv \
               G:/Qt/opencv/3.2.0/install/include/opencv2

LIBS += G:/Qt/opencv/3.2.0/install/x86/mingw/bin/libopencv_*
