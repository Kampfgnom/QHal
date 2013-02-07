QHAL_PATH = ..
include($$QHAL_PATH/QHal.pri)

### General config ###

TARGET          = $$QHAL_TARGET
VERSION         = $$QHAL_VERSION
TEMPLATE        = lib
QT              +=
QT              -= gui
CONFIG          += staticlib
QMAKE_CXXFLAGS  += -Wall -ansi -pedantic -Wno-long-long
INCLUDEPATH     += $$QHAL_INCLUDEPATH


### Files ###

HEADERS += \
    qhalresource.h \
    qhallink.h

SOURCES += \
    qhalresource.cpp \
    qhallink.cpp
