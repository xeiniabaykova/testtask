#-------------------------------------------------
#
# Project created by QtCreator 2017-04-18T11:53:41
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TestTaskGUI
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        MainWindow.cpp \
    FileIO.cpp \
    EllipseCreator.cpp \
    CollectPoints.cpp \
    PointCreator.cpp \
    Line.cpp \
    Point.cpp \
    Ellipse.cpp \
    LineCreator.cpp \
    GeometricPrimitiveCreator.cpp \
    MainWindowHandler.cpp

HEADERS  += MainWindow.h \
    FileIO.h \
    GeometricPrimitiveCreator.h \
    PointCreator.h \
    LineCreator.h \
    EllipseCreator.h \
    CollectPoints.h \
    Line.h \
    Point.h \
    Ellipse.h \
    GeometricPrimitive.h \
    MainWindowHandler.h

FORMS    += MainWindow.ui

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../Qt/5.8/msvc2015_64/lib/ -lQt5Charts
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../Qt/5.8/msvc2015_64/lib/ -lQt5Chartsd
else:unix: LIBS += -L$$PWD/../../../../Qt/5.8/msvc2015_64/lib/ -lQt5Charts

INCLUDEPATH += $$PWD/../../../../Qt/5.8/msvc2015_64/include
DEPENDPATH += $$PWD/../../../../Qt/5.8/msvc2015_64/include

