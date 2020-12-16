#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "yash.h"

char *tokens[ARGLEN];

char **SplitLine(char *str) //命令解析，拆！
{
    int i = 0;
    const char s[2] = " ";
    char *temp;

    tokens[i] = malloc(MAXARGS);
    temp = strtok(str, s);

    while (temp != NULL)
    {
        strcpy(tokens[i], temp);
        //printf("%s\n", tokens[i]);
        i++;
        tokens[i] = malloc(MAXARGS);
        temp = strtok(NULL, s);
    }
    free(tokens[i]);
    tokens[i] = 0;
    return tokens;
}

void FreeList(char **list) //把字符串数组tokens里的元素都free掉
{
    char **cp = list;
    while (*cp)
    {
        free(*cp);
        cp++;
    }
}