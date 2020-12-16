#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "yash.h"

int assign(char *);
int okname(char *);
extern int background;

int BuiltinCommand(char **args, int *resultp)
{
    int rv = 1;
    int i;
    if (strcmp(args[0], "set") == 0)
    {
        VLlist();
        *resultp = 0;
        rv = 0;
    }
    else if (strchr(args[0], '=') != NULL)
    {
        *resultp = assign(args[0]);
        if (*resultp != -1)
            rv = 0;
    }
    else if (strcmp(args[0], "$") == 0)
    {
        rv = 6;
    }
    else if (strcmp(args[0], "export") == 0)
    {
        if (args[1] != NULL && okname(args[1]))
            *resultp = VLexport(args[1]);
        else
            *resultp = 1;
        rv = 0;
    }
    else if (args[1])
    {
        if (strcmp(args[1], ">") == 0)
            rv = 2;
        else if (strcmp(args[1], "<") == 0)
        {
            rv = 3;
            if (args[3] != NULL)
            {
                if (strcmp(args[3], ">") == 0)
                {
                    rv = 4;
                }
            }
        }
    }
    for (i = 1; args[i]; i++)
    {
        if (strcmp(args[i], "&") == 0)
        {
            background = 1;
            //free(args[i]);
            break;
        }
    }
    for (i = 1; args[i]; i++)
    {
        if (strcmp(args[i], "|") == 0)
        {
            pipeHandler(args);
            rv = 9;
            break;
        }
    }
    return rv;
}

int assign(char *str)
{
    char *cp;
    int rv;
    cp = strchr(str, '=');
    *cp = '\0';
    rv = (okname(str) ? VLstore(str, cp + 1) : -1);
    *cp = '=';
    return rv;
}

int okname(char *str)
{
    char *cp;
    for (cp = str; *cp; cp++)
    {
        if ((isdigit(*cp) && cp == str) || !(isalnum(*cp) || *cp == '_'))
            return 0;
    }
    return cp != str;
}