#############################################################################
# Makefile for building: admin-console
#############################################################################

####### Compiler, tools and options

CC            = gcc
CFLAGS        = -Wall
LIBS          = -lsqlite3

####### Files

SOURCES = main.c \
		commands.c \
		dbinterface.c \
		admin_command_cust.c
OBJECTS = main.o \
		commands.o \
		dbinterface.o \
		admin_command_cust.o

TARGET  = admin-console

####### Build rules

all: $(TARGET)

clean:
	$(RM) -f $(TARGET) *.o *~

$(TARGET):  $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) $(LIBS) -o $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

