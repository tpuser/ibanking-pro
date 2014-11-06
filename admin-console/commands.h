#ifndef COMMANDS_H
#define COMMANDS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sqlite3.h>

#include "bool.h"
#include "dbinterface.h"

static const char COMMAND_1[] = "cmd1";
static const char COMMAND_2[] = "cmd2";
static const char COMMAND_HELP[] = "help";
static const char COMMAND_EXIT[] = "exit";

static const int ADMIN_GROUP = 0;
static const int OPERATOR_GROUP = 1;

bool commandExists(const char *command);
bool executeCommand(sqlite3 *db, const char *command);
bool checkCommandPermission(sqlite3 *db, const char *login, const char *command);

void commandHelp();
void command1();
void command1();

#endif // COMMANDS_H
