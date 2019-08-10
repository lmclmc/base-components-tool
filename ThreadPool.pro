TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    workqueue.cpp \
    lthread.cpp

HEADERS += \
    yqueue/yqueue.hpp \
    yqueue/ypipe_base.hpp \
    yqueue/ypipe.hpp \
    yqueue/atomic_ptr.hpp \
    workqueue.h \
    lthread.h

QMAKE_CXXFLAGS += -pthread
