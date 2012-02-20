#-------------------------------------------------
#
# Project created by QtCreator 2011-06-23T12:47:15
#
#-------------------------------------------------

QT       += core gui
QT       += opengl

TARGET = "Flame Visualiser"
TEMPLATE = app
ICON = flame_icon_v.icns

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
    iterationinfodialog.h

FORMS    += mainwindow.ui \
    positiondialog.ui \
    conditiondialog.ui \
    imagesdialog.ui \
    shapedialog.ui \
    timedialog.ui \
    agentdialog.ui \
    restrictaxesdialog.ui \
    iterationinfodialog.ui

OTHER_FILES +=
