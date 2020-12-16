#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include "yash.h"

extern struct PIDset
{
    int pid;
    char name[100];
    int state;
} pidset[100];
extern int pidsetnum;

extern int c_pid;

void ShowJobs()
{
    int temp = pidsetnum;
    int i = 0;
    while (i < temp)
    {
        if (pidset[i].state)
            printf("    %d    %d    %s    运行中\n", i + 1, pidset[i].pid, pidset[i].name);
        else
            printf("    %d    %d    %s    已停止\n", i + 1, pidset[i].pid, pidset[i].name);
        i++;
    }
}

int Fg(char *argv)
{
    int num = argv[0] - '0';
    if (pidset[num - 1].state)
    {
        c_pid = pidset[num - 1].pid;
        return 1;
    }
    else
    {
        printf("还没bg\n");
        return 0;
    }
}

void Bg(char *argv)
{
    int num = argv[0] - '0';
    kill(pidset[num - 1].pid, SIGCONT);
    pidset[num - 1].state = 1;
}

void Sg(char *argv)
{
    int num = argv[0] - '0';
    kill(pidset[num - 1].pid, SIGKILL);
    for (int i = num - 1; i < pidsetnum; i++)
    {
        pidset[i].pid = pidset[i + 1].pid;
        strcpy(pidset[i].name, pidset[i + 1].name);
        pidset[i].state = pidset[i + 1].state;
    }
    pidsetnum--;
}