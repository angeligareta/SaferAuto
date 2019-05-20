QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SaferAutoGUI
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11 static

SOURCES += \
        src/main.cpp \
        src/yolo.cpp \
        src/detectionwindow.cpp \
        src/mainwindow.cpp \
        src/yoloclassclassifier.cpp

HEADERS += \
        include/yolo_v2_class.hpp \
        include/yolo.h \
        include/detectionwindow.h \
        include/mainwindow.h \
        include/yoloclassclassifier.h

FORMS += \
        ui/detectionwindow.ui \
        ui/mainwindow.ui


INCLUDEPATH += $$PWD/darknet
INCLUDEPATH += $$PWD/darknet/src
INCLUDEPATH += /usr/local/include

INCLUDEPATH += $$PWD/lib/tesseract-api/api
INCLUDEPATH += $$PWD/lib/tesseract-api/arch
INCLUDEPATH += $$PWD/lib/tesseract-api/ccmain
INCLUDEPATH += $$PWD/lib/tesseract-api/ccstruct
INCLUDEPATH += $$PWD/lib/tesseract-api/ccutil
INCLUDEPATH += $$PWD/lib/tesseract-api/classify
INCLUDEPATH += $$PWD/lib/tesseract-api/cutil
INCLUDEPATH += $$PWD/lib/tesseract-api/dict
INCLUDEPATH += $$PWD/lib/tesseract-api/lstm
INCLUDEPATH += $$PWD/lib/tesseract-api/opencl
INCLUDEPATH += $$PWD/lib/tesseract-api/textord
INCLUDEPATH += $$PWD/lib/tesseract-api/training
INCLUDEPATH += $$PWD/lib/tesseract-api/viewer
INCLUDEPATH += $$PWD/lib/tesseract-api/wordrec

INCLUDEPATH += $$PWD/lib/leptonica

DEPENDPATH += $$PWD/darknet

LIBS += -L$$PWD/darknet/ -ldarknet
LIBS += `pkg-config --cflags --libs tesseract opencv`

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
