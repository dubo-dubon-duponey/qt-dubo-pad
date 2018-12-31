TEMPLATE = lib
QT = core

PROJECT_ROOT = $$PWD/..
include($$PROJECT_ROOT/config/qmakeitup.pri)

INCLUDEPATH += $$PWD

DEFINES += LIBDUBOPAD_LIBRARY
contains(DUBO_LINK_TYPE, static){
    DEFINES += LIBDUBOPAD_USE_STATIC
}

copyToDestdir($$PWD/lib$${TARGET}/*.h, $$DESTDIR/../include/lib$${TARGET})
copyToDestdir($$PWD/../res/redist/*, $$DESTDIR/../share/lib$${TARGET})

SOURCES +=  $$PWD/root.cpp \
            $$PWD/client.cpp

HEADERS +=  $$PWD/lib$${TARGET}/global.h \
            $$PWD/lib$${TARGET}/root.h \
            $$PWD/lib$${TARGET}/config.h \
            $$PWD/lib$${TARGET}/client.h

mac {
    # Should work for other platforms as well - to be tested
    copydata.commands = $(COPY_DIR) $$PWD/../third-party/build.$${DUBO_PLATFORM}.$$system(uname -m)/bin/* $${DESTDIR}/../bin/
    first.depends = $(first) copydata
    export(first.depends)
    export(copydata.commands)
    QMAKE_EXTRA_TARGETS += first copydata
}

# XXX version is kind of meaningless here
# Also, this will be unusable if compiled against external sources
DEFINES += CRASHPAD_VERSION=\\\"$${DUBO_INTERNAL_VERSION}\\\"
DEFINES += CRASHPAD_REVISION=\\\"$${DUBO_INTERNAL_VERSION}\\\"


#win32 {
#}

#!mac:!win32{
#}
