TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += \
        $$PWD/include \
        $$PWD/math/include

SOURCES += \
        main.cpp \
        vmfimporter.cpp

HEADERS += \
    vmfelements.h \
    vmfimporter.h
