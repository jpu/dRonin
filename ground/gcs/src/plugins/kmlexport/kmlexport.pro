TEMPLATE = lib
TARGET = KMLExport
QT += svg

include(../../gcsplugin.pri)

include(../../plugins/coreplugin/coreplugin.pri)
include(../../plugins/uavobjects/uavobjects.pri)
include(../../plugins/uavtalk/uavtalk.pri)

HEADERS += kmlexportplugin.h \
    kmlexport.h

SOURCES += kmlexportplugin.cpp \
    kmlexport.cpp

SOURCES += $$UAVOBJECT_SYNTHETICS/uavobjectsinit.cpp

OTHER_FILES += KMLExport.pluginspec

INCLUDEPATH *= $$PWD/../../../../../tools/libkml/include
DEPENDPATH *= $$PWD/../../../../../tools/libkml/include

win32:CONFIG(release, debug|release): {
    LIBS *= -L$$PWD/../../../../../tools/libkml/lib/release/
}
else:win32:CONFIG(debug, debug|release): {
    LIBS *= -L$$PWD/../../../../../tools/libkml/lib/debug/
}
else:unix: {
    LIBS *= -L$$PWD/../../../../../tools/libkml/lib/
}

LIBS *= -lkmlbase -lkmlconvenience -lkmlengine -lkmlregionator -lkmldom
