TEMPLATE = app
CONFIG += console
CONFIG -= qt

SOURCES += main.c \
    commands.c \
    dbinterface.c\
    admin_command_cust.c\
    sqlite3.c
LIBS += -lpthread -ldl

HEADERS += \
    commands.h \
    bool.h \
    dbinterface.h\
    admin_command_cust.h\
    sqlite3.h
