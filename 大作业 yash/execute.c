#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>
#include "yash.h"

extern char **environ;
int background = 0;
struct PIDset
{
    int pid;
    char name[100];
    int state;
} pidset[10];
int pidsetnum = 0;

int c_pid = 0;
int cpidnum = 0;

void getNameByPid(pid_t pid, char *task_name)
{
    char proc_pid_path[1024];
    char buf[1024];
    sprintf(proc_pid_path, "/proc/%d/status", pid);
    FILE *fp = fopen(proc_pid_path, "r");
    if (NULL != fp)
    {
        if (fgets(buf, 1023, fp) == NULL)
        {
            fclose(fp);
        }
        fclose(fp);
        sscanf(buf, "%*s %s", task_name);
    }
}

void handler1()
{
    signal(SIGTSTP, handler1);
    if (c_pid > 0)
    {
        printf("进程挂起 PID: %d\n", c_pid);
        for (int i = 0; i < pidsetnum; i++)
        {
            if (pidset[i].pid == c_pid)
            {
                pidset[i].state = 0;
                kill(c_pid, SIGSTOP);
                c_pid = 0;
                return;
            }
        }
        pidset[pidsetnum].pid = c_pid;
        getNameByPid(c_pid, pidset[pidsetnum].name);
        pidset[pidsetnum].state = 0; //后台，挂起
        pidsetnum++;
        kill(c_pid, SIGSTOP);
        c_pid = 0;
    }
}

void handler2()
{
    signal(SIGINT, handler2);
    if (c_pid > 0)
    {
        kill(c_pid, SIGTERM);
        c_pid = 0;
        for (int i = cpidnum - 1; i < pidsetnum; i++)
        {
            pidset[i].pid = pidset[i + 1].pid;
            strcpy(pidset[i].name, pidset[i + 1].name);
            pidset[i].state = pidset[i + 1].state;
        }
        pidsetnum--;
    }
}

int Execute(char *argv[], int flag) //exec函数，执行命令
{
    int fd;              //文件描述符
    int child_info = -1; //返回值，wait()的结果

    if (argv[0] == NULL) //没有参数可供执行
        return 1;
    if (!strcmp(argv[0], "history") && (argv[1] == NULL)) //history不是PATH下的命令，所以用我自己写的show history来看
    {
        ShowHistory();
        return 0;
    }
    if (!strcmp(argv[0], "jobs") && (argv[1] == NULL))
    {
        ShowJobs();
        return 0;
    }
    if (!strcmp(argv[0], "fg"))
    {
        if (argv[1] != NULL)
        {
            if (Fg(argv[1]))
            {
                cpidnum = argv[1][0] - '0';
                signal(SIGTSTP, handler1);
                signal(SIGINT, handler2);
                waitpid(c_pid, NULL, WUNTRACED);
            }
        }
        return 0;
    }
    if (!strcmp(argv[0], "bg"))
    {
        if (argv[1] != NULL)
            Bg(argv[1]);
        return 0;
    }
    if (!strcmp(argv[0], "shutbg"))
    {
        if (argv[1] != NULL)
            Sg(argv[1]);
        return 0;
    }
    if (!strcmp(argv[0], "exit") && (argv[1] == NULL)) //exit
    {
        return -1;
    }
    if (!strcmp(argv[0], "cd"))
    {
        if (argv[1] == NULL)
        {
            chdir(getenv("HOME"));
            return 0;
        }
        else
        {
            if (chdir(argv[1]) == -1)
            {
                printf(" %s: no such directory\n", argv[1]);
                return 1;
            }
        }
        return 0;
    }
    if (flag == 9)
    {
        return 0;
    }
    if ((c_pid = fork()) == -1) //fork出错
    {
        printf("子进程fork失败。\n");
        return 1;
    }
    else if (c_pid == 0) //子进程，执行命令
    {
        environ = VLtable2environ(); //执行命令即更新环境变量

        signal(SIGINT, SIG_IGN); //子进程ctrl信号由父进程给出
        signal(SIGQUIT, SIG_IGN);
        signal(SIGTSTP, SIG_IGN);
        //signal(SIGUSR1, getit);

        if (flag == 2)
        {
            close(1);
            fd = creat(argv[2], 0644);
            execlp(argv[0], argv[0], NULL);
            perror("完蛋啦，你这啥命令啊，exec失败了呀！"); //出错
            exit(1);
        }
        else if (flag == 3)
        {
            close(0);
            fd = open(argv[2], O_RDONLY);
            execlp(argv[0], argv[0], NULL);
            perror("完蛋啦，你这啥命令啊，exec失败了呀！"); //出错
            exit(1);
        }
        else if (flag == 4)
        {
            close(0);
            fd = open(argv[2], O_RDONLY);
            close(1);
            fd = creat(argv[4], 0644);
            execlp(argv[0], argv[0], NULL);
            perror("完蛋啦，你这啥命令啊，exec失败了呀！"); //出错
            exit(1);
        }
        else if (flag == 6)
        {
            if (argv[1] != NULL)
            {
                printf("%s\n", VLlookup(argv[1]));
            }
            exit(0);
        }
        else
        {
            execvp(argv[0], argv);                          //exec函数执行
            perror("完蛋啦，你这啥命令啊，exec失败了呀！"); //出错
            exit(1);
        }
    }
    else //爹进程，等着崽
    {
        if (background == 0)
        {
            signal(SIGTSTP, handler1);
            signal(SIGINT, handler2);

            waitpid(c_pid, &child_info, WUNTRACED);
        }
        else
        {
            usleep(100000);
            printf("后台运行进程 PID: %d\n", c_pid);
            pidset[pidsetnum].pid = c_pid;
            getNameByPid(c_pid, pidset[pidsetnum].name);
            pidset[pidsetnum].state = 1;
            pidsetnum++;
            child_info = 0;
            background = 0;
            c_pid = 0;
            //waitpid(c_pid, NULL, WNOHANG);
        }
    }
    return child_info; //返回wait的结果
}