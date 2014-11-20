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
static const char COMMAND_DEB[] = "debit";
static const char COMMAND_CRED[] = "credit";
static const char COMMAND_CHCK[] = "check";
static const char COMMAND_TRNSF[] = "transfer";
static const char COMMAND_UNDO[] = "undo";
static const char COMMAND_COMMIT[] = "cmit";

static const int ADMIN_GROUP = 0;
static const int OPERATOR_GROUP = 1;

bool commandExists(const char *command);
bool executeCommand(sqlite3 *db, const char *command);
bool checkCommandPermission(sqlite3 *db, const char *login, const char *command);

void commandHelp();
void command1();
void command1();
void debit(int acc_id, double sumtoget);
void credit(int acc_id, double sumtopush);
void checkAccount(int acc_id);
void transfer(int acc_from, int acc_to, double sum);
void undo();

#endif // COMMANDS_H
