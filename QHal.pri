isEmpty(QHAL_PATH) {
    error(You have to set QHAL_PATH to the path of QHal)
}

QHAL_TARGET           = qhal
QHAL_VERSION          = 0.1.0
QHAL_INCLUDEPATH      = $$PWD/include
QHAL_LIBS             = -L$$QHAL_PATH/src -l$$QHAL_TARGET
