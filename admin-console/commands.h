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

static const char *ADMIN_COMMANDS[] = {COMMAND_1};
static const char *OPERATOR_COMMANDS[] = {COMMAND_2};
static const char *COMMON_COMMANDS[] = {COMMAND_HELP, COMMAND_EXIT};
#define ADMIN_COMMANDS_COUNT 1
#define OPERATOR_COMMANDS_COUNT 1
#define COMMON_COMMANDS_COUNT 2

bool commandExists(const char *command);
bool executeCommand(sqlite3 *db, const char *command);
bool checkCommandPermission(sqlite3 *db, const char *login, const char *command);

void commandHelp();
void command1();
void command1();

#endif // COMMANDS_H
