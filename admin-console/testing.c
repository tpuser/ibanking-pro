#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bool.h"
#include "dbinterface.h"
#include "commands.h"
#include "admin_command_cust.h"

// for scanf
#define XSTR(A) STR(A)
#define STR(A) #A

static const char DEFAULT_DB_FILENAME[] = "../db.sqlite";

void logTestResult(const char *msg, bool result)
{
    if (result)
        printf("test passed: %s\n", msg);
    else
        printf("TEST FAILED: %s\n", msg);
}

int main(int argc, char *argv[])
{
    bool status = true;

    // test function commandExists
    if (!commandExists("help"))
        status = false;
    logTestResult("commandExists(\"help\");", status);

    if (commandExists("notexists"))
        status = false;
    logTestResult("commandExists(\"notexists\");", status);

    return (!status);
}
