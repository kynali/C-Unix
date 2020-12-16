#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define Num 100    //数字的个数
#define Max_n 1001 //支持的运算位数

char result[Max_n * 2];       //加减用
char mul_result[Max_n * 2];   //乘法用
char div_result[Max_n * 2];   //除法用
char pow_result[Max_n * 2];   //
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

char *float_plus(char *a, char *b);     //加法
int float_compare(char *a, char *b);    //比较绝对值大小
char *float_minus(char *a, char *b);    //减法，只能大减小
char *float_multiply(char *a, char *b); //乘法
char *cheng10k(char *temp_result, int k);
char *chu10k(char *temp_result, int k);
char *float_divide(char *a, char *b); //除法
char *float_power(char *a, char *b);  //除法

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
    int f_flag = 0;
    char temp[Max_n];
    stacki stacks;            //定义一个操作符栈stacks
    STACK optstack = &stacks; //定义操作符栈指针optstack
    MakeNull(optstack);
    count = PreParse(tokens, str);
    for (i = 0; i < count; i++)
    { //是运算符的话
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
                    if (!strcmp(tokens[i], "-"))
                    {
                        if (!strcmp(tokens[i - 1], "("))
                        {
                            f_flag = 1;
                            continue;
                        }
                    }
                    if (GetProsity(tokens[i]) > GetProsity(optstack->elem[optstack->top]))
                    {
                        push(optstack, tokens[i]);
                        //PrintStack(optstack);
                    }
                    else
                    {
                        while (optstack->top < Num)
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
        else //非负数则加上符号位，直接加到suffix的后面
        {
            if (f_flag)
            {
                strcpy(ctokens[l], "f");
                f_flag = 0;
            }
            else
                strcpy(ctokens[l], "z");
            strcat(ctokens[l], tokens[i]);
            strcat(suffix, ctokens[l]);
            strcat(suffix, " ");
            l++;
        }
    }
    while (optstack->top < Num) //将剩余元素弹出
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

    int pa = -1, pb = -1, len_a, len_b, len;
    int i, td;

    len_a = strlen(a);
    len_b = strlen(b);
    for (i = 0; i < len_a; i++)
    { //找到小数点
        if (a[i] == '.')
            pa = i;
        if (pa >= 0)
            break;
    }
    if (pa < 0)
        pa = len_a;
    for (i = 0; i < len_b; i++)
    { //找到小数点
        if (b[i] == '.')
            pb = i;
        if (pb >= 0)
            break;
    }
    if (pb < 0)
        pb = len_b;
    if (pa < pb) //小数点对齐，前面的0变成k
    {
        for (i = len_a - 1; i >= 0; i--)
            a[i + pb - pa] = a[i];
        for (i = 0; i < pb - pa; i++)
            a[i] = 'k';
        td = pb;
    }
    else
    {
        for (i = len_b - 1; i >= 0; i--)
            b[i + pa - pb] = b[i];
        for (i = 0; i < pa - pb; i++)
            b[i] = 'k';
        td = pa;
    }

    len_a = strlen(a);
    len_b = strlen(b);

    for (len = i = (len_a > len_b ? len_a - 1 : len_b - 1); i > 0; i--)
    { //从低到高按位求和 每一位都是数值形式，不是字符
        if (i == td)
        {
            result[i - 1] = result[i];
            result[i] = '.';
            continue;
        }
        if (a[i] == 'k')
        {
            result[i] += (b[i] == 0 ? 0 : b[i] - 48);
            result[i - 1] = result[i] / 10; //进位
            result[i] %= 10;
        }
        else if (b[i] == 'k')
        {
            result[i] += (a[i] == 0 ? 0 : a[i] - 48);
            result[i - 1] = result[i] / 10; //进位
            result[i] %= 10;
        }
        else
        {
            result[i] += (a[i] == 0 ? 0 : a[i] - 48) + (b[i] == 0 ? 0 : b[i] - 48);
            result[i - 1] = result[i] / 10; //进位
            result[i] %= 10;
        }
    }
    if (a[0] == 'k')
        result[0] += (b[0] == 0 ? 0 : b[0] - 48);
    else if (b[0] == 'k')
        result[0] += (a[0] == 0 ? 0 : a[0] - 48);
    else
        result[0] += (a[0] == 0 ? 0 : a[0] - 48) + (b[0] == 0 ? 0 : b[0] - 48);
    for (i = len; i > 0; i--)
        if (i != td)
            result[i] += '0';
    if (result[0] > 9) //若整体进了一位，则数组整体后移，前面加一位
    {
        strcpy(result + 1, result);
        result[0] = result[0] / 10;
        result[1] = result[0] % 10 + '0';
    }
    result[0] = result[0] + '0';

    while (td != 1 && result[0] == '0') //去前置0
    {
        strcpy(result, result + 1);
        result[len] = 0;
        len--;
        td--;
    }

    for (i = Max_n; i < Max_n * 2; i++)
        result[i] = 0;

    return result;
}

