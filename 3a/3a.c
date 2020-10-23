#include <stdio.h>
#include <string.h>

#define Max_l 50    //单词最长长度
#define Max_n 10000 //最多单词个数

int main()
{
    FILE *fp = NULL;
    char tokens[Max_n][Max_l];     //存单词
    char repeat[Max_n / 2][Max_l]; //存重复的单词
    int i = 0;
    int j = 0;
    int k = 0;
    int length = 0;   //tokens中元素个数
    int r_length = 0; //repeat中元素个数
    int flag = 0;     //是否是新的重复单词

    for (i = 0; i < Max_n; i++) //初始化
        memset(tokens[i], 0, sizeof(tokens[i]));
    for (i = 0; i < Max_n / 2; i++)
        memset(repeat[i], 0, sizeof(repeat[i]));

    fp = fopen("./3a.txt", "r"); //读文件，存入单词数组tokens
    while (1)
    {
        if (feof(fp))
            break;
        fscanf(fp, "%s", tokens[i]);
        i++;
        printf("%s ", tokens[i - 1]);
    }
    fclose(fp);

    length = i;
    for (i = 0; i < length; i++)
    {
        for (j = 1; j < 25; j++)
        {
            if ((i + j) == length) //最后几个单词的比较不能越界
                break;
            if (!strcmp(tokens[i], tokens[i + j])) //比较，若有重复
            {
                flag = 0;
                for (k = 0; k < r_length; k++) //判断是否是新的重复单词
                {
                    if (!strcmp(tokens[i], repeat[k]))
                    {
                        flag = 1; //不是新的，不管
                        break;
                    }
                }
                if (!flag) //若是新的，记录下来
                {
                    strcpy(repeat[r_length], tokens[i]);
                    r_length++;
                }
                break; //知道有重复，就不再比较后面的了
            }
        }
    }

    putchar('\n');
    printf("\n重复单词：");
    for (i = 0; i < r_length; i++) //输出重复单词
    {
        if (i % 15 == 0)
            putchar('\n');
        printf("%s  ", repeat[i]);
    }
    putchar('\n');
    putchar('\n');

    return 0;
}
