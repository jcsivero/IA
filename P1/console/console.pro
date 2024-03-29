QT       += core


TARGET = p1console

CONFIG += c++11 console
CONFIG -= app_bundle

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


unix::MOC_DIR = $$PWD/tmp/moc
win32::MOC_DIR = $$PWD/tmp/moc
mac::OBJECTS_DIR = $$PWD/tmp/o

unix::OBJECTS_DIR = $$PWD/tmp/o
win32::OBJECTS_DIR = $$PWD/tmp/o
mac::OBJECTS_DIR = $$PWD/tmp/o

unix::RCC_DIR = $$PWD/tmp/resources
mac::RCC_DIR = $$PWD/tmp/resources
win32::RCC_DIR = $$PWD/tmp/resources



 CONFIG(debug, debug|release) {
     unix: TARGET = $$join(TARGET,,,d)
     win32: TARGET = $$join(TARGET,,,d)
     mac: TARGET = $$join(TARGET,,,d)
 }


SOURCES += \
        src/cpp/main.cpp \
    src/cpp/EstadoIA_t.cpp


HEADERS += \
    src/include/EstadoIA_t.hpp \
    src/include/GrafoIA_t.hpp \
    src/include/NodeIA_t.hpp \
    src/include/varias.hpp

INCLUDEPATH += $$PWD/src/include
DEPENDPATH += $$PWD/src/include