char *float_minus(char *a, char *b)
{
    int temp_i;
    memset(result, 0, sizeof(result));

    int pa = -1, pb = -1, len_a, len_b, len;
    int i, j, td;

    len_a = strlen(a);
    len_b = strlen(b);
    for (i = 0; i < len_a; i++)
    { //找到小数点
        if (a[i] == '.')
            pa = i;
        if (pa >= 0)
            break;
    }
    if (pa < 0)
        pa = len_a;
    for (i = 0; i < len_b; i++)
    { //找到小数点
        if (b[i] == '.')
            pb = i;
        if (pb >= 0)
            break;
    }
    if (pb < 0)
        pb = len_b;
    for (i = len_b - 1; i >= 0; i--)
    {
        j = i + pa - pb;
        b[j] = b[i];
    }
    for (i = 0; i < pa - pb; i++)
        b[i] = 'k';
    td = pa;

    len_a = strlen(a);
    len_b = strlen(b);

    for (len = i = (len_a > len_b ? len_a - 1 : len_b - 1); i > 0; i--)
    { //从低到高按位求差 每一位都是数值形式，不是字符
        if (i == td)
        {
            result[i - 1] = result[i];
            result[i] = '.';
            continue;
        }
        if (b[i] == 'k')
        {
            result[i] += (a[i] == 0 ? 0 : a[i] - 48);
            if (result[i] < 0) //退位
            {
                result[i] += 10;
                result[i - 1] = -1;
            }
        }
        else
        {
            result[i] += (a[i] == 0 ? 0 : a[i] - 48) - (b[i] == 0 ? 0 : b[i] - 48);
            if (result[i] < 0) //退位
            {
                result[i] += 10;
                result[i - 1] = -1;
            }
        }
    }
    if (b[0] == 'k')
        result[0] += (a[0] == 0 ? 0 : a[0] - 48);
    else
        result[0] += (a[0] == 0 ? 0 : a[0] - 48) - (b[0] == 0 ? 0 : b[0] - 48);

    while (td != 1 && result[0] == 0) //去掉前置0
    {
        for (i = 0; i < len; i++)
            result[i] = result[i + 1];
        result[len] = 0;
        len--;
        td--;
    }
    for (i = len; i > 0; i--)
        if (i != td)
            result[i] += '0';
    if (result[0] == 0 && td != 1) //若整体退了一位，则数组整体前移
    {
        strcpy(result, result + 1);
        result[len] = '\0';
    }
    result[0] = result[0] + '0';

    for (i = Max_n; i < Max_n * 2; i++)
        result[i] = 0;

    return result;
}

int float_compare(char *a, char *b)
{
    int temp_i;
    char result[Max_n * 2];
    memset(result, 0, sizeof(result));

    int pa = -1, pb = -1, len_a, len_b, len;
    int i, td;

    len_a = strlen(a);
    len_b = strlen(b);
    for (i = 0; i < len_a; i++)
    { //找到小数点
        if (a[i] == '.')
            pa = i;
        if (pa >= 0)
            break;
    }
    if (pa < 0)
        pa = len_a;
    for (i = 0; i < len_b; i++)
    { //找到小数点
        if (b[i] == '.')
            pb = i;
        if (pb >= 0)
            break;
    }
    if (pb < 0)
        pb = len_b;
    if (pa < pb) //b比a大
        return 0;
    else if (pa > pb)
        return 1;
    else
    {
        for (len = (len_a > len_b ? len_a : len_b), i = 0; i < len; i++)
        { //从高到低位按位比较
            if (a[i] > b[i])
                return 1;
            else if (a[i] < b[i])
                return 0;
        }
        return 1;
    }
}

