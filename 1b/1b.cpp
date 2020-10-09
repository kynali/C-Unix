#include <stdio.h>
#include <stdlib.h>
#include <time.h> 
#include <math.h>

#define MAX_REN 800     //人数
#define MAX_DAY 100     //最长观测天数
#define PERCENT 80      //初始感染百分比

//以下为变长数组 table 的定义和操作函数的定义
typedef struct Table {
        int * head;
        int length;
        int size;
    }table;
table initTable0() {
        table t;
        t.head = NULL;
        t.length = 0;
        t.size = 0;
        return t;
    }
table initTable1() {
    table t;
    t.head = (int*)malloc(MAX_REN * sizeof(int));
    if (!t.head)
    {
        printf("初始化失败");
        exit(0);
    }
    t.length = 0;
    t.size = MAX_REN;
    return t;
}
table addTable(table t, int elem, int add)
    {
        int i;
        if (add > t.length + 1 || add < 1) {
            printf("插入位置有问题");
            return t;
        }
        if (t.length >= t.size) {
            t.head = (int *)realloc(t.head, (t.size + 1) * sizeof(int));
            if (!t.head) {
                printf("存储分配失败");
            }
            t.size += 1;
        }
        for (i = t.length - 1; i >= add - 1; i--) {
            t.head[i + 1] = t.head[i];
        }
        t.head[add - 1] = elem;
        t.length++;
        return t;
    }
table delTable(table t, int add) {
        int i;
        if (add > t.length || add < 1) {
            printf("被删除元素的位置有误");
            exit(0);
        }
        for (i = add; i < t.length; i++) {
            t.head[i - 1] = t.head[i];
        }
        t.length--;
        return t;
    }
int selectTable(table t, int elem) {
        int i;
        for (i = 0; i < t.length; i++) {
            if (t.head[i] == elem) {
                return i + 1;
            }
        }
        return -1;
    }
table amendTable(table t, int elem, int newElem) {
        int add = selectTable(t, elem);
        t.head[add - 1] = newElem;
        return t;
    }
void displayTable(table t) {
        int i;
        for (i = 0; i < t.length; i++) {
            printf("%d ", t.head[i]);
        }
        printf("\n");
    }

int day = 0;    //目前的天数

bool keda[MAX_REN][MAX_REN];    //某两人之间是否可以通信，是则矩阵元素置1

struct map_node
{
    bool shuxing;
    table ren;      //ren记录位于此点处的人的编号，为变长数组
}map[600][600];     //地图上的单元点，600*600，共36万

struct renmin
{
    int x;          //人的坐标
    int y;
    bool bingle;    //是否病了
    bool kejian;    //其位置是否被监控系统可见
}ren[MAX_REN][MAX_DAY]; //每个人，每一天的数据

table bingren;      //变长数组，记录病人编号

struct Jizhan
{
    int x;
    int y;
}jizhan[4];         //四个基站（簇节点），只有位置

void Init()         //初始化函数
{
    int i = 0, j = 0;   //计数用i，j
    for(i = 0; i < 600; i++)    //循环初始化地图上的单元点
        for(j = 0; j < 600; j++)
        {   
            map[i][j].ren = initTable0();
            if((i%15<=2) && (j%15<=2))
                map[i][j].shuxing=1;        //小区（目前无效）
            else  map[i][j].shuxing=0;      //空地（目前无效）
        }
    
    bingren = initTable1();     //病人数组建立
 
    
    for(i = 0; i < MAX_REN; i++)    //每个人的位置初始化、是否患病为随机事件
    {                               //病人被记录到病人数组中
        ren[i][0].x = rand()%600;
        ren[i][0].y = rand()%600;
        map[ren[i][0].x][ren[i][0].y].ren = addTable(map[ren[i][0].x][ren[i][0].y].ren, i, map[ren[i][0].x][ren[i][0].y].ren.length + 1);
        if(rand()%100 < PERCENT)
        {    
            ren[i][0].bingle = 1;
            bingren = addTable(bingren, i, bingren.length + 1);
        }
        else ren[i][0].bingle = 0;
        ren[i][0].kejian = 0;
    }

    jizhan[0].x = 200;  //基站位置初始化
    jizhan[0].y = 200;
    jizhan[1].x = 400;
    jizhan[1].y = 400;
    jizhan[2].x = 200;
    jizhan[2].y = 400;
    jizhan[3].x = 400;
    jizhan[3].y = 200;
}


void Move()     //移动函数，每个人每天的位置移动，这里很简陋，位置只是均匀分布，与前一天也无关
{
    int i = 0;
    int num = 0;
    int temp = rand()%100;
    //if(temp >= 20 && temp < 50)
    for(i = 0; i < MAX_REN; i++)    //每个人的数据记录到新一天，每个地图点上的人直接覆盖记录
    {
        num = selectTable(map[ren[i][day - 1].x][ren[i][day - 1].y].ren, i);
        map[ren[i][day - 1].x][ren[i][day - 1].y].ren = delTable(map[ren[i][day - 1].x][ren[i][day - 1].y].ren, num);      
        ren[i][day].x = rand()%600;
        ren[i][day].y = rand()%600;
        map[ren[i][day].x][ren[i][day].y].ren = addTable(map[ren[i][day].x][ren[i][day].y].ren, i, map[ren[i][day].x][ren[i][day].y].ren.length + 1);
    }
}


