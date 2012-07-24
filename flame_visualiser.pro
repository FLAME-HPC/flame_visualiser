#-------------------------------------------------
#
# Project created by QtCreator 2011-06-23T12:47:15
#
#-------------------------------------------------

QT       += core gui
QT       += opengl

# Qt 4.8 Doesn't include OpenGL Glu library automatically on linux
unix:!macx:LIBS *= -lGLU

TEMPLATE = app
TARGET = "FLAME Visualiser"

# Qt 4.7 Doesn't like spaces in target name on windows
QT_VERSION = $$[QT_VERSION]
QT_VERSION = $$split(QT_VERSION, ".")
QT_VER_MAJ = $$member(QT_VERSION, 0)
QT_VER_MIN = $$member(QT_VERSION, 1)
lessThan(QT_VER_MIN, 8):lessThan(QT_VER_MAJ, 5) {
	win32:TARGET = FLAME_Visualiser
}

macx:ICON = flame_icon_v.icns
win32:RC_FILE = flame-v.rc

SOURCES += main.cpp\
        mainwindow.cpp \
    glwidget.cpp \
    zeroxmlreader.cpp \
    visualsettingsmodel.cpp \
    visualsettingsitem.cpp \
    configxmlreader.cpp \
    agenttypedelegate.cpp \
    shapedelegate.cpp \
    shapedialog.cpp \
    shape.cpp \
    colourdelegate.cpp \
    positiondelegate.cpp \
    positiondialog.cpp \
    position.cpp \
    condition.cpp \
    conditiondelegate.cpp \
    conditiondialog.cpp \
    graphsettingsmodel.cpp \
    graphsettingsitem.cpp \
    graphwidget.cpp \
    enableddelegate.cpp \
    graphdelegate.cpp \
    imagesdialog.cpp \
    timedialog.cpp \
    agentdialog.cpp \
    restrictaxesdialog.cpp \
    iterationinfodialog.cpp

HEADERS  += mainwindow.h \
    glwidget.h \
    zeroxmlreader.h \
    agent.h \
    visualsettingsmodel.h \
    visualsettingsitem.h \
    configxmlreader.h \
    agenttype.h \
    agenttypedelegate.h \
    shapedelegate.h \
    shapedialog.h \
    shape.h \
    colourdelegate.h \
    positiondelegate.h \
    positiondialog.h \
    position.h \
    condition.h \
    conditiondelegate.h \
    conditiondialog.h \
    graphsettingsmodel.h \
    graphsettingsitem.h \
    graphwidget.h \
    enableddelegate.h \
    graphdelegate.h \
    imagesdialog.h \
    timedialog.h \
    timescale.h \
    agentdialog.h \
    restrictaxesdialog.h \
    dimension.h \
    iterationinfodialog.h \
    ruleagent.h

FORMS    += mainwindow.ui \
    positiondialog.ui \
    conditiondialog.ui \
    imagesdialog.ui \
    shapedialog.ui \
    timedialog.ui \
    agentdialog.ui \
    restrictaxesdialog.ui \
    iterationinfodialog.ui

test {
    SOURCES  -= main.cpp
    SOURCES  += test_flame_visualiser.cpp
    CONFIG   += qtestlib
    QMAKE_CXXFLAGS +=-DTESTBUILD
}