char *float_multiply(char *a, char *b)
{
    int temp_i;
    char temp;
    memset(mul_result, 0, sizeof(mul_result));
    mul_result[0] = '0';
    char temp_result[Max_n * 2];
    memset(temp_result, 0, sizeof(temp_result));
    char temp2_result[Max_n * 2];
    memset(temp2_result, 0, sizeof(temp2_result));

    int pa = -1, pb = -1, pt = -1, len_a, len_b, len;
    int i, j, m;

    len_a = strlen(a);
    len_b = strlen(b);
    for (i = 0; i < len_a; i++)
    { //找到小数点
        if (a[i] == '.')
            pa = i;
        if (pa >= 0)
            break;
    }
    if (pa < 0)
        pa = len_a;
    for (i = 0; i < len_b; i++)
    { //找到小数点
        if (b[i] == '.')
            pb = i;
        if (pb >= 0)
            break;
    }
    if (pb < 0)
        pb = len_b;

    for (i = 0; i < len_b; i++)
    { //b从高到低按位求积 每一位都是数值形式，不是字符
        memset(temp_result, 0, sizeof(temp_result));
        if (b[i] == '.')
            continue;
        for (j = len_a - 1; j > 0; j--)
        { //a从低到高按位求积
            if (a[j] == '.')
            {
                temp_result[j - 1] = temp_result[j];
                temp_result[j] = '.';
                continue;
            }
            temp_result[j] += (a[j] == 0 ? 0 : a[j] - 48) * (b[i] == 0 ? 0 : b[i] - 48);
            temp_result[j - 1] = temp_result[j] / 10; //进位
            temp_result[j] %= 10;
        }
        temp_result[0] += (a[0] == 0 ? 0 : a[0] - 48) * (b[i] == 0 ? 0 : b[i] - 48);
        for (j = len_a - 1; j > 0; j--)
            if (temp_result[j] != '.')
                temp_result[j] += '0';
        if (temp_result[0] > 9) //若整体进了一位，则数组整体后移，前面加一位
        {
            for (j = strlen(temp_result); j > 0; j--)
                temp_result[j] = temp_result[j - 1];
            temp_result[0] /= 10;
            temp_result[1] %= 10;
            temp_result[1] += '0';
        }
        temp_result[0] += '0';
        len = strlen(temp_result);
        pt = -1;
        for (j = 0; j < len; j++)
        { //找到temp_result的小数点
            if (temp_result[j] == '.')
                pt = j;
            if (pt >= 0)
                break;
        }
        if (pt < 0)
            pt = len;

        if (i < pb)
        {
            for (j = pb - i; j > 1; j--)
            {
                if ((temp_result[pt] == '.') && (temp_result[pt + 1] != 0))
                {
                    if (pt == 1 && temp_result[0] == '0')
                    {
                        strcpy(temp_result, temp_result + 2);
                        temp_result[len - 1] = 0;
                        temp_result[len] = 0;
                        len -= 2;
                        continue;
                    }
                    temp = temp_result[pt + 1];
                    temp_result[pt + 1] = '.';
                    temp_result[pt] = temp;
                    pt++;
                    if (temp_result[pt + 1] == 0)
                        temp_result[pt] = 0;
                }
                else if ((temp_result[pt] == '.') && (temp_result[pt + 1] == 0))
                {
                    temp_result[pt] = '0';
                }
                else
                {
                    temp_result[pt] = '0';
                    pt++;
                }
            }
        }
        else
        {
            for (j = i - pb; j > 0; j--)
            {
                len = strlen(temp_result);
                if (pt != 1)
                {
                    if (temp_result[pt] != '.' && temp_result[pt - 1] == '0')
                        temp_result[pt - 1] = 0;
                    else
                    {
                        temp = temp_result[pt - 1];
                        temp_result[pt - 1] = '.';
                        temp_result[pt] = temp;
                    }
                    pt--;
                }
                else
                {
                    if (temp_result[0] != '0' && temp_result[1] != '.')
                    {
                        temp_result[2] = temp_result[0];
                        temp_result[0] = '0';
                        temp_result[1] = '.';
                        pt = 1;
                    }
                    else if (temp_result[1] == '.')
                    {
                        for (m = len - 1; m > 1; m--)
                            temp_result[m + 1] = temp_result[m];
                        temp_result[2] = temp_result[0];
                        temp_result[0] = '0';
                        temp_result[1] = '.';
                    }
                }
            }
        }
        strcpy(temp2_result, float_plus(mul_result, temp_result));
        memset(mul_result, 0, sizeof(mul_result));
        strcpy(mul_result, temp2_result);
        memset(temp2_result, 0, sizeof(temp2_result));
    }

    for (i = Max_n; i < Max_n * 2; i++)
        mul_result[i] = 0;

    return mul_result;
}

