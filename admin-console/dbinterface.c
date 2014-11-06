#include "dbinterface.h"

#define UNUSED(x) (void)x;

static int callbackRowExists(void *boolParam, int argc, char **argv, char **azColName)
{
    bool *result = (bool*)boolParam;

    UNUSED(argv);
    UNUSED(azColName);
    *result = (argc != 0);
    return 0;
}

static int callbackGetUserGroup(void *intParam, int argc, char **argv, char **azColName)
{
    int *result = (int*)intParam;

    UNUSED(azColName);
    if ((argc < 1) || (sscanf(argv[0], "%d", result) != 1))
        *result = -1;

    return 0;
}

bool loginExists(sqlite3 *db, const char *login)
{
    char *query;
    bool result = false;

    query = malloc(strlen("select * from users where login=\"%s\"") + strlen(login) + 1);
    sprintf(query, "select * from users where login=\"%s\"", login);
    sqlite3_exec(db, query, callbackRowExists, (void*)&result, NULL);
    free(query);

    return result;
}

bool checkPassword(sqlite3 *db, const char *login, const char *password)
{
    char *query;
    bool result = false;

    query = malloc(strlen("select * from users where login=\"%s\" and password=\"%s\"") + strlen(login) + 1);
    sprintf(query, "select * from users where login=\"%s\" and password=\"%s\"", login, password);
    sqlite3_exec(db, query, callbackRowExists, (void*)&result, NULL);
    free(query);

    return result;
}

int getUserGroup(sqlite3 *db, const char *login)
{
    char *query;
    int result = 0;

    query = malloc(strlen("select role from users where login=\"%s\"") + strlen(login) + 1);
    sprintf(query, "select role from users where login=\"%s\"", login);
    sqlite3_exec(db, query, callbackGetUserGroup, (void*)&result, NULL);
    free(query);

    return result;
}
