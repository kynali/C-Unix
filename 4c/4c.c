#include <stdio.h>
#include <stdlib.h>

typedef int Type;

//定义二叉树结点
typedef struct BiTree_root
{
    struct BiTree_root *left;
    struct BiTree_root *right;
} Root;

// 定义二叉树数据结构体
typedef struct Node
{
    Type data;
    Root root;
} BiTreeNode;

// 初始化二叉树
Root *Initiate(Root *root, Type x)
{
    BiTreeNode *node = malloc(sizeof(BiTreeNode));
    //root = malloc(sizeof(Root));
    node->data = x;
    root = &(node->root);
    root->left = NULL;
    root->right = NULL;
    return root;
}

//插入结点的左子树
void InsertLeftNode(Root *root, Type x)
{
    BiTreeNode *leftnode = malloc(sizeof(BiTreeNode));
    leftnode->data = x;
    root->left = &(leftnode->root);
    root->left->left = NULL;
    root->left->right = NULL;
}

// 插入右子树
void InsertRightNode(Root *root, Type x)
{
    BiTreeNode *rightnode = malloc(sizeof(BiTreeNode));
    rightnode->data = x;
    root->right = &(rightnode->root);
    root->left->left = NULL;
    root->left->right = NULL;
}

// 销毁这个结点及其所有子树
void Destroy(Root *root)
{
    if (root != NULL && root->left != NULL)
        Destroy(root->left);
    if (root != NULL && root->right != NULL)
        Destroy(root->right);
    free(root);
}

// 删除所指结点的左子树
void DeleteLeftTree(Root *root)
{
    Destroy(root->left);
    root->left = NULL;
}

// 删除所指结点的右子树
void DeleteRightTree(Root *root)
{
    Destroy(root->right);
    root->right = NULL;
}

// 打印二叉树
void PrintBiTree(Root *root, int n)
{
    if (root == NULL)
        return;
    PrintBiTree(root->right, n + 1);
    for (int i = 0; i < n - 1; i++)
        printf("   ");
    if (n > 0)
    {
        printf("---");
        BiTreeNode *n = (int *)root - 2;
        printf("%d\n", n->data);
    }

    PrintBiTree(root->left, n + 1);
}

int main()
{
    Root *root;
    root = Initiate(root, 1);
    InsertLeftNode(root, 2);
    InsertRightNode(root, 3);
    InsertLeftNode(root->left, 4);
    InsertRightNode(root->left, 5);
    InsertLeftNode(root->right, 6);
    InsertRightNode(root->right, 7);
    PrintBiTree(root, 1);
    return 0;
}