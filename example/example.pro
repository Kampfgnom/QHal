### General config ###

TARGET          = QHalExample
VERSION         = 0.1.0
TEMPLATE        = app
QT              +=
QT              -=
CONFIG          +=
QMAKE_CXXFLAGS  += -Wall -ansi -pedantic -Wno-long-long

### QHAL ###

QHAL_PATH = ..
include($$QHAL_PATH/QHal.pri)
LIBS            += $$QHAL_LIBS
INCLUDEPATH     += $$QHAL_INCLUDEPATH


### Files ###

HEADERS += \

SOURCES += \
    main.cpp

RESOURCES += \
    exampleDocuments.qrc

OTHER_FILES += \
    example.json
