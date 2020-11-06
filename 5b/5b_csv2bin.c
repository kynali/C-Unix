#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFSIZE 4096   //每次读写4096字节

int main()
{
    int fd;
    FILE *outfile = NULL;
    int i;
    ssize_t n;
    char buf[BUFFSIZE];

    if ((fd = open("./5b_sample.csv", O_RDONLY)) < 0)   //open文件 ./5b_sample.csv
    {
        printf("csv文件无法打开！\n");
        exit(1);
    }
    if ((outfile = fopen("5b_bin_outfile", "wb")) == NULL)     //open文件 outfile, wb二进制
    {
        printf("二进制文件无法打开！\n");
        exit(1);
    }

    while ((n = read(fd, buf, BUFFSIZE)) > 0)   //读取csv内容到buf字符数组里，每次BUFFSIZE个char
    {
        if (fwrite(buf, sizeof(char), n, outfile) != n) //将buf字符数组中读到的内容写入bin文件中，因为是char，所以就是big-endian的
        {
            printf("bin文件写入出错！\n");
            exit(1);
        }
    }

    fclose(outfile);    //关闭文件
    close(fd);
    printf("bin文件写入完成！\n");
    exit(0);
}