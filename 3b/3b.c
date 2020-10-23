#include <stdio.h>

typedef struct fraction //分数类
{
    int numerator;
    int denominator;
} fraction;

int gcd(int m, int n)   //最大公约数
{
    if (n == 0)
        return m;
    return gcd(n, m % n);
}

fraction yuefen(fraction a) //约分
{
    int tp;
    tp = gcd(a.numerator, a.denominator);
    a.numerator /= tp;
    a.denominator /= tp;
    if (a.denominator < 0)
    {
        a.numerator = -a.numerator;
        a.denominator = -a.denominator;
    }
    return a;
}

fraction plus(fraction the_fraction_a, fraction the_fraction_b) //加法
{
    fraction temp;
    temp.numerator = the_fraction_a.numerator * the_fraction_b.denominator + the_fraction_b.numerator * the_fraction_a.denominator;
    temp.denominator = the_fraction_a.denominator * the_fraction_b.denominator;
    temp = yuefen(temp);
    return temp;
}

fraction minus(fraction the_fraction_a, fraction the_fraction_b)    //减法
{
    fraction temp;
    temp.numerator = the_fraction_a.numerator * the_fraction_b.denominator - the_fraction_b.numerator * the_fraction_a.denominator;
    temp.denominator = the_fraction_a.denominator * the_fraction_b.denominator;
    temp = yuefen(temp);
    return temp;
}

fraction multi(fraction the_fraction_a, fraction the_fraction_b)    //乘法
{
    fraction temp;
    temp.numerator = the_fraction_a.numerator * the_fraction_b.numerator;
    temp.denominator = the_fraction_a.denominator * the_fraction_b.denominator;
    temp = yuefen(temp);
    return temp;
}

fraction divide(fraction the_fraction_a, fraction the_fraction_b)   //除法
{
    fraction temp;
    temp.numerator = the_fraction_a.numerator * the_fraction_b.denominator;
    temp.denominator = the_fraction_a.denominator * the_fraction_b.numerator;
    temp = yuefen(temp);
    return temp;
}

double xiaoshu(fraction a)  //转化成小数
{
    double c=a.numerator;
    double d=a.denominator;
    return c/d;
}

int main()
{
    fraction a = {-135, 298};   //第一个分数
    fraction b = {334, 75};     //第二个分数
    fraction c;
    printf("分数 a 是\t\t%d / %d\n", a.numerator, a.denominator);
    printf("分数 b 是\t\t%d / %d\n", b.numerator, b.denominator);
    c = yuefen(a);
    printf("a 约分的结果：\t\t%d / %d  =  %f\n", c.numerator, c.denominator,xiaoshu(c));
    c = yuefen(b);
    printf("b 约分的结果：\t\t%d / %d  =  %f\n", c.numerator, c.denominator,xiaoshu(c));
    c = plus(a, b);
    printf("a+b：\t\t\t%d / %d  =  %f\n", c.numerator, c.denominator,xiaoshu(c));
    c = minus(a, b);
    printf("a-b：\t\t\t%d / %d  =  %f\n", c.numerator, c.denominator,xiaoshu(c));
    c = multi(a, b);
    printf("a*b：\t\t\t%d / %d  =  %f\n", c.numerator, c.denominator,xiaoshu(c));
    c = divide(a, b);
    printf("a/b：\t\t\t%d / %d  =  %f\n", c.numerator, c.denominator,xiaoshu(c));
    return 0;
}