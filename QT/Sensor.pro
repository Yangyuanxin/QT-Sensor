QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets serialport printsupport

CONFIG += c++11
#CONFIG += console
CONFIG += warn_off

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    SerialDriver/serialthread.cpp \
    main.cpp \
    mainwindow.cpp \
    qcustomplot.cpp \
    until.cpp

HEADERS += \
    SerialDriver/serialthread.h \
    mainwindow.h \
    public.h \
    qcustomplot.h

FORMS += \
    mainwindow.ui \

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

#可执行文件名称
TARGET=SensorTool

#版本信息
VERSION = 1.3.3

#公司名称
QMAKE_TARGET_COMPANY = "Copyright(C)    2022,      Anycubic Co.Ltd."

#产品名称
QMAKE_TARGET_PRODUCT = "Anycubic DebugTool"

#文件说明
QMAKE_TARGET_DESCRIPTION = "Qt Creator based on Qt 5.14.2 (MinGW 64 bit)"

#版权信息
QMAKE_TARGET_COPYRIGHT = "Anycubic Co.Ltd."

#中文（简体）
RC_LANG = 0x0004