char *cheng10k(char *temp_result, int k)
{
    char temp;
    int len = strlen(temp_result);
    int pt = -1;
    int i, j;

    for (j = 0; j < len; j++)
    { //找到temp_result的小数点
        if (temp_result[j] == '.')
            pt = j;
        if (pt >= 0)
            break;
    }
    if (pt < 0)
        pt = len;

    while (k > 0)
    {
        if ((temp_result[pt] == '.') && (temp_result[pt + 1] != 0))
        {
            if (pt == 1 && temp_result[0] == '0')
            {
                strcpy(temp_result, temp_result + 2);
                temp_result[len - 1] = 0;
                temp_result[len] = 0;
                len -= 2;
                k--;
                continue;
            }
            temp = temp_result[pt + 1];
            temp_result[pt + 1] = '.';
            temp_result[pt] = temp;
            pt++;
            if (temp_result[pt + 1] == 0)
                temp_result[pt] = 0;
        }
        else if ((temp_result[pt] == '.') && (temp_result[pt + 1] == 0))
        {
            temp_result[pt] = '0';
        }
        else
        {
            temp_result[pt] = '0';
            pt++;
        }
        k--;
    }

    for (i = Max_n; i < Max_n * 2; i++)
        temp_result[i] = 0;
    return temp_result;
}

char *chu10k(char *temp_result, int k)
{
    char temp;
    int len = strlen(temp_result);
    int pt = -1;
    int i, j;

    for (j = 0; j < len; j++)
    { //找到temp_result的小数点
        if (temp_result[j] == '.')
            pt = j;
        if (pt >= 0)
            break;
    }
    if (pt < 0)
        pt = len;

    while (k > 0)
    {
        len = strlen(temp_result);
        if (pt != 1)
        {
            if (temp_result[pt] != '.' && temp_result[pt - 1] == '0')
                temp_result[pt - 1] = 0;
            else
            {
                temp = temp_result[pt - 1];
                temp_result[pt - 1] = '.';
                temp_result[pt] = temp;
            }
            pt--;
        }
        else
        {
            if (temp_result[0] != '0' && temp_result[1] != '.')
            {
                temp_result[2] = temp_result[0];
                temp_result[0] = '0';
                temp_result[1] = '.';
                pt = 1;
            }
            else if (temp_result[1] == '.')
            {
                for (j = len - 1; j > 1; j--)
                    temp_result[j + 1] = temp_result[j];
                temp_result[2] = temp_result[0];
                temp_result[0] = '0';
                temp_result[1] = '.';
            }
        }
        k--;
    }

    for (i = Max_n; i < Max_n * 2; i++)
        temp_result[i] = 0;
    return temp_result;
}

