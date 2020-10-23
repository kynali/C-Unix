#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define Num 100    //数字的个数
#define Max_n 1001 //支持的运算位数

char result[Max_n * 2];
char suffix[Max_n * 20];      //后缀表达式
char ret[2];                  //字符串栈进行弹出时的返回值（弹出元素）
char tokens[Num][Max_n * 2];  //令牌，将中缀表达式分解后的字符串数组
char ctokens[Num][Max_n * 2]; //令牌，将后缀表达式分解后的字符串数组
int count;                    //tokens令牌的个数
char value[Max_n * 2];        //表达式的值
int l = 0;                    //ctokens令牌的个数
typedef struct stack1
{ //操作符栈
    int top;
    char elem[Num][2];
} stacki;
typedef struct stack2
{ //操作数栈
    int top;
    char elem[Num][Max_n * 2];
} stackj;
typedef stacki *STACK;  //指向操作符栈的指针
typedef stackj *DSTACK; //指向操作数栈的指针

void toSuffix(char *str);                          //将中缀表达式转化成后缀表达式
int PreParse(char tokens[][Max_n * 2], char *str); //将中缀表达式分解成令牌返回令牌中字符串个数
int GetProsity(char *ch);                          //获得操作符的优先级并返回
void push(STACK S, char *opt);                     //操作符栈的入栈
char *pop(STACK S);                                //操作符栈的出栈，并返回出栈元素
int IsEmpty(STACK S);                              //判断操作符栈是否为空
void MakeNull(STACK S);                            //将操作符栈制空
void PrintStack(STACK S);                          //打印操作符栈
void dpush(DSTACK S, char *opd);                   //操作符数的入栈
void dpop(DSTACK S);                               //操作符数的出栈
void PrintStack1(DSTACK S);                        //打印操作数栈
void MakeNull1(DSTACK S);                          //将操作数栈制空
char *Evalute(char ctokens[][Max_n * 2]);          //计算后缀表达式的值并返回

char *float_plus(char *a, char *b); //加法

int main()
{
    char str[Num];
    printf("请输入算术表达式：\n");
    gets(str);     //输入所求中缀表达式str
    toSuffix(str); //将中缀表达式转化成后缀表达式
    printf("后缀表达式形式为：  ");
    puts(suffix); //输出转化后的后缀表达式
    putchar('\n');
    strcpy(value, Evalute(ctokens)); //计算后缀表达式的值
    printf("\n计算结果为：  ");
    puts(value);
    putchar('\n');
    return 0;
}

void toSuffix(char *str)
{
    int i;
    stacki stacks;            //定义一个操作符栈stacks
    STACK optstack = &stacks; //定义操作符栈指针optstack
    MakeNull(optstack);
    count = PreParse(tokens, str);
    for (i = 0; i < count; i++)
    { //不是运算符，而是数
        if (!(strcmp(tokens[i], "+") && strcmp(tokens[i], "-") && strcmp(tokens[i], "*") && strcmp(tokens[i], "/") && strcmp(tokens[i], "(") && strcmp(tokens[i], ")") && strcmp(tokens[i], "^") && strcmp(tokens[i], "s") && strcmp(tokens[i], "c")))
        {
            if (IsEmpty(optstack)) //栈空，数入栈
            {
                push(optstack, tokens[i]);
                //PrintStack(optstack);
            }
            else
            {
                if (!strcmp(tokens[i], "("))
                {
                    push(optstack, tokens[i]);
                    //PrintStack(optstack);
                }
                else if (!strcmp(tokens[i], ")"))
                {
                    while (strcmp(optstack->elem[optstack->top], "(")) //循环直到遇见左括号
                    {
                        strcpy(ctokens[l], optstack->elem[optstack->top]);
                        l++;
                        strcat(suffix, pop(optstack));
                        PrintStack(optstack);
                    }
                    pop(optstack); //左括号弹出
                    //PrintStack(optstack);
                }
                else
                {
                    if (GetProsity(tokens[i]) > GetProsity(optstack->elem[optstack->top]))
                    {
                        push(optstack, tokens[i]);
                        //PrintStack(optstack);
                    }
                    else
                    {
                        while (optstack->top < 100)
                        {
                            if (GetProsity(tokens[i]) <= GetProsity(optstack->elem[optstack->top]))
                            {
                                strcpy(ctokens[l], optstack->elem[optstack->top]);
                                l++;
                                strcat(suffix, pop(optstack));
                                //PrintStack(optstack);
                            }
                            else
                            {
                                break;
                            }
                        }
                        push(optstack, tokens[i]);
                        //PrintStack(optstack);
                    }
                }
            }
        }
        else //是数字则直接加到suffix的后面
        {
            strcpy(ctokens[l], tokens[i]);
            l++;
            strcat(suffix, tokens[i]);
            strcat(suffix, " ");
        }
    }
    while (optstack->top < 100) //将剩余元素弹出
    {
        strcpy(ctokens[l], optstack->elem[optstack->top]);
        l++;
        strcat(suffix, pop(optstack));
    }
    PrintStack(optstack);
}

