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
    # Copy over the handler
    PAD_HANDLER.files = $${DESTDIR}/crashpad_handler
    PAD_HANDLER.path = Contents/Helpers
    QMAKE_BUNDLE_DATA += PAD_HANDLER

    # Link our demo helpers
    OBJECTIVE_SOURCES=helpers.mm

    # Add plist with Pad properties, and a nice icon
    OTHER_FILES += $$PWD/Info.plist \
        $$PWD/main.icns

    QMAKE_INFO_PLIST = $${PWD}/Info.plist
    ICON = $${PWD}/main.icns
}

contains(DUBO_LINK_TYPE, static){

    mac{
        # Link against the embedded copy
        QMAKE_LFLAGS += -F$${DESTDIR}/$${TARGET}.app/Contents/Frameworks
    }

    !isEmpty(DUBO_INTERNAL){
        message( -> Using internal third-party $${DUBO_INTERNAL_VERSION})
        win32{
#            copyToDestdir($$DUBO_EXTERNAL/lib/WinSparkle.dll, $$DESTDIR)
        }
    }
}

# Generate symbol dump: ../../../dump_syms -a x86_64 ./dubopad


#LIBS += -L/Users/dmp/dev/webitup/breakpadautotoolsbuild/lib
#LIBS += -llibbreakpad
#INCLUDEPATH += /Users/dmp/dev/webitup/breakpadautotoolsbuild/include

#system(mkdir ../buildd/demo)

#}

#LIBS +=         -l$${DEPNAME}

#mac{
#    contains(DUBO_LINK_TYPE, static){
#        # If compiling statically
#        isEmpty(DUBO_EXTERNAL){
#            QMAKE_LFLAGS += -F$$PWD/../../../qt-$${DEPNAME}/third-party/BreakpadClient/
#        }else{
#            QMAKE_LFLAGS += -F$${DUBO_EXTERNAL}/Frameworks/
#        }
#    }

#    isEmpty(DUBO_DESTDIR){
#        # If one wants to test from here, we need to link to BreakPad (be it static or dynamic)
#        system(mkdir -p $$DESTDIR/$${TARGET}.app/Contents/Frameworks)
#        system(ln -s $$PWD/../../../qt-$${DEPNAME}/third-party/BreakpadClient/Breakpad.framework $$DESTDIR/$${TARGET}.app/Contents/Frameworks)
#    }
#}

