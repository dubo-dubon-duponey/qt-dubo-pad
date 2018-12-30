TEMPLATE = app
QT = core gui widgets

PROJECT_ROOT = $$PWD/..
include($$PROJECT_ROOT/config/qmakeitup.pri)

INCLUDEPATH += $$PWD

LIBS += -l$${DUBO_LINK_NAME}

contains(DUBO_LINK_TYPE, static){
    DEFINES += LIBDUBOPAD_USE_STATIC
}

SOURCES +=  $$PWD/main.cpp
RESOURCES += $$PWD/demo.qrc

LIBS += -L$${DUBO_EXTERNAL}/lib
INCLUDEPATH += $${DUBO_EXTERNAL}/include

mac{
    # AppKit necessary because of the helpers
    LIBS += -framework AppKit

    # Helpers
    OBJECTIVE_SOURCES=helpers.mm

    # Add plist with Pad properties, and a nice icon
    OTHER_FILES += $$PWD/Info.plist \
        $$PWD/main.icns

    QMAKE_INFO_PLIST = $${PWD}/Info.plist
    ICON = $${PWD}/main.icns

    # Copy over the handler
    PAD_HANDLER.files = $${DESTDIR}/crashpad_handler
    PAD_HANDLER.path = Contents/Helpers
    QMAKE_BUNDLE_DATA += PAD_HANDLER

}
