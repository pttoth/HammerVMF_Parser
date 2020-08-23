TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

LIBS += \
        -L"$$PWD/lib" \
        -l"libpttoth"


INCLUDEPATH += \
        $$PWD/include \
        $$PWD/lib/libpttoth/pttoth/include \
        $$PWD/math/include

SOURCES += \
        main.cpp \
        vmfimporter.cpp

HEADERS += \
    vmfelements.h \
    vmfimporter.h
