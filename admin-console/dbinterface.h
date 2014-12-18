#ifndef DBINTERFACE_H
#define DBINTERFACE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sqlite3.h"

#include "bool.h"

static const char EXISTS_LOGIN[] = "select login from users where login=\"%s\"";
static const char EXISTS_PASSWORD[] = "select login from users where login=\"%s\" and password=\"%s\"";
static const char CHECK_ROLE[] = "select role from users where login=\"%s\"";
static const char UPD_BALANCE[] = "update account set balance = balance + %s where accountID = %s;";
static const char COMMIT[] = "commit;";
static const char ROLLBACK[] = "rollback";
static const char BEGIN[] = "begin;";
static const char CHECK_BALANCE[] = "select balance from account where accountid = %s";
static const char SHOW[] = "select login, accountID, balance, accountType, time, "
        "totalTransaction from account inner join accounttype on accounttype.accountTypeID "
        "= account.accounttypeid inner join customers on  customerID = accountOwner";

static const char SHOWLOGGER[] = "select operation, table_name, data, customerID FROM LOGGER";

int checkBalance(sqlite3 *db, char * acc_id, double * balance);
int managTransaction(sqlite3 * db, int trans);
int updateBalance(sqlite3 *db, char * acc_id, char * sum);
bool loginExists(sqlite3 *db, const char *login);
bool checkPassword(sqlite3 *db, const char *login, const char *password);
int getUserGroup(sqlite3 *db, const char *login);
bool showInfo(sqlite3 * db, char * res);

//logger
bool showLogger(sqlite3 * db, char * result);

#endif // DBINTERFACE_H
