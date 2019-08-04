TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    thread.cpp

HEADERS += \
    thread.h

QMAKE_CXXFLAGS += -pthread
