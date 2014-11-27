TEMPLATE = app
CONFIG += console
#CONFIG += debug
CONFIG -= qt

SOURCES += main.c \
    commands.c \
    dbinterface.c\
    admin_command_cust.c
LIBS += -lsqlite3

HEADERS += \
    commands.h \
    bool.h \
    dbinterface.h\
    admin_command_cust.h