char *float_divide(char *a, char *b)
{
    int i, j, k, n;
    int yushu = 0;
    int yushu_flag = 0;
    char s[Max_n] = "1";
    int st = 1;
    char temp_result[Max_n * 2];
    memset(div_result, 0, sizeof(div_result));
    div_result[0] = '0';
    memset(temp_result, 0, sizeof(temp_result));
    char temp2_result[Max_n * 2];
    memset(temp2_result, 0, sizeof(temp2_result));

    n = 0;
    while (!float_compare(a, b)) //若b比a大，a*10
    {
        cheng10k(a, 1);
        n++;
    }
    strcpy(temp_result, b);
    for (k = 1; float_compare(a, cheng10k(temp_result, 1)); k++)
        ;
    k--;

    cheng10k(b, k);
    memset(temp_result, 0, sizeof(temp_result));
    strcpy(temp_result, b);
    memset(temp2_result, 0, sizeof(temp2_result));
    strcpy(temp2_result, a);
    if (float_compare(temp2_result, temp_result) && float_compare(temp_result, temp2_result))
    {
        memset(temp_result, 0, sizeof(temp_result));
        strcpy(temp_result, "10");
        strcpy(div_result, cheng10k(temp_result, k));
        for (i = Max_n; i < Max_n * 2; i++)
            div_result[i] = 0;
        cheng10k(div_result, k);
        chu10k(div_result, n + 1);
        for (i = Max_n; i < Max_n * 2; i++)
            div_result[i] = 0;
        return div_result;
    }

    for (i = 0; i < Max_n - 10; i++)
    {
        if (i != 1)
        {
            if (float_compare(a, b))
            {
                memset(temp_result, 0, sizeof(temp_result));
                strcpy(temp_result, b);
                s[0] = st + '0';
                while (float_compare(a, float_multiply(temp_result, s)))
                {
                    memset(temp_result, 0, sizeof(temp_result));
                    strcpy(temp_result, b);
                    st++;
                    s[0] = st + '0';
                }
                memset(temp_result, 0, sizeof(temp_result));
                strcpy(temp_result, b);
                s[0] = st + '0';
                st--;
                s[0] = st + '0';
                div_result[i - yushu] = st + '0';
                memset(temp_result, 0, sizeof(temp_result));
                strcpy(temp_result, b);
                memset(temp2_result, 0, sizeof(temp2_result));
                strcpy(temp2_result, float_multiply(temp_result, s));
                memset(temp_result, 0, sizeof(temp_result));
                strcpy(temp_result, float_minus(a, temp2_result));
                memset(a, 0, sizeof(a));
                strcpy(a, temp_result);
                if (a[0] == '0' && a[1] != '.')
                    break;
                st = 1;
                yushu++;
                yushu_flag = 1;
            }
            else
            {
                if (!yushu_flag)
                    div_result[i - yushu] = '0';
                yushu_flag = 0;
                cheng10k(a, 1);
            }
        }
    }
    if (div_result[2] != 0)
        div_result[1] = '.';
    cheng10k(div_result, k);
    chu10k(div_result, n);
    for (i = Max_n; i < Max_n * 2; i++)
        div_result[i] = 0;
    return div_result;
}

