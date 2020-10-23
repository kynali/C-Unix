#include <stdio.h>

struct list_head
{
    struct list_head *next;
};
struct my_node
{
    int data;
    struct list_head list;
};

struct my_node2
{
    double data;
    struct list_head list;
};

int main()
{
    struct list_head mylisthead = {&mylisthead};
    struct my_node node1 = {666, &node1.list};
    struct my_node2 node2 = {777.77, &node2.list};
    mylisthead.next = &node1.list;
    mylisthead.next->next = &node2.list;
    struct my_node2 *n = mylisthead.next->next - 1;
    printf("\n%d %f %d %d\n", *(mylisthead.next-1),n->data,&node2.data,&node2);
}