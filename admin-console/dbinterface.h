#ifndef DBINTERFACE_H
#define DBINTERFACE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>

#include "bool.h"

bool loginExists(sqlite3 *db, const char *login);
bool checkPassword(sqlite3 *db, const char *login, const char *password);
int getUserGroup(sqlite3 *db, const char *login);

#endif // DBINTERFACE_H