char *float_power(char *a, char *b)
{
    memset(pow_result, 0, sizeof(pow_result));
    pow_result[0] = '0';
    char temp_result[Max_n * 2];
    memset(temp_result, 0, sizeof(temp_result));
    char temp2_result[Max_n * 2];
    memset(temp2_result, 0, sizeof(temp2_result));
    char temp3_result[Max_n * 2];
    memset(temp3_result, 0, sizeof(temp3_result));
    char tempb_result[Max_n * 2];
    memset(tempb_result, 0, sizeof(tempb_result));
    char temp4_result[Max_n * 2];
    char temp5_result[Max_n * 2];
    char temp6_result[Max_n * 2];
    char temp7_result[Max_n * 2];
    char temp8_result[Max_n * 2];
    char temp9_result[Max_n * 2];

    int pa = -1, pb = -1, pt = -1, len_a, len_b, len;
    int i, j, k;

    len_a = strlen(a);
    len_b = strlen(b);
    for (i = 0; i < len_b; i++)
    { //找到小数点
        if (b[i] == '.')
            pb = i;
        if (pb >= 0)
            break;
    }
    if (pb < 0)
        pb = len_b;

    if (b[pb] != '.')
    {
        strcpy(temp3_result, a);
        strcpy(tempb_result, "1");
        while (float_compare(b, "2"))
        {
            memset(temp_result, 0, sizeof(temp_result));
            strcpy(temp_result, a);
            memset(temp2_result, 0, sizeof(temp2_result));
            strcpy(temp2_result, temp3_result);
            memset(temp3_result, 0, sizeof(temp3_result));
            strcpy(temp3_result, float_multiply(temp_result, temp2_result));
            memset(temp_result, 0, sizeof(temp_result));
            strcpy(temp_result, b);
            strcpy(b, float_minus(temp_result, tempb_result));
        }
        strcpy(pow_result, temp3_result);
        for (i = Max_n; i < Max_n * 2; i++)
            pow_result[i] = 0;
        return pow_result;
    }
    else
    {
        strcpy(tempb_result, "1");
        memset(temp_result, 0, sizeof(temp_result));
        strcpy(temp_result, a);
        memset(temp2_result, 0, sizeof(temp2_result));
        if (float_compare(temp_result, tempb_result))
            strcpy(temp2_result, float_minus(temp_result, tempb_result));
        else
            strcpy(temp2_result, float_minus(tempb_result, temp_result));
        memset(temp3_result, 0, sizeof(temp3_result));
        memset(temp_result, 0, sizeof(temp_result));
        strcpy(temp_result, a);
        memset(tempb_result, 0, sizeof(tempb_result));
        strcpy(tempb_result, "1");
        strcpy(temp3_result, float_plus(temp_result, tempb_result));
        memset(temp_result, 0, sizeof(temp_result));
        strcpy(temp_result, float_divide(temp2_result, temp3_result));
        strcpy(temp3_result, temp_result);
        memset(temp2_result, 0, sizeof(temp2_result));
        strcpy(temp2_result, "1");
        memset(temp4_result, 0, sizeof(temp4_result));
        memset(temp5_result, 0, sizeof(temp5_result));
        memset(temp6_result, 0, sizeof(temp6_result));
        strcpy(temp6_result, "1");
        memset(temp7_result, 0, sizeof(temp7_result));
        memset(temp8_result, 0, sizeof(temp8_result));
        memset(temp9_result, 0, sizeof(temp9_result));
        for (i = 2; i < 50; i += 2)
        {
            memset(tempb_result, 0, sizeof(tempb_result));
            strcpy(tempb_result, "1");
            memset(temp7_result, 0, sizeof(temp7_result));
            strcpy(temp7_result, "2");
            strcpy(temp5_result, temp3_result);
            for (j = i; j > 1; j--)
            {
                memset(temp3_result, 0, sizeof(temp3_result));
                strcpy(temp3_result, temp_result);
                memset(temp4_result, 0, sizeof(temp4_result));
                strcpy(temp4_result, temp5_result);
                memset(temp5_result, 0, sizeof(temp5_result));
                strcpy(temp5_result, float_multiply(temp3_result, temp4_result));

                memset(tempb_result, 0, sizeof(tempb_result));
                strcpy(tempb_result, "1");
                memset(temp8_result, 0, sizeof(temp8_result));
                strcpy(temp8_result, float_plus(temp7_result, tempb_result));
                memset(temp7_result, 0, sizeof(temp7_result));
                strcpy(temp7_result, temp8_result);
            }
            memset(temp3_result, 0, sizeof(temp3_result));
            strcpy(temp3_result, temp_result);
            memset(temp4_result, 0, sizeof(temp4_result));
            strcpy(temp4_result, temp6_result);
            memset(temp6_result, 0, sizeof(temp6_result));
            strcpy(temp6_result, float_divide(temp5_result, temp7_result));
            memset(temp9_result, 0, sizeof(temp9_result));
            strcpy(temp9_result, float_plus(temp6_result, temp4_result));
            memset(temp6_result, 0, sizeof(temp6_result));
            strcpy(temp6_result, temp9_result);
        }
        memset(temp9_result, 0, sizeof(temp9_result));
        strcpy(temp9_result, float_multiply(temp_result, temp6_result));
        memset(temp7_result, 0, sizeof(temp7_result));
        strcpy(temp7_result, "2");
        memset(temp6_result, 0, sizeof(temp6_result));
        strcpy(temp6_result, float_multiply(temp9_result, temp7_result));
        memset(temp7_result, 0, sizeof(temp7_result));
        strcpy(temp7_result, b);
        memset(temp_result, 0, sizeof(temp_result));
        strcpy(temp_result, float_multiply(temp6_result, temp7_result));
    }
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
    char tropd[Max_n * 2], tlopd[Max_n * 2];
    char t[Max_n * 2];
    char t2[Max_n * 2];
    memset(t, 0, sizeof(t));
    memset(t2, 0, sizeof(t2));
    memset(ropd, 0, sizeof(ropd));
    memset(lopd, 0, sizeof(lopd));
    memset(tropd, 0, sizeof(tropd));
    memset(tlopd, 0, sizeof(tlopd));
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
            if ((lopd[0] == 'z') && (ropd[0] == 'z'))
            {
                strcpy(tlopd, lopd + 1);
                strcpy(tropd, ropd + 1);
                strcpy(t, "z");
                strcat(t, float_plus(tlopd, tropd));
            }
            else if ((lopd[0] == 'f') && (ropd[0] == 'f'))
            {
                strcpy(tlopd, lopd + 1);
                strcpy(tropd, ropd + 1);
                strcpy(t, "f");
                strcat(t, float_plus(tlopd, tropd));
            }
            else if ((lopd[0] == 'z') && (ropd[0] == 'f'))
            {
                strcpy(tlopd, lopd + 1);
                strcpy(tropd, ropd + 1);
                if (float_compare(tlopd, tropd))
                {
                    strcpy(t, "z");
                    strcat(t, float_minus(tlopd, tropd));
                }
                else
                {
                    strcpy(t, "f");
                    strcat(t, float_minus(tropd, tlopd));
                }
            }
            else if ((lopd[0] == 'f') && (ropd[0] == 'z'))
            {
                strcpy(tlopd, lopd + 1);
                strcpy(tropd, ropd + 1);
                if (float_compare(tlopd, tropd))
                {
                    strcpy(t, "f");
                    strcat(t, float_minus(tlopd, tropd));
                }
                else
                {
                    strcpy(t, "z");
                    strcat(t, float_minus(tropd, tlopd));
                }
            }
            dpush(opdstack, t);
            printf("%s + %s = %s\n", lopd, ropd, t);
        }
        else if (!strcmp(ctokens[i], "-"))
        {
            strcpy(ropd, opdstack->elem[opdstack->top]);
            dpop(opdstack);
            strcpy(lopd, opdstack->elem[opdstack->top]);
            dpop(opdstack);
            if ((lopd[0] == 'z') && (ropd[0] == 'f'))
            {
                strcpy(tlopd, lopd + 1);
                strcpy(tropd, ropd + 1);
                strcpy(t, "z");
                strcat(t, float_plus(tlopd, tropd));
            }
            else if ((lopd[0] == 'f') && (ropd[0] == 'z'))
            {
                strcpy(tlopd, lopd + 1);
                strcpy(tropd, ropd + 1);
                strcpy(t, "f");
                strcat(t, float_plus(tlopd, tropd));
            }
            else if ((lopd[0] == 'z') && (ropd[0] == 'z'))
            {
                strcpy(tlopd, lopd + 1);
                strcpy(tropd, ropd + 1);
                if (float_compare(tlopd, tropd))
                {
                    strcpy(t, "z");
                    strcat(t, float_minus(tlopd, tropd));
                }
                else
                {
                    strcpy(t, "f");
                    strcat(t, float_minus(tropd, tlopd));
                }
            }
            else
            {
                strcpy(tlopd, lopd + 1);
                strcpy(tropd, ropd + 1);
                if (float_compare(tlopd, tropd))
                {
                    strcpy(t, "f");
                    strcat(t, float_minus(tlopd, tropd));
                }
                else
                {
                    strcpy(t, "z");
                    strcat(t, float_minus(tropd, tlopd));
                }
            }
            dpush(opdstack, t);
            printf("%s - %s = %s\n", lopd, ropd, t);
        }
        else if (!strcmp(ctokens[i], "*"))
        {
            strcpy(ropd, opdstack->elem[opdstack->top]);
            dpop(opdstack);
            strcpy(lopd, opdstack->elem[opdstack->top]);
            dpop(opdstack);
            if ((lopd[0] == 'z') && (ropd[0] == 'z'))
            {
                strcpy(tlopd, lopd + 1);
                strcpy(tropd, ropd + 1);
                strcpy(t, "z");
                strcat(t, float_multiply(tlopd, tropd));
            }
            else if ((lopd[0] == 'f') && (ropd[0] == 'f'))
            {
                strcpy(tlopd, lopd + 1);
                strcpy(tropd, ropd + 1);
                strcpy(t, "z");
                strcat(t, float_multiply(tlopd, tropd));
            }
            else
            {
                strcpy(tlopd, lopd + 1);
                strcpy(tropd, ropd + 1);
                strcpy(t, "f");
                strcat(t, float_multiply(tlopd, tropd));
            }
            dpush(opdstack, t);
            printf("%s * %s = %s\n", lopd, ropd, t);
        }
        else if (!strcmp(ctokens[i], "/"))
        {
            strcpy(ropd, opdstack->elem[opdstack->top]);
            dpop(opdstack);
            strcpy(lopd, opdstack->elem[opdstack->top]);
            dpop(opdstack);
            if ((lopd[0] == 'z') && (ropd[0] == 'z'))
            {
                strcpy(tlopd, lopd + 1);
                strcpy(tropd, ropd + 1);
                strcpy(t, "z");
                strcat(t, float_divide(tlopd, tropd));
            }
            else if ((lopd[0] == 'f') && (ropd[0] == 'f'))
            {
                strcpy(tlopd, lopd + 1);
                strcpy(tropd, ropd + 1);
                strcpy(t, "z");
                strcat(t, float_divide(tlopd, tropd));
            }
            else
            {
                strcpy(tlopd, lopd + 1);
                strcpy(tropd, ropd + 1);
                strcpy(t, "f");
                strcat(t, float_divide(tlopd, tropd));
            }
            dpush(opdstack, t);
            printf("%s / %s = %s\n", lopd, ropd, t);
        }
        else if (!strcmp(ctokens[i], "^"))
        {
            strcpy(ropd, opdstack->elem[opdstack->top]);
            dpop(opdstack);
            strcpy(lopd, opdstack->elem[opdstack->top]);
            dpop(opdstack);
            if ((lopd[0] == 'z') && (ropd[0] == 'z'))
            {
                strcpy(tlopd, lopd + 1);
                strcpy(tropd, ropd + 1);
                strcpy(t, "z");
                strcat(t, float_power(tlopd, tropd));
            }
            else if ((lopd[0] == 'z') && (ropd[0] == 'f'))
            {
                strcpy(tlopd, lopd + 1);
                strcpy(tropd, ropd + 1);
                strcpy(t, float_power(tlopd, tropd));
                memset(t2, 0, sizeof(t2));
                strcpy(t2, float_divide("1", t));
                memset(t, 0, sizeof(t));
                strcpy(t, "z");
                strcat(t, t2);
            }
            dpush(opdstack, t);
            printf("%s ^ %s = %s\n", lopd, ropd, t);
        }
        else if (!strcmp(ctokens[i], "s"))
        {
        }
        else if (!strcmp(ctokens[i], "c"))
        {
        }
        else //操作数入栈
        {
            dpush(opdstack, ctokens[i]);
            //PrintStack1(opdstack);
        }
    }
    strcpy(value, opdstack->elem[opdstack->top]);
    return value;
}