int PreParse(char tokens[][Max_n * 2], char *str)
{
    char p[Num];
    char temp[2];
    int i = 0, j = 0, l, k = 0;

    for (i = 0; i < strlen(str); i++) //删除表达式中的空格
    {
        if (str[i] == ' ')
            continue;
        else
        {
            p[j] = str[i];
            j++;
        }
    }
    p[j] = '\0';

    i = 0;
    for (j = 0; j < strlen(p); j++)
    {
        if (p[j] == '+' || p[j] == '-' || p[j] == '*' || p[j] == '/' || p[j] == '(' || p[j] == ')' || p[j] == '^' || p[j] == 's' || p[j] == 'c') //运算符转化为字符串
        {
            temp[0] = p[j];
            temp[1] = '\0';
            strcpy(tokens[k], temp);
        }
        else
        {
            for (l = 0; (isdigit(p[j])) || (p[j] == '.'); l++) //得到多位数字的字符串
            {
                tokens[k][l] = p[j];
                j++;
            }
            tokens[k][l] = '\0';
            j--;
        }
        k++;
    }
    return k;
}

int GetProsity(char *ch)
{
    int prosity;
    if (!(strcmp(ch, "(")))
    {
        prosity = 0;
    }
    if (!(strcmp(ch, "+") && strcmp(ch, "-")))
    {
        prosity = 1;
    }
    if (!(strcmp(ch, "*") && strcmp(ch, "/")))
    {
        prosity = 2;
    }
    if (!strcmp(ch, "^"))
    {
        prosity = 3;
    }
    if (!(strcmp(ch, "s") && strcmp(ch, "c")))
    {
        prosity = 4;
    }
    return prosity;
}

void push(STACK S, char *opt)
{
    if (S->top == 0)
    {
        return;
    }
    S->top--;
    strcpy(S->elem[S->top], opt);
    return;
}

char *pop(STACK S)
{
    strcpy(ret, S->elem[S->top]);
    S->top++;
    return ret;
}

int IsEmpty(STACK S)
{
    if (S->top > Num - 1)
        return 1;
    else
        return 0;
}

void MakeNull(STACK S)
{
    S->top = Num;
}

void MakeNull1(DSTACK S)
{
    S->top = Num;
}
void PrintStack(STACK S)
{
    int i;
    for (i = Num - 1; i >= S->top; i--)
    {
        printf("%s ", S->elem[i]);
    }
    printf("\n");
}

char *float_plus(char *a, char *b)
{
    int temp_i;
    memset(result, 0, sizeof(result));

    char sumint[Max_n] = {0}, sumdec[Max_n] = {0}; //每一位都是数值形式，不是字符。前者倒排
    int pa = -1, pb = -1, len_a, len_b;
    int i, td;

    len_a = strlen(a);
    len_b = strlen(b);
    for (i = 0; i < len_a; i++)
    { //找到小数点
        if (a[i] == '.' || !a[i] && pa == -1)
            pa = i; //万一输入没有点
        if (pa >= 0)
            break;
    }
    for (i = 0; i < len_b; i++)
    { //找到小数点
        if (b[i] == '.' || !b[i] && pb == -1)
            pb = i; //万一输入没有点
        if (pb >= 0)
            break;
    }
    for (td = i = (len_a - pa > len_b - pb) ? len_a - pa : len_b - pb; i > 0; i--)
    { //小数部分从低到高按位求和，前面多出一位保存给整数部分的进位，td保存了用得到的最大下标
        sumdec[i] += (a[pa + i] ? a[pa + i] - 48 : 0) + (b[pb + i] ? b[pb + i] - 48 : 0);
        sumdec[i - 1] = sumdec[i] / 10;
        sumdec[i] %= 10;
    }
    while (td > 0 && !sumdec[td])
        td--;              //抹掉后缀0
    sumint[0] = sumdec[0]; //进位
    for (i = 1; pa - i >= 0 || pb - i >= 0; i++)
    { //整数部分从低到高按位求和
        sumint[i - 1] += (pa - i >= 0 ? a[pa - i] - 48 : 0) + (pb - i >= 0 ? b[pb - i] - 48 : 0);
        sumint[i] = sumint[i - 1] / 10;
        sumint[i - 1] %= 10;
    }
    while (i > 0 && !sumint[i])
        i--; //抹掉前置0
    temp_i = i;
    for (; i >= 0; i--) //因为倒排，所以倒输出
        result[temp_i - i] = sumint[i] + '0';
    if (td > 0)
        result[temp_i + 1] = '.'; //小数部分为零就没有小数点了
    for (i = 1; i < td + 1; i++)  //输出小数部分,sumdec[0]是进位信息，所以从下标1开始
        result[temp_i + i + 1] = sumdec[i] + '0';

    return result;
}

