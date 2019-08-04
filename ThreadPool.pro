TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    thread.cpp

HEADERS += \
    thread.h \
    yqueue/yqueue.hpp \
    yqueue/ypipe_base.hpp \
    yqueue/ypipe.hpp \
    yqueue/atomic_ptr.hpp

QMAKE_CXXFLAGS += -pthread
