#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFSIZE 4096 //每次读写4096字节

int main()
{
    int fd;
    FILE *outfile = NULL;
    FILE *infile = NULL;
    FILE *endfile = NULL;
    FILE *finalfile = NULL;
    int i;
    int j;
    int k;
    int flag = 0;
    int pt, length;
    pid_t pid;
    ssize_t n;
    char buf[BUFFSIZE];
    char outname[10];
    int outid = 1;
    char inname[10];
    int inid = 1;
    char endname[10];
    int endid = 1;

    if ((fd = open("./5b_sample.csv", O_RDONLY)) < 0) //open文件 ./5b_sample.csv
    {
        printf("csv文件无法打开！\n");
        exit(1);
    }

    sprintf(outname, "%d", outid);
    strcat(outname, ".csv");
    outfile = fopen(outname, "w"); //open文件 outname, w

    memset(buf, 0, sizeof(buf));

    while ((n = read(fd, buf, BUFFSIZE - 1)) > 0) //读取csv内容到buf字符数组里，每次BUFFSIZE个char
    {
        i = 0;
        pt = 0;
        length = 0;

        while (pt < n)
        {
            while (buf[i] != '\n')
            {
                if (buf[i] == '\0')
                {
                    flag = 1;
                    break;
                }
                i++;
            }

            i++;
            length = i - pt;
            if (flag)
            {
                length--;
                flag = 0;
                fwrite(buf + pt, sizeof(char), length, outfile);
            }
            else
            {
                fwrite(buf + pt, sizeof(char), length, outfile);
                fclose(outfile); //一行结束，关掉写下一个文件
                outid++;
                sprintf(outname, "%d", outid);
                strcat(outname, ".csv");
                outfile = fopen(outname, "w"); //open文件 outname, w
                pt = i;
            }
        }

        memset(buf, 0, sizeof(buf));
    }
    fclose(outfile);
    close(fd);
    printf("bin文件拆分完成！\n");  //父进程拆分文件为一个个小csv文件完成

    while (inid <= outid)
    {
        if ((pid = fork()) < 0)
        {
            printf("fork error!");
            exit(1);
        }
        else if (pid == 0)  //子进程转换每个小csv文件为一个bin文件
        {
            sprintf(inname, "%d", inid);
            sprintf(endname, "%d", inid);
            strcat(inname, ".csv");
            infile = fopen(inname, "r");
            endfile = fopen(endname, "wb");
            memset(buf, 0, sizeof(buf));
            while ((n = fread(buf, sizeof(char), BUFFSIZE, infile)) > 0)
                fwrite(buf, sizeof(char), n, endfile);
            fclose(infile);
            fclose(endfile);
            exit(0);
        }
        else    //父进程将转换后的bin文件重组为一个最终文件 bin
        {
            waitpid(pid, NULL, 0);
            inid++;
            if (inid > outid)
            {
                finalfile = fopen("bin", "wb");
                while (endid <= outid)
                {
                    sprintf(endname, "%d", endid);
                    endfile = fopen(endname, "rb");
                    memset(buf, 0, sizeof(buf));
                    while ((n = fread(buf, sizeof(char), BUFFSIZE, endfile)) > 0)
                        fwrite(buf, sizeof(char), n, finalfile);
                    fclose(endfile);
                    endid++;
                }
            }
        }
    }
    fclose(finalfile);
    printf("bin文件重组完成！\n");
    return 0;
}