char *float_minus(char *a, char *b)
{
    int temp_i;
    memset(result, 0, sizeof(result));

    char sumint[Max_n] = {0}, sumdec[Max_n] = {0}; //每一位都是数值形式，不是字符。前者倒排
    int pa = -1, pb = -1, len_a, len_b;
    int i, td;

    len_a = strlen(a);
    len_b = strlen(b);
    for (i = 0; i < len_a; i++)
    { //找到小数点
        if (a[i] == '.' || !a[i] && pa == -1)
            pa = i; //万一输入没有点
        if (pa >= 0)
            break;
    }
    for (i = 0; i < len_b; i++)
    { //找到小数点
        if (b[i] == '.' || !b[i] && pb == -1)
            pb = i; //万一输入没有点
        if (pb >= 0)
            break;
    }
    for (td = i = (len_a - pa > len_b - pb) ? len_a - pa : len_b - pb; i > 0; i--)
    { //小数部分从低到高按位求和，前面多出一位保存给整数部分的进位，td保存了用得到的最大下标
        sumdec[i] += (a[pa + i] ? a[pa + i] - 48 : 0) - (b[pb + i] ? b[pb + i] - 48 : 0);
        sumdec[i - 1] = (sumdec[i] < 0) ? (-1) : 0;
        sumdec[i] = (sumdec[i] < 0) ? (sumdec[i] + 10) : sumdec[i];
    }
    while (td > 0 && !sumdec[td])
        td--;              //抹掉后缀0
    sumint[0] = sumdec[0]; //进位
    for (i = 1; pa - i >= 0 || pb - i >= 0; i++)
    { //整数部分从低到高按位求和
        sumint[i - 1] += (pa - i >= 0 ? a[pa - i] - 48 : 0) - (pb - i >= 0 ? b[pb - i] - 48 : 0);
        sumint[i] = (sumint[i - 1] < 0) ? (-1) : 0;
        sumint[i - 1] = (sumint[i - 1] < 0) ? (sumint[i - 1] + 10) : sumint[i - 1];
    }
    while (i > 0 && !sumint[i])
        i--; //抹掉前置0
    temp_i = i;
    for (; i >= 0; i--) //因为倒排，所以倒输出
        result[temp_i - i] = sumint[i] + '0';
    if (td > 0)
        result[temp_i + 1] = '.'; //小数部分为零就没有小数点了
    for (i = 1; i < td + 1; i++)  //输出小数部分,sumdec[0]是进位信息，所以从下标1开始
        result[temp_i + i + 1] = sumdec[i] + '0';

    return result;
}

void dpush(DSTACK S, char *opd)
{
    if (S->top == 0)
    {
        return;
    }
    S->top--;
    strcpy(S->elem[S->top], opd);
}

void dpop(DSTACK S)
{
    S->top++;
}

void PrintStack1(DSTACK S)
{
    int i;
    for (i = Num - 1; i >= S->top; i--)
        puts(S->elem[i]);
    printf("\n");
}
char *Evalute(char ctokens[][Max_n * 2])
{
    int i;
    char ropd[Max_n * 2], lopd[Max_n * 2];
    char t[Max_n * 2];
    memset(t, 0, sizeof(t));
    memset(ropd, 0, sizeof(t));
    memset(lopd, 0, sizeof(t));
    stackj stack;             //定义一个操作数栈
    DSTACK opdstack = &stack; //初始化操作数栈指针
    MakeNull1(opdstack);
    for (i = 0; i < l; i++)
    {
        if (!strcmp(ctokens[i], "+")) //操作符则弹出两栈内元素进行相应运算
        {
            strcpy(ropd, opdstack->elem[opdstack->top]);
            dpop(opdstack);
            strcpy(lopd, opdstack->elem[opdstack->top]);
            dpop(opdstack);
            strcpy(t, float_plus(lopd, ropd));
            dpush(opdstack, t);
            printf("%s + %s = %s\n", lopd, ropd, t);
        }
        else if (!strcmp(ctokens[i], "-"))
        {
            strcpy(ropd, opdstack->elem[opdstack->top]);
            dpop(opdstack);
            strcpy(lopd, opdstack->elem[opdstack->top]);
            dpop(opdstack);
            strcpy(t, float_minus(lopd, ropd));
            dpush(opdstack, t);
            printf("%s - %s = %s\n", lopd, ropd, t);
        }
        /*else if (!strcmp(ctokens[i], "*"))
        {
            ropd = opdstack->elem[opdstack->top];
            dpop(opdstack);
            lopd = opdstack->elem[opdstack->top];
            dpop(opdstack);
            t = lopd * ropd;
            dpush(opdstack, t);
            PrintStack1(opdstack);
        }
        else if (!strcmp(ctokens[i], "/"))
        {
            ropd = opdstack->elem[opdstack->top];
            dpop(opdstack);
            lopd = opdstack->elem[opdstack->top];
            dpop(opdstack);
            t = lopd / ropd;
            dpush(opdstack, t);
            PrintStack1(opdstack);
        }*/
        else //操作数入栈
        {
            dpush(opdstack, ctokens[i]);
            //PrintStack1(opdstack);
        }
    }
    strcpy(value, opdstack->elem[opdstack->top]);
    return value;
}