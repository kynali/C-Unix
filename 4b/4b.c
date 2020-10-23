#include <stdio.h>
#include <time.h>
#include <stdlib.h>

int a = 8;          //a,b,c,表示此时8ml，5ml，3ml杯子里对应的水量
int b = 0;
int c = 0;
int state = 0;      //state，每次倒水之后，abc的状态
int allstate[300];      //allstate，记录每一种新的abc状态
int state_trans[300];   //state_trans，记录新的状态是如何转化而来的
int allstate_l = 1;     //allstate_l，不同状态的个数
int state_flag = 0;     //state_flag，是否是新的状态
int flag = 0;       //1-6取值，表示倒水方式ab、ba、ac、ca、bc、cb中的一种
int i = 0;          //计数用

//以下为倒水方式ab、ba、ac、ca、bc、cb的实现函数

void ab()
{
    if(a >= (5 - b)){       //5ml的杯子会被倒满
        a = a - (5 - b);
        b = 5;
    }
    else{                   //否则8ml杯子被倒空
        b = b + a;
        a = 0;
    }
    flag = 1;               //记录使用了ab倒水函数
}
void ba()
{
    if(b >= (8 - a)){    
        b = b - (8 - a);
        a = 8;
    }
    else{
        a = a + b;
        b = 0;
    }
    flag = 2;
}
void ac()
{
    if(a >= (3 - c)){    
        a = a - (3 - c);
        c = 3;
    }
    else{
        c = c + a;
        a = 0;
    }
    flag = 3;
}
void ca()
{
    if(c >= (8 - a)){    
        c = c - (8 - a);
        a = 8;
    }
    else{
        a = a + c;
        c = 0;
    }
    flag = 4;
}
void bc()
{
    if(b >= (3 - c)){    
        b = b - (3 - c);
        c = 3;
    }
    else{
        c = c + b;
        b = 0;
    }
    flag = 5;
}
void cb()
{
    if(c >= (5 - b)){    
        c = c - (5 - b);
        b = 5;
    }
    else{
        b = b + c;
        c = 0;
    }
    flag = 6;
}

int main()
{
    time_t t;           //随机数生成
    int temp = 0;       //暂存随机数
    srand((unsigned) time(&t));

    for(i = 0; i < 300; i++)    //数组初始化
        allstate[i] = 0;
    allstate[0] = 800;          //初始状态 8 0 0

    for(i = 0; i < 300; i++)
        state_trans[i] = 0;

    while(1)        //主循环
    {
        temp = rand() % 6;
        switch(temp)        //随机倒水
        {
            case 0:{        //在a有水可倒，b没有满，且上一次没有从b向a倒水的情况下，从a向b倒水
                if((a!=0) && (b!=5) && (flag != 2))
                    ab();
                }break;
            case 1:{
                if((b!=0) && (a!=8) && (flag != 1))
                    ba();
                }break;
            case 2:{
                if((a!=0) && (c!=3) && (flag != 4))
                    ac();
                }break;
            case 3:{
                if((c!=0) && (a!=8) && (flag != 3))
                    ca();
                }break;
            case 4:{
                if((b!=0) && (c!=3) && (flag != 6))
                    bc();
                }break;
            default:{
                if((c!=0) && (b!=5) && (flag != 5))
                    cb();
                }
        }

        state = a*100 + b*10 +c;    //计算abc目前状态

        state_flag = 0;
        for(i = 0; i < allstate_l; i++) //遍历，看state是不是新的状态
            if(state == allstate[i])    //若绕了一圈，回到了某个之前的状态
            {
                allstate_l = i + 1;     //则回到那个之前的状态，中间经历过的状态不再记录
                state_flag = 1;
                break;
            }
        if(state_flag == 0)             //若是新的状态，记录下来
        {
            allstate[allstate_l] = state;
            state_trans[allstate_l] = flag;
            allstate_l++;
        }

        if((a == 4) && (b == 4)) break; //若 4 4 0，则实现了题目要求，跳出
    }

    for(i = 0; i < allstate_l; i++) //将记录的 状态 和 状态转移情况 打印出来
    {
        switch(state_trans[i])
        {
            case 1:printf("a->b\n");break;
            case 2:printf("b->a\n");break;
            case 3:printf("a->c\n");break;
            case 4:printf("c->a\n");break;
            case 5:printf("b->c\n");break;
            case 6:printf("c->b\n");break;
            default:break;
        }
        printf("%d ",allstate[i]/100);
        printf("%d ",allstate[i]%100/10);
        printf("%d   ",allstate[i]%10);
    }
    printf("\n");
    
    return 0;
}