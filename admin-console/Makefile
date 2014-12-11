#############################################################################
# Makefile for building: admin-console
#############################################################################

####### Compiler, tools and options

CC            = gcc
CFLAGS        = -Wall
LIBS          = -pthread -ldl

####### Files

SOURCES = main.c \
		testing.c \
		commands.c \
		dbinterface.c \
		admin_command_cust.c \
		sqlite3.c
OBJECTS = commands.o \
		dbinterface.o \
		admin_command_cust.o \
		sqlite3.o

TARGET  = admin-console
TEST_TARGET = test

####### Build rules

all: $(TARGET) tests

tests: testing.o
	$(CC) $(CFLAGS) testing.o $(OBJECTS) $(LIBS) -o $(TEST_TARGET)

clean:
	$(RM) -f $(TARGET) $(TEST_TARGET) *.o *~

$(TARGET): $(OBJECTS) main.o
	$(CC) $(CFLAGS) main.o $(OBJECTS) $(LIBS) -o $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $<
