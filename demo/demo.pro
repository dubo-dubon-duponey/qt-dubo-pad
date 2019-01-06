CONFIG += c++14

TEMPLATE = app
QT = core widgets webengine webenginewidgets webchannel

QMAKE_TARGET_BUNDLE_PREFIX=com.dubo-dubon-duponey

PROJECT_ROOT = $$PWD/..
include($$PROJECT_ROOT/config/qmakeitup.pri)

INCLUDEPATH += $$PWD

LIBS += -l$${DUBO_LINK_NAME}

contains(DUBO_LINK_TYPE, static){
    DEFINES += LIBDUBOPAD_USE_STATIC
}

SOURCES += $$PWD/main.cpp
RESOURCES += $$PWD/demo.qrc

mac{
    # Add plist, and a nice icon
    OTHER_FILES += $$PWD/Info.plist \
        $$PWD/demo.icns

    QMAKE_INFO_PLIST = $${PWD}/Info.plist
    ICON = $${PWD}/demo.icns
}




LIBS += -L$${DUBO_EXTERNAL}/lib
INCLUDEPATH += $${DUBO_EXTERNAL}/include

SOURCES += $$PWD/helpers.cpp
HEADERS += $$PWD/helpers.h

mac{
    # AppKit necessary because of the helpers
    LIBS += -framework AppKit

    # Helpers
    OBJECTIVE_SOURCES=$$PWD/mac.mm

    # Copy over the handler
    PAD_HANDLER.files = $${DESTDIR}/crashpad_handler
    PAD_HANDLER.path = Contents/Helpers
    QMAKE_BUNDLE_DATA += PAD_HANDLER

}
