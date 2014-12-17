
#include "dbinterface.h"

#define UNUSED(x) (void)x;

//LastError
//int error;

const char * ADMNSTRTV_QRS[] = {CHECK_ROLE, EXISTS_LOGIN, EXISTS_PASSWORD};
const char * MAN_QRS[] = {UPD_BALANCE, COMMIT, BEGIN, ROLLBACK, CHECK_BALANCE, SHOW};

//Callback for checking
static int callbackCheckAcc(void *cursum, int argc, char **argv, char **azColName)
{
    (void)argc;         // unused
    (void)azColName;    // unused
    double * sum = (double*)cursum;
    sscanf(argv[0], "%lf", sum);
    return 0;
}

static int callbackShowInfo(void *res, int argc, char **argv, char **azColName)
{
    (void)azColName;    // unused
    int i = 0;
    char * sum = (char*)res;
    for (i = 0; i < argc; ++i)
    {
        strcat(sum, argv[i]);
        strcat(sum, " ");

    }
    strcat(sum, "\n");
    return 0;
}

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

    query = malloc(strlen(EXISTS_LOGIN) + strlen(login) + 1);
    sprintf(query,        EXISTS_LOGIN, login);
    sqlite3_exec(db, query, callbackRowExists, (void*)&result, NULL);
    free(query);

    return result;
}

bool checkPassword(sqlite3 *db, const char *login, const char *password)
{
    char *query;
    bool result = false;

    query = malloc(strlen(EXISTS_PASSWORD) + strlen(login) + 1);
    sprintf(query,        EXISTS_PASSWORD, login, password);
    sqlite3_exec(db, query, callbackRowExists, (void*)&result, NULL);
    free(query);

    return result;
}

int managTransaction(sqlite3 * db, int trans)
{
    int error = 0;
    sqlite3_stmt * query;
    switch (trans)
    {
    case 0: {
            error = sqlite3_prepare_v2(db, COMMIT, strlen(COMMIT) + 1, &query, NULL);
            error = sqlite3_step(query);
            sqlite3_reset(query);
            error = sqlite3_prepare_v2(db, BEGIN, strlen(BEGIN) + 1, &query, NULL);
            error = sqlite3_step(query);
            sqlite3_reset(query);
            break;
    }
    case 1: {
        error = sqlite3_prepare_v2(db, ROLLBACK, strlen(ROLLBACK) + 1, &query, NULL);
        error = sqlite3_step(query);
        sqlite3_reset(query);
        break;
    }
    default:{
        error = 1;
    }

    }




    return error;
}

bool showInfo(sqlite3 * db, char * res)
{
    bool result = false;
    sqlite3_exec(db, SHOW, callbackShowInfo, (void*)res, NULL);

    return result;
}


bool showLogger(sqlite3 * db, char * result){
    bool finalResult = false;
    sqlite3_exec(db, SHOW, callbackShowInfo, (void*)result, NULL);

    return finalResult;
}

int updateBalance(sqlite3 *db, char * acc_id, char * sum)
{
    char *statement;
    int error = 0;

    sqlite3_stmt * query;
    statement = calloc(strlen(UPD_BALANCE) + strlen(acc_id) + strlen(sum) + 1, sizeof(char));
    sprintf(statement, UPD_BALANCE, sum, acc_id);
    error = sqlite3_prepare_v2(db, statement, strlen(statement)+1, &query, NULL);
    free(statement);
    error = sqlite3_step(query);
    sqlite3_reset(query);
    return error;
}

int checkBalance(sqlite3 *db, char * acc_id, double * balance)
{
    char *statement;
    int error = 0;

    statement = calloc(strlen(CHECK_BALANCE) + strlen(acc_id) + 1, sizeof(char));
    sprintf(statement, CHECK_BALANCE, acc_id);
    sqlite3_exec(db, statement, callbackCheckAcc, balance, NULL);
    free(statement);
    return error;
}

int getUserGroup(sqlite3 *db, const char *login)
{
    char *query;
    int result = 0;

    query = malloc(strlen(CHECK_ROLE) + strlen(login) + 1);
    sprintf(query,        CHECK_ROLE, login);
    sqlite3_exec(db, query, callbackGetUserGroup, (void*)&result, NULL);
    free(query);

    return result;
}
