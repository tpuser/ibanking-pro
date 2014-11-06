#include "commands.h"

const char *ADMIN_COMMANDS[] = {COMMAND_1};
const char *OPERATOR_COMMANDS[] = {COMMAND_2};
const char *COMMON_COMMANDS[] = {COMMAND_HELP, COMMAND_EXIT};
const int ADMIN_COMMANDS_COUNT = 1;
const int OPERATOR_COMMANDS_COUNT = 1;
const int COMMON_COMMANDS_COUNT = 2;

bool commandExists(const char *command)
{
    if (command == NULL)
        return false;

    if (!strcmp(command, COMMAND_EXIT))
        return true;
    if (!strcmp(command, COMMAND_HELP))
        return true;
    if (!strcmp(command, COMMAND_1))
        return true;
    if (!strcmp(command, COMMAND_2))
        return true;


    return false;
}

// COMMANDS

void commandHelp()
{
    printf("\t%s - command 1 (admin only)\n", COMMAND_1);
    printf("\t%s - command 2 (operator only)\n", COMMAND_2);
    printf("\t%s - display this help\n", COMMAND_HELP);
    printf("\t%s - exit\n", COMMAND_EXIT);
}

void command1()
{
    printf("command 1 executed\n");
}

void command2()
{
    printf("command 2 executed\n");
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
