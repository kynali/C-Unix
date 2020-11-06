#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFSIZE 4096 //每次读写4096字节

int main()
{
    int fd;
    FILE *binfile = NULL;
    int i;
    int j = 0;
    ssize_t n;
    char buf[BUFFSIZE];

    if ((binfile = fopen("5b_bin_outfile", "rb")) == NULL) //open文件, rb二进制
    {
        printf("二进制文件无法打开！\n");
        exit(1);
    }

    while ((n = fread(buf, sizeof(char), BUFFSIZE, binfile)) > 0) //读取bin内容到buf字符数组里，每次BUFFSIZE个char
    {
        for (i = 0; i < strlen(buf); i++)
        {
            if (buf[i] == ',') //看到“，”，就要加制表符，加1或者2个，视字段长决定
            {
                j = (i - j) / 8 + 1;
                while (j < 3)
                {
                    putchar('\t');
                    j++;
                }
                j = i + 1;
            }
            else if (buf[i] == '\n') //换行不影响字段长，不影响制表符的添加
            {
                putchar(buf[i]);
                j = i + 1;
            }
            else
                putchar(buf[i]);
        }
    }

    fclose(binfile); //关闭文件
    printf("\nbin文件打印完成！\n");
    exit(0);
}