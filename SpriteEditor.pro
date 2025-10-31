QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# Fix for macOS AGL framework issue (deprecated framework)
macx {
    # AGL is deprecated and not available in newer macOS SDKs
    # Remove from Qt's OpenGL configuration
    QMAKE_LIBS_OPENGL -= -framework AGL
    QMAKE_LIBS_OPENGL_ES2 -= -framework AGL
    LIBS -= -framework AGL
    QMAKE_LFLAGS -= -framework AGL
}

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    drawingarea.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    drawingarea.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    spriteeditorplan.qmodel

RESOURCES += \
    resources.qrc
