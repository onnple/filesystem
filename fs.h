//
// Created by Once on 2019/7/6.
//

#ifndef DATALGORITHM_FS_H
#define DATALGORITHM_FS_H

#define FILE_MAX_SIZE 10

// 文件数据实体
typedef struct file{
    char file_name[128]; // 关键字，同一目录中唯一
    char file_content[512];
    int is_directory;
} File;

// 文件结点
typedef struct tree Tree;
typedef struct fnode{
    File file;
//    int count; // 文件数量
//    struct fnode *files[FILE_MAX_SIZE]; // 文件数组（顺序表），一个目录最多FILE_MAX_SIZE个文件，
// 也可以使用链表存储，更好的方法可以使用一个二叉树存储（AVL树或伸展树，可以加快查找速度），如：
    Tree *files;
    struct fnode *parent;
} FNode;

// 二叉树ADT，作为一个数据容器，用于存储一个结点的所有儿子，不使用AVL平衡树或伸展树，查找速度最差的情况有可能是O(M)，
// M为儿子数，平均情况为O(logN)
typedef struct node{
//    char *file_name; // 关键字使用File中的文件名
    FNode *fnode; // int fnode
    struct node *left;
    struct node *right;
} Node;
struct tree{
    Node *root;
    int count; // 文件数量
};

// 文件树ADT
typedef struct ftree{
    FNode *root;
    int size;
} FTree;

// 二叉树ADT，数据容器
extern Tree *tree_init();
extern int tree_is_full(Tree *tree);
extern int tree_is_empty(Tree *tree);
extern int tree_add(Tree *tree, FNode *fnode);
extern FNode *tree_search(Tree *tree, char *file_name);
extern void tree_for_each(Tree *tree, void (*run)(FNode *));
extern int tree_clear(Tree *tree);


// 文件树ADT
extern FTree *ftree_init();
extern int ftree_is_full(FTree *ftree);
extern int ftree_is_empty(FTree *ftree);
extern int ftree_add(FTree *ftree, char directory[], File *file);
extern int ftree_is_exist(FTree *ftree, char file_name_absolute[]);
extern void ftree_traverse(FTree *ftree);
extern int ftree_clear(FTree *ftree);

#endif //DATALGORITHM_FS_H
