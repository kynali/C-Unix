#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/types.h>
#include "yash.h"

void pipeHandler(char *args[])
{
    int filedes[2]; // [0]是管道输出, [1]是管道输入
    int filedes2[2];
    int num_cmds = 0;   // 管道数
    char *command[100]; //存放命令
    pid_t pid;
    int fd;

    int err = -1;
    int end = 0;

    int i = 0; // 计数用
    int j = 0;
    int k = 0;
    int l = 0;
    int m = 0;
    // 找找有几个 |，计算管道数
    while (args[l] != NULL)
    {
        if (strcmp(args[l], "|") == 0)
            num_cmds++;
        l++;
    }
    num_cmds++;

    // Main loop of this method. For each command between '|', the
    // pipes will be configured and standard input and/or output will
    // be replaced. Then it will be executed
    while (args[j] != NULL && end != 1)
    {
        k = 0;
        while (strcmp(args[j], "|") != 0)
        {
            command[k] = args[j]; //存放命令
            j++;
            if (args[j] == NULL)
            {
                end = 1; //没有参数了，end
                k++;
                break;
            }
            k++;
        }

        command[k] = NULL; //用于exec
        j++;

        // Depending on whether we are in an iteration or another, we
        // will set different descriptors for the pipes inputs and
        // output. This way, a pipe will be shared between each two
        // iterations, enabling us to connect the inputs and outputs of
        // the two different commands.
        if (i % 2 != 0)
        {
            pipe(filedes); // 奇数 i 1 3 5
        }
        else
        {
            pipe(filedes2); // 偶数 i 0 2 4
        }

        pid = fork();
        if (pid == -1)
        {
            if (i != num_cmds - 1)
            {
                if (i % 2 != 0)
                {
                    close(filedes[1]); // 奇数
                }
                else
                {
                    close(filedes2[1]); // 偶数
                }
            }
            printf("子进程无法建立\n");
            return;
        }
        if (pid == 0)
        {
            if (i == 0) //第一个命令，设置管道2的输出
            {
                dup2(filedes2[1], STDOUT_FILENO);
            }
            else if (i == num_cmds - 1) //最后一个命令，设置输入
            {
                if (num_cmds % 2 != 0)
                { // 奇数
                    dup2(filedes[0], STDIN_FILENO);
                }
                else
                { // 偶数
                    dup2(filedes2[0], STDIN_FILENO);
                }
                if (command[1] != NULL)
                {
                    for (m = 1; m < k; m++)
                    {
                        if (!strcmp(command[m], ">"))
                        {
                            close(1);
                            fd = creat(command[m + 1], 0644);
                            command[m] = NULL;
                            command[m + 1] = NULL;
                            break;
                        }
                    }
                }
            }
            else // 中间状态，用两个管道
            {    // 奇数
                if (i % 2 != 0)
                {
                    dup2(filedes2[0], STDIN_FILENO);
                    dup2(filedes[1], STDOUT_FILENO);
                }
                else
                { // 偶数
                    dup2(filedes[0], STDIN_FILENO);
                    dup2(filedes2[1], STDOUT_FILENO);
                }
            }

            if (execvp(command[0], command) == err)
                kill(getpid(), SIGTERM); //出错，SIGTERM
        }

        // 父进程，关闭管道文件
        if (i == 0)
            close(filedes2[1]);
        else if (i == num_cmds - 1)
        {
            if (num_cmds % 2 != 0)
            {
                close(filedes[0]);
            }
            else
            {
                close(filedes2[0]);
            }
        }
        else
        {
            if (i % 2 != 0)
            {
                close(filedes2[0]);
                close(filedes[1]);
            }
            else
            {
                close(filedes[0]);
                close(filedes2[1]);
            }
        }

        waitpid(pid, NULL, 0);

        i++;
    }
}