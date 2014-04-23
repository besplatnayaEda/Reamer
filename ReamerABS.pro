#-------------------------------------------------
#
# Project created by QtCreator 2014-04-12T15:51:49
#
#-------------------------------------------------

QT       += core gui opengl
CONFIG   += opengl


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

include(src/qextserialport.pri)

TARGET = ReamerABS
TEMPLATE = app


SOURCES  +=     main.cpp\
                reamerabs.cpp \
                mainreamer.cpp

HEADERS  +=     reamerabs.h \
                mainreamer.h \
                helper.h

FORMS    +=     reamerabs.ui


