#-------------------------------------------------
#
# Project created by QtCreator 2014-11-13T16:39:47
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = gl-experiments
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    glwidget.cpp \
    sphere.cpp

HEADERS  += widget.h \
    glwidget.h \
    sphere.h \
    main.h

FORMS    += widget.ui
