# -------------------------------------------------
# Project created by QtCreator 2017-08-29T10:57:34
# -------------------------------------------------
QT += network \
    sql
TARGET = CoalQualityDetection
TEMPLATE = app
SOURCES += main.cpp \
    mainwindow.cpp \
    dialog_add.cpp \
    dialog_find.cpp \
    dbutil.cpp \
    logutil.cpp \
    data_dependency.cpp
HEADERS += mainwindow.h \
    dialog_add.h \
    dialog_find.h \
    dbutil.h \
    logutil.h \
    data_dependency.h
FORMS += mainwindow.ui \
    dialog_add.ui \
    dialog_find.ui
RESOURCES += main.qrc
OTHER_FILES += coal.sql \
    config.xml