void Infect()   //传染函数，用于在位置移动结束后，统计传染情况，这里也很简陋，每日位于同一点，则感染
{
    int i = 0;
    int j = 1;
    int tempx = 0;
    int tempy = 0;
    int num = 0;
    for(i = 0; i < MAX_REN; i++)    //每个人的病情先延续到新一天
        ren[i][day].bingle = ren[i][day - 1].bingle;
    for(i = 0; i < bingren.length; i++) //查看病人所在位置的其他人，全部感染，记录为病人
    {
        tempx = ren[bingren.head[i]][day].x;    //day 病人位置
        tempy = ren[bingren.head[i]][day].y;
        for(j = 1; j < map[tempx][tempy].ren.length; j++)
        {
            if(selectTable(bingren, map[tempx][tempy].ren.head[j])<0)
            {
                bingren = addTable(bingren, map[tempx][tempy].ren.head[j], bingren.length + 1);
                ren[map[tempx][tempy].ren.head[j]][day].bingle = 1;
            }
        }
    }
}

void Link_New_Node(int jizhan_ok)   //用于通信，若此人没有被监控系统可见
{                                   //且与newnode这个人距离2km内，计他为newnode，令其可见，再进一步遍历
    for(int i = 0; i < MAX_REN; i++)
        if(keda[jizhan_ok][i] && (!ren[i][day].kejian))
        {
            ren[i][day].kejian = 1;
            Link_New_Node(i);
        }
}

void Communicate()      //通信函数，统计监控系统可见的人
{
    int i = 0, j = 0;
    int distance = 0;
    table jizhan_1 = initTable1();  //基站可见的人，以这些人为起始，2km内搜索

    for(i = 0; i < MAX_REN; i++)    //计算每个人之间是否两两可通信
        for(j = 0; j < MAX_REN; j++)
        {
            distance = abs(ren[i][day].x - ren[j][day].x) + abs(ren[i][day].y - ren[j][day].y);
            if(distance <= 40)
                keda[i][j] = 1;
            else
                keda[i][j] = 0;           
        }
    
    for(i = 0; i < MAX_REN; i++)
        ren[i][day].kejian = 0;

    for(i = 0; i < MAX_REN; i++)    //判断四个基站一步可见的人，记入jizhan_1数组
    {
        distance = abs(ren[i][day].x - jizhan[0].x) + abs(ren[i][day].y - jizhan[0].y);
        if(distance <= 40)
        {
            jizhan_1 = addTable(jizhan_1, i, jizhan_1.length + 1);
            ren[i][day].kejian = 1;
        }
        distance = abs(ren[i][day].x - jizhan[1].x) + abs(ren[i][day].y - jizhan[1].y);
        if(distance <= 40)
        {
            jizhan_1 = addTable(jizhan_1, i, jizhan_1.length + 1);
            ren[i][day].kejian = 1;
        }
        distance = abs(ren[i][day].x - jizhan[2].x) + abs(ren[i][day].y - jizhan[2].y);
        if(distance <= 40)
        {
            jizhan_1 = addTable(jizhan_1, i, jizhan_1.length + 1);
            ren[i][day].kejian = 1;
        }
        distance = abs(ren[i][day].x - jizhan[3].x) + abs(ren[i][day].y - jizhan[3].y);
        if(distance <= 40)
        {
            jizhan_1 = addTable(jizhan_1, i, jizhan_1.length + 1);
            ren[i][day].kejian = 1;
        }              
    }

    for(i = 0; i < jizhan_1.length; i++)    //以基站一步可见的人为起始，传播可见性
        Link_New_Node(jizhan_1.head[i]);
}

int main()  //main函数
{
    int beicha_ren = 0;
    int beicha_day = 0;
    int beicha_x = 0;
    int beicha_y = 0;
    int i = 0;
    int distance = 0;
    int count = 0;
    srand((unsigned)time(NULL));//srand()函数产生一个以当前时间开始的随机种子
    Init();     //初始化

    //displayTable(bingren);
    //printf("\n%d",bingren.length);

    for(day = 1; day < MAX_DAY; day++)
    {
        Move();     //每天进行三步，人移动，判感染，判监控系统是否可见
        Infect();
        Communicate();

        //printf("\n%d",day);
    }

    printf("数据已计算完成\n");

    while(1)        //已经得到数据，以下实现查询功能
    {
        printf("请输入要查询的[人的编号]，要查询的[日期]（数 数）\n");
        scanf("%d %d", &beicha_ren, &beicha_day);
        printf("\n已录入，此人在最近14天内的行动轨迹如下：\n");
        for(i = 0; i < 14; i++)
        {
            if(beicha_day - i <= 0) break;
            if(i == 7) printf("\n");
            if(ren[beicha_ren][beicha_day - i].kejian)
                printf("%d:(%d %d)  ", beicha_day - i, ren[beicha_ren][beicha_day - i].x, ren[beicha_ren][beicha_day - i].y);
            else
                printf("%d:不可见  ", beicha_day - i);
        }

        printf("\n\n请输入要查询的[位置]（x y）和要查询的[日期]（数 数 数）\n");
        scanf("%d %d %d", &beicha_x, &beicha_y, &beicha_day);
        printf("\n已录入，(%d %d)在第 %d 天 1km 范围内的感染者人数为：  ", beicha_x, beicha_y, beicha_day);
        count = 0;
        for(i = 0; i < bingren.length; i++)
        {
            if(ren[bingren.head[i]][beicha_day].kejian)
            {
                distance = abs(ren[bingren.head[i]][beicha_day].x - beicha_x) + abs(ren[bingren.head[i]][beicha_day].y - beicha_y);
                if(distance <= 20) count++;
            }
        }
        printf("%d\n\n", count);
        printf("**************************************************************\n");
    }
    
    //displayTable(bingren);
    //printf("\n%d",bingren.length);

    // int count = 0;
    // for(i = 0; i < MAX_REN; i++)
    //     if(ren[i][30].kejian)
    //         count++;
    // printf("\n%d",count);

    //printf("\n%d\n%d\n%d\n",ren->x,ren->y,map[ren->x][ren->y].shuxing);

    return 0;
}