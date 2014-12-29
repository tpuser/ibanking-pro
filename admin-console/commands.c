#include "commands.h"
#include "dbinterface.h"
#include "admin_command_cust.h"
const char *ADMIN_COMMANDS[] = {COMMAND_1, COMMAND_ADD, COMMAND_DEL,
                                COMMAND_ADDACC,
                                    COMMAND_DELACC, COMMAND_TRANSF};
const char *OPERATOR_COMMANDS[] = {COMMAND_2};
const char *COMMON_COMMANDS[] = {COMMAND_EXIT, COMMAND_HELP,
                                 COMMAND_DEB, COMMAND_CRED, COMMAND_CHCK,
                                 COMMAND_UNDO, COMMAND_COMMIT, COMMAND_SHOW, COMMAND_LOGGER };
const int ADMIN_COMMANDS_COUNT = 6;
const int OPERATOR_COMMANDS_COUNT = 1;
const int COMMON_COMMANDS_COUNT = 9;

//Last error
int error;

//Needs to be changed, unsafe, char * using with calloc(200...
//query - global, but gets free after being used
char * statement;
sqlite3_stmt * query;

//
bool commandExists(const char *command)
{
    int i;
    if (command == NULL)
        return false;
    for (i = 0; i < COMMON_COMMANDS_COUNT; ++i)
    {
        if (!strcmp(command, COMMON_COMMANDS[i]))
            return true;
    }
    for (i = 0; i < ADMIN_COMMANDS_COUNT; ++i)
    {
        if (!strcmp(command, ADMIN_COMMANDS[i]))
            return true;
    }
    for (i = 0; i < OPERATOR_COMMANDS_COUNT; ++i)
    {
        if (!strcmp(command, OPERATOR_COMMANDS[i]))
            return true;
    }

    return false;
}

// COMMANDS

void commandHelp()
{
    printf("\t%s - command 1 (admin only)\n", COMMAND_1);
    printf("\t%s - command 2 (operator only)\n", COMMAND_2);
    printf("\t%s - display this help\n", COMMAND_HELP);
    printf("\t%s - exit\n", COMMAND_EXIT);
    printf("\t%s - debit \"id\" \"sum\"\n", COMMAND_DEB);
    printf("\t%s - credit \"id\" \"sum\"\n", COMMAND_CRED);
    printf("\t%s - check state \"id\" \n", COMMAND_CHCK);
    printf("\t%s - transfer \"from_id\" \"to_id\" \"sum\"\n", COMMAND_TRANSF);
    printf("\t%s - undo\n", COMMAND_UNDO);
    printf("\t%s - commit\n", COMMAND_COMMIT);
    printf("\t%s - logging\n", COMMAND_LOGGER);
    printf("\t%s - show all account states\n", COMMAND_SHOW);
    printf("\t%s - add new customer\n", COMMAND_ADD);
    printf("\t%s - delete customer\n", COMMAND_DEL);
    printf("\t%s - add account for customer\n", COMMAND_ADDACC);
    printf("\t%s - delete account for customer\n", COMMAND_DELACC);
    printf("We love Apple!\n");
}

void command1()
{
    printf("command 1 executed\n");
}

void command2()
{
    printf("command 2 executed\n");
}

void commit(sqlite3 *db)
{
    managTransaction(db, 0);
}

void undo(sqlite3 *db)
{
    managTransaction(db, 1);
}

void debit(sqlite3 *db, int acc_id, double sumtoget)
{
    char * acc, * sum;
    acc = calloc(100, sizeof(char));
    sum = calloc(100, sizeof(char));
    sprintf(acc, "%i", acc_id);
    sprintf(sum, "%lf", - sumtoget);

    updateBalance(db, acc, sum);
    free(acc);
    free(sum);
}

void credit(sqlite3 *db, int acc_id, double sumtopush)
{
    char * acc, * sum;
    acc = calloc(100, sizeof(char));
    sum = calloc(100, sizeof(char));
    sprintf(acc, "%i", acc_id);
    sprintf(sum, "%lf", sumtopush);

    updateBalance(db, acc, sum);
    free(acc);
    free(sum);
}

void transfer(sqlite3 *db, int acc_from, int acc_to, double sum)
{
    debit(db, acc_from, sum);
    credit(db, acc_to, sum);
    //credit(db, acc_to, sum);
}

void checkAccount (sqlite3 *db, int acc_id)
{
    double sum;
    char * acc;
    acc = calloc(100, sizeof(char));
    sprintf(acc, "%i", acc_id);
    checkBalance(db, acc, &sum);
    free(acc);
    printf("balance (of id = %i) = %lf\n", acc_id, sum);
}

void showAll(sqlite3 * db)
{
    char * res;
    res = calloc(500, sizeof(char));
    strcat(res, "\n");
    showInfo(db, res);
   // printf("%s", res);
}

void logger(sqlite3 * db)
{
    char * result;
    result = calloc(2000, sizeof(char));
    strcat(result, "\n");
    showLogger(db, result);
    printf("%s", result);
}

// PARSING

int countWords(const char *string)
{
    int count = 0;
    int i = 0;

    while ((string[i] != '\0') && (isspace(string[i])))
        i++;
    while (string[i] != '\0')
    {
        count++;
        while ((string[i] != '\0') && (!isspace(string[i])))
            i++;
        while ((string[i] != '\0') && (isspace(string[i])))
            i++;
    }

    return count;
}

//Parsing command-word
char *getWord(const char *string, int index)
{
    int i = 0;
    int len;
    char *word;

    while ((string[i] != '\0') && (isspace(string[i])))
        i++;
    while ((string[i] != '\0') && (index > 0))
    {
        index--;
        while ((string[i] != '\0') && (!isspace(string[i])))
            i++;
        while ((string[i] != '\0') && (isspace(string[i])))
            i++;
    }

    len = 0;
    while ((string[i + len] != '\0') && (!isspace(string[i + len])))
        len++;

    word = malloc(len + 1);
    strncpy(word, &string[i], len);
    word[len] = '\0';
    return word;
}

bool executeCommand(sqlite3 *db, const char *command)
{
    char **params;
    int paramsCount;
    int i;

    int from, to;
    double sum;

    (void)db;   // unused


    // parse command
    paramsCount = countWords(command);
    params = malloc(sizeof(char*) * paramsCount);
    for (i=0; i<paramsCount; i++)
        params[i] = getWord(command, i);            

    if ((paramsCount >= 1) && (!strcmp(params[0], COMMAND_HELP)))
        commandHelp();

    if ((paramsCount >= 1) && (!strcmp(params[0], COMMAND_1)))
        command1();

    if ((paramsCount >= 1) && (!strcmp(params[0], COMMAND_2)))
        command2();

    if ((paramsCount >= 4) && (!strcmp(params[0], COMMAND_TRANSF)))
    {
        sscanf(params[1], "%i", &from);
        sscanf(params[3], "%lf", &sum);
        sscanf(params[2], "%i", &to);
        //transfer(db, from, to, sum);
        if (sum < 0 ) return false;
        debit(db, from, sum);
        credit(db, to, sum);
    }

    if ((paramsCount >= 3) && (!strcmp(params[0], COMMAND_DEB)))
    {
        sscanf(params[1], "%i", &from);
        sscanf(params[2], "%lf", &sum);
        if (sum < 0 ) return false;
        debit(db, from, sum);
    }

    if ((paramsCount >= 3) && (!strcmp(params[0], COMMAND_CRED)))
    {
        sscanf(params[1], "%i", &to);
        sscanf(params[2], "%lf", &sum);
        if (sum < 0) return false;
        credit(db, to, sum);
    }

    if ((paramsCount >= 2) && (!strcmp(params[0], COMMAND_CHCK)))
    {
        sscanf(params[1], "%i", &to);
        checkAccount(db, to);
    }

    if ((paramsCount >= 1) && (!strcmp(params[0], COMMAND_UNDO)))
        undo(db);

    if ((paramsCount >= 1) && (!strcmp(params[0], COMMAND_ADD)))
        createNewCustomer(db);

    if ((paramsCount >= 1) && (!strcmp(params[0], COMMAND_DEL)))
        deleteCustomer(db);

    if ((paramsCount >= 1) && (!strcmp(params[0], COMMAND_DELACC)))
        deleteAccount(db);

    if ((paramsCount >= 1) && (!strcmp(params[0], COMMAND_ADDACC)))
        addAccount(db);

    if ((paramsCount >= 1) && (!strcmp(params[0], COMMAND_COMMIT)))
        commit(db);

    if ((paramsCount >= 1) && (!strcmp(params[0], COMMAND_SHOW)))
        showAll(db);

    if ((paramsCount >= 1) && (!strcmp(params[0], COMMAND_LOGGER)))
        logger(db);

    for (i=0; i<paramsCount; i++)
        free(params[i]);
    free(params);

    return true;
}

bool checkCommandPermission(sqlite3 *db, const char *login, const char *command)
{
    int group;
    bool permission = false;
    int i;

    group = getUserGroup(db, login);
    if (group < 0)
        return false;

    for (i=0; i<COMMON_COMMANDS_COUNT; i++)
        if (!strcmp(COMMON_COMMANDS[i], command))
            permission = true;

    if (group == ADMIN_GROUP)
        for (i=0; i<ADMIN_COMMANDS_COUNT; i++)
            if (!strcmp(ADMIN_COMMANDS[i], command))
                permission = true;

    if (group == OPERATOR_GROUP)
        for (i=0; i<OPERATOR_COMMANDS_COUNT; i++)
            if (!strcmp(OPERATOR_COMMANDS[i], command))
                permission = true;

    return permission;
}
