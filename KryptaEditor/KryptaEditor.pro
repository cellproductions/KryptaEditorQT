#-------------------------------------------------
#
# Project created by QtCreator 2014-08-20T22:41:42
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = KryptaEditor
TEMPLATE = app


SOURCES += main.cpp\
    ClickableLabel.cpp \
    EntBrowserDialog.cpp \
    AssetListItem.cpp \
    Utilities.cpp \
    Assets.cpp \
    Configuration.cpp \
    EnvBrowserDialog.cpp \
    GLPanel.cpp \
    LayerBrowserDialog.cpp \
    LayerOptionsItem.cpp \
    MainWindow.cpp \
    Map.cpp \
    MapProject.cpp \
    PrjSetupDialog.cpp \
    Resources.cpp \
    ConfigDialog.cpp \
    ObjectAction.cpp

HEADERS  += \
    ClickableLabel.h \
    EntBrowserDialog.h \
    AssetListItem.h \
    Utilities.h \
    Assets.h \
    Configuration.h \
    DialogResult.h \
    EnvBrowserDialog.h \
    GLPanel.h \
    LayerBrowserDialog.h \
    LayerOptionsItem.h \
    MainWindow.h \
    Map.h \
    MapProject.h \
    PrjSetupDialog.h \
    Resources.h \
    Tool.h \
    ConfigDialog.h \
    ObjectAction.h

FORMS    += mainwindow.ui \
    prjsetupdialog.ui \
    envbrowserdialog.ui \
    layerbrowserdialog.ui \
    EntBrowserDialog.ui \
    ConfigDialog.ui

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../Users/Callum/Desktop/Krypta2D/release/ -lKrypta2D
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../Users/Callum/Desktop/Krypta2D/debug/ -lKrypta2D

INCLUDEPATH += $$PWD/../../../../Users/Callum/Desktop/Krypta2D/Krypta2D/include
INCLUDEPATH += $$PWD/../../../../Users/Callum/Desktop/Krypta2D/Krypta2D/libs/include
DEPENDPATH += $$PWD/../../../../Users/Callum/Desktop/Krypta2D/Krypta2D/include

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../../Users/Callum/Desktop/Krypta2D/release/libKrypta2D.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../../Users/Callum/Desktop/Krypta2D/debug/libKrypta2D.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../../Users/Callum/Desktop/Krypta2D/release/Krypta2D.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../../Users/Callum/Desktop/Krypta2D/debug/Krypta2D.lib

win32: LIBS += -L$$PWD/../../../../Users/Callum/Desktop/Krypta2D/Krypta2D/libs/ -lglew32

INCLUDEPATH += $$PWD/../../../../Users/Callum/Desktop/Krypta2D/Krypta2D/libs/include/GL
DEPENDPATH += $$PWD/../../../../Users/Callum/Desktop/Krypta2D/Krypta2D/libs/include/GL

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/../../../../Users/Callum/Desktop/Krypta2D/Krypta2D/libs/glew32.lib
else:win32-g++: PRE_TARGETDEPS += $$PWD/../../../../Users/Callum/Desktop/Krypta2D/Krypta2D/libs/libglew32.a

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../Users/Callum/Desktop/Krypta2D/Krypta2D/libs/ -lopenAL32
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../Users/Callum/Desktop/Krypta2D/Krypta2D/libs/ -lopenAL32d

INCLUDEPATH += $$PWD/
DEPENDPATH += $$PWD/

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../../Users/Callum/Desktop/Krypta2D/Krypta2D/libs/libopenAL32.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../../Users/Callum/Desktop/Krypta2D/Krypta2D/libs/libopenAL32d.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../../Users/Callum/Desktop/Krypta2D/Krypta2D/libs/openAL32.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../../Users/Callum/Desktop/Krypta2D/Krypta2D/libs/openAL32d.lib

OTHER_FILES +=
