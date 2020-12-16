#include <stdio.h>
#include <string.h>
#include "yash.h"

enum states
{
    WANT_IF,
    WANT_THEN,
    AFTER_THEN,
    AFTER_ELSE,
};
enum results
{
    SUCCESS,
    FAIL
};

int envar = 0;

static int if_state = WANT_IF;
static int if_result = SUCCESS;
static int last_stat = WANT_IF;

int SynErr();

int OkToExecute()
{
    int rv = 0;
    if (if_state == WANT_THEN)
    {
        SynErr();
        rv = 0;
    }
    else if (if_state == WANT_IF)
        rv = 1;
    else if (if_state == AFTER_THEN && if_result == SUCCESS)
        rv = 1;
    else if (if_state == AFTER_ELSE && if_result == FAIL)
        rv = 1;
    return rv;
}

int IsControlCommand(char *s)
{
    return (strcmp(s, "if") == 0 || strcmp(s, "then") == 0 || strcmp(s, "else") == 0 || strcmp(s, "fi") == 0);
}

int DoControlCommand(char **args)
{
    char *cmd = args[0];
    int rv = -1;
    if (strcmp(cmd, "if") == 0)
    {
        if (if_state != WANT_IF)
            rv = SynErr();
        else
        {
            last_stat = Process(args + 1);
            if_result = (last_stat == 0 ? SUCCESS : FAIL);

            if (strcmp(args[1], "$") == 0)
            {
                envar = VLlookup(args[2])[0] - '0';
                if (envar)
                    if_result = SUCCESS;
                else
                    if_result = FAIL;
            }
            if_state = WANT_THEN;
            rv = 0;
        }
    }
    else if (strcmp(cmd, "then") == 0)
    {
        if (if_state != WANT_THEN)
            rv = SynErr();
        else
        {
            if_state = AFTER_THEN;
            rv = 0;
        }
    }
    else if (strcmp(cmd, "else") == 0)
    {
        if (if_state != AFTER_THEN)
            rv = SynErr();
        else
        {
            if_state = AFTER_ELSE;
            rv = 0;
        }
    }
    else if (strcmp(cmd, "fi") == 0)
    {
        if (if_state != AFTER_THEN && if_state != AFTER_ELSE)
            rv = SynErr();
        else
        {
            if_state = WANT_IF;
            rv = 0;
        }
    }
    else
        Fatal("奇怪的事情发生了，这是if then else fi里的一个吗？: ", cmd, 2);
    return rv;
}

int SynErr()
{
    if_state = WANT_IF;
    printf("错了错了，重新来一遍。\n");
    return 0;
}
