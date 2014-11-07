#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sqlite3.h>

#ifdef WIN32
#include <windows.h>
#else
#include <termios.h>
#include <unistd.h>
#endif

#include "bool.h"
#include "dbinterface.h"
#include "commands.h"

// for scanf
#define XSTR(A) STR(A)
#define STR(A) #A

#define MAX_LOGIN_SIZE 64
#define MAX_PASSWORD_SIZE 64
#define MAX_COMMAND_SIZE 256
#define PASSWORD_RETRY_COUNT 3

static const char DEFAULT_DB_FILENAME[] = "../db.sqlite";

void setStdinEcho(bool enable)
{
#ifdef WIN32
    HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
    DWORD mode;
    GetConsoleMode(hStdin, &mode);

    if(!enable)
        mode &= ~ENABLE_ECHO_INPUT;
    else
        mode |= ENABLE_ECHO_INPUT;

    SetConsoleMode(hStdin, mode );

#else
    struct termios tty;
    tcgetattr(STDIN_FILENO, &tty);
    if( !enable )
        tty.c_lflag &= ~ECHO;
    else
        tty.c_lflag |= ECHO;

    (void) tcsetattr(STDIN_FILENO, TCSANOW, &tty);
#endif
}

int main(int argc, char *argv[])
{
    char *dbFilename = NULL;
    sqlite3 *db;
    char login[MAX_LOGIN_SIZE + 1];
    char password[MAX_PASSWORD_SIZE + 1];
    int retryCount;

    char command[MAX_COMMAND_SIZE + 1];
    char *cmd;
    int cmdLength;

    // read cmdline params
    if (argc > 2)
        return printf("error: too many parameters\nusage: %s [filename.sqlite]\n", argv[0]);

    if (argc > 1)
    {
        dbFilename = malloc(strlen(argv[1]) + 1);
        strcpy(dbFilename, argv[1]);
    }
    else
    {
        dbFilename = malloc(strlen(DEFAULT_DB_FILENAME) + 1);
        strcpy(dbFilename, DEFAULT_DB_FILENAME);
        printf("using default database file: %s\n", DEFAULT_DB_FILENAME);
    }

    // open database file
    if (sqlite3_open_v2(dbFilename, &db, SQLITE_OPEN_READONLY, NULL) == SQLITE_OK)
        printf("opened database file %s\n", dbFilename);
    else
        return printf("failed to open database file: %s\n", dbFilename);
    free(dbFilename);

    // login
Login:
    printf("login: ");
    while (scanf("%"XSTR(MAX_LOGIN_SIZE)"s", login) != 1) {}
    while (!loginExists(db, login))
    {
        printf("login %s does not exist\n", login);
        printf("login: ");
        while (scanf("%"XSTR(MAX_LOGIN_SIZE)"s", login) != 1) {}
    }

    // password
    retryCount = 1;
    printf("password: ");
    setStdinEcho(false);    // hide password input
    while (scanf("%"XSTR(MAX_PASSWORD_SIZE)"s", password) != 1) {}
    setStdinEcho(true);
    printf("\n");
    while (!checkPassword(db, login, password))
    {
        sleep(2);
        printf("wrong password, try again\n");
        printf("password: ");
        setStdinEcho(false);    // hide password input
        while (scanf("%"XSTR(MAX_PASSWORD_SIZE)"s", password) != 1) {}
        setStdinEcho(true);
        printf("\n");
        if ((++retryCount) >= PASSWORD_RETRY_COUNT)
        {
            printf("sorry, failed to login, try again\n\n");
            goto Login;
        }
    }

    // commands scanning loop
    if (fgets(command, MAX_COMMAND_SIZE, stdin) != NULL) {}        // skip any trash left in stdin
    while (1)
    {
        printf("%s> ", login);
        if (fgets(command, MAX_COMMAND_SIZE, stdin) == NULL)
            continue;

        // get first word from command string
        cmdLength = 0;
        while ((command[cmdLength] != '\0') && (!isspace(command[cmdLength])))
            cmdLength++;
        if (cmdLength == 0)     // empty command
            continue;
        cmd = malloc(cmdLength + 1);
        strncpy(cmd, command, cmdLength);
        cmd[cmdLength] = '\0';

        // check command
        if (!commandExists(cmd))
        {
            printf("%s: command not found\n", cmd);
            free(cmd);
            continue;
        }

        // check permissions
        if (!checkCommandPermission(db, login, cmd))
        {
            free(cmd);
            printf("sorry, you don't have permission\n");
            continue;
        }

        // check exit command
        if (!strcmp(cmd, COMMAND_EXIT))
        {
            free(cmd);
            sqlite3_close(db);
            return 0;
        }

        // run command
        executeCommand(db, command);

        free(cmd);
    }

    return 0;
}
