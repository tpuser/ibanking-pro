TEMPLATE = app
CONFIG += console
CONFIG -= qt

SOURCES += main.c \
    commands.c \
    dbinterface.c
LIBS += -lsqlite3

HEADERS += \
    commands.h \
    bool.h \
    dbinterface.h
