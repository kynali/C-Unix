#include <stdio.h>
#include "yash.h"

int IsControlCommand(char *); //Process  附属函数
int DoControlCommand(char **);
int OkToExecute();

int Process(char **args)
{
    int rv = 0;
    int flag;
    if (args[0] == NULL) //判断是控制语句if then else fi，还是要执行的命令
        rv = 0;
    else if (IsControlCommand(args[0]))
        rv = DoControlCommand(args);
    else if (OkToExecute())
    {
        if (flag = BuiltinCommand(args, &rv)) //还要判断是不是内置命令
            rv = Execute(args, flag);
    }
    return rv;
}