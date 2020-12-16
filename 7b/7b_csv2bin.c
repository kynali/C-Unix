#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFSIZE 4096 //每次读写4096字节

int main()
{
    int fd;
    FILE *outfile = NULL;
    FILE *endfile = NULL;
    FILE *finalfile = NULL;
    int i;
    int j;
    int k;
    int pt, length;
    pid_t pid;
    ssize_t n;
    char buf[BUFFSIZE];
    char outname[10];
    char endname[10];
    int endid = 1;

    int pp[1000][2]; //准备1000个管道
    int pipenum = 0; //需要的管道数（子进程数、文件行数）

    if ((fd = open("./5b_sample.csv", O_RDONLY)) < 0) //open文件 ./5b_sample.csv
    {
        printf("csv文件无法打开！\n");
        exit(1);
    }

    memset(buf, 0, sizeof(buf));

    while ((n = read(fd, buf, BUFFSIZE - 1)) > 0) //读取csv内容到buf字符数组里，每次BUFFSIZE个char
    {
        i = 0;
        pt = 0;
        length = 0;

        while (pt < n)
        {
            while (buf[i] != '\n') //找换行符
            {
                if (buf[i] == '\0') //若buf已经结束了
                    break;
                i++;
            }

            i++;
            length = i - pt;

            pipe(pp[pipenum]);

            if ((pid = fork()) < 0)
            {
                printf("fork error!");
                exit(1);
            }
            else if (pid == 0) //子进程转换一行为一个bin文件
            {
                int len = read(pp[pipenum][0], buf, BUFFSIZE); //子进程从管道中读出数据
                sprintf(outname, "%d", pipenum + 1);
                outfile = fopen(outname, "wb");
                fwrite(buf, sizeof(char), len, outfile); //完成小bin文件的创建
                fclose(outfile);
                read(pp[pipenum][0], buf, BUFFSIZE); //再读管道，用于阻塞，等待分发完成
                usleep(10000);
                write(pp[pipenum][1], "ok", sizeof("ok")); //等10ms，返回完成指令
                exit(0);
            }
            else
            {
                waitpid(pid, NULL, 1);
                write(pp[pipenum][1], buf + pt, length);
                pt = i;
                pipenum++;
            }
        }
        memset(buf, 0, sizeof(buf));
    }
    close(fd); //分发完成
    printf("csv文件分发完成！\n");

    for (k = 0; k < pipenum; k++)
    {
        write(pp[k][1], "123", sizeof("123")); //告知各个子进程，我已分发完毕
        read(pp[k][0], buf, BUFFSIZE);         //获取子进程返回的转换完成命令
    }
    printf("各个bin文件转换完成！\n");

    finalfile = fopen("bin", "wb");
    for (k = 1; k <= pipenum; k++)
    {
        sprintf(endname, "%d", k);
        endfile = fopen(endname, "rb");
        memset(buf, 0, sizeof(buf));
        while ((n = fread(buf, sizeof(char), BUFFSIZE, endfile)) > 0)
            fwrite(buf, sizeof(char), n, finalfile);
        fclose(endfile);
    }
    fclose(finalfile);
    printf("bin文件重组完成！\n");
    return 0;
}