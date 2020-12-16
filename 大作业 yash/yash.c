#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>            //sleep用到
#include <readline/readline.h> //readline,好东西啊，tab自动补齐，上下记录历史输入命令
#include <readline/history.h>
#include <sys/wait.h>
#include <sys/types.h>
#include "yash.h"

void ShowHistory() //显示命令history
{
    int i = 0;
    HIST_ENTRY **his;
    his = history_list();
    while (his[i] != NULL)
    {
        printf("\t%d\t%s\n", i + 1, his[i]->line);
        i++;
    }
}

void Init() //加入环境变量；除了exit，不能用ctrl+c来退出yash
{
    extern char **environ;
    VLenviron2table(environ);

    signal(SIGINT, SIG_IGN);
    signal(SIGQUIT, SIG_IGN);
    signal(SIGTSTP, SIG_IGN);
}

void Fatal(char *s1, char *s2, int n) //出大问题啊，这不应该出现的
{
    fprintf(stderr, "Error: %s, %s\n", s1, s2);
    exit(n);
}

int main()
{
    int exec_result;
    char **arglist;
    char *cmdline;
    char directory[256];
    char directory2[256];
    char *dir;
    memset(directory, 0, sizeof(directory));
    memset(directory2, 0, sizeof(directory2));

    Init();

    strcat(directory, BEGIN(49, 33));
    dir = getcwd(directory2, sizeof(directory2));
    strcat(directory, dir);
    strcat(directory, " $ ");
    strcat(directory, CLOSE);
    //GNU readline 库函数,tab自动补齐，上下记录历史输入命令
    while ((cmdline = readline(directory)) != NULL)
    {
        add_history(cmdline); //GNU readline库，记录命令history
        write_history("/home/kynali/桌面/yash/history");

        if ((arglist = SplitLine(cmdline)) != NULL)
        {
            exec_result = Process(arglist);
            FreeList(arglist);
        }
        free(cmdline);
        if (exec_result == -1)
            return 0;

        memset(directory, 0, sizeof(directory));
        memset(directory2, 0, sizeof(directory2));
        strcat(directory, BEGIN(49, 33));
        dir = getcwd(directory2, sizeof(directory2));
        strcat(directory, dir);
        strcat(directory, " $ ");
        strcat(directory, CLOSE);
    }
    printf("readline出错!被强制干掉了？\n");
    return 0;
}