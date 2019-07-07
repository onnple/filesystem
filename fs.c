//
// Created by Once on 2019/7/6.
//

#include "fs.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// 二叉树ADT，数据容器
Tree *tree_init(){
    Tree *tree = malloc(sizeof(Tree));
    if(!tree){
        perror("init tree failed.");
        return NULL;
    }
    tree->root = NULL;
    tree->count = 0;
    return tree;
}

int tree_is_full(Tree *tree){
    Node *node = (Node *)malloc(sizeof(Node));
    if(!node)
        return 1;
    free(node);
    return 0;
}

int tree_is_empty(Tree *tree){
    if(tree == NULL)
        return 1;
    return tree->count == 0;
}

/**
 * 先序遍历：
 * 先将根取出来处理：为空则添加数据
 * 根不为空，将儿子取出，作为新的根
 * 循环第二步
 * */
static Node* add_node(Tree *tree, Node *root, FNode *fnode){
    if(!root){
        root = (Node*)malloc(sizeof(Node));
        if(!root){
            perror("init node error");
            return NULL;
        }
        root->left = NULL;
        root->right = NULL;
        root->fnode = fnode;
        tree->count++;
        return root;
    }
    if(strcmp(fnode->file.file_name, root->fnode->file.file_name) < 0){
        root->left = add_node(tree, root->left, fnode);
        return root;
    }
    else if(strcmp(fnode->file.file_name, root->fnode->file.file_name) > 0){
        root->right = add_node(tree, root->right, fnode);
        return root;
    }
    else
        return NULL;
}

int tree_add(Tree *tree, FNode *fnode){
    if(tree == NULL || fnode == NULL)
        return 0;
    if(tree->count == 0){
        tree->root = add_node(tree, tree->root, fnode);
        return tree->root != NULL;
    }
    else
        return add_node(tree, tree->root, fnode) != NULL;
}

// 时间复杂度平均为O(logN)
static FNode* search_node(Node *root, char *file_name){
    if(!root)
        return NULL;
    if(strcmp(file_name, root->fnode->file.file_name) == 0)
        return root->fnode;
    else if(strcmp(file_name, root->fnode->file.file_name) < 0)
        return search_node(root->left, file_name);
    else
        return search_node(root->right, file_name);
}

FNode *tree_search(Tree *tree, char *file_name){
    if(tree == NULL || file_name == NULL)
        return NULL;
    return search_node(tree->root, file_name);
}

// 使用中序遍历
static void tree_each(Node *root, void (*run)(FNode*)){
    if(!root)
        return;
    tree_each(root->left, run);
    run(root->fnode);
    tree_each(root->right, run);
}

void tree_for_each(Tree *tree, void (*run)(FNode*)){
    if(tree == NULL || tree->count == 0 || run == NULL)
        return;
    tree_each(tree->root, run);
}

// 使用后序遍历
static int remove_node(Node *root){
    if(!root)
        return 0;
    remove_node(root->left);
    remove_node(root->right);
    free(root);
    return 1;
}

int tree_clear(Tree *tree){
    if(tree == NULL)
        return 0;
    int result =  remove_node(tree->root);
    free(tree);
    return result;
}


// 文件树ADT
FTree *ftree_init(){
    FTree *ftree = (FTree*)malloc(sizeof(FTree));
    if(!ftree){
        perror("init ptree error");
        return NULL;
    }
    ftree->root = NULL;
    ftree->size = 0;
    return ftree;
}

int ftree_is_full(FTree *ftree){
    FNode *fnode = (FNode*)malloc(sizeof(FNode));
    if(!fnode)
        return 1;
    free(fnode);
    return 0;
}

int ftree_is_empty(FTree *ftree){
    if(ftree == NULL)
        return 0;
    return ftree->size == 0;
}

/**
 * 先序遍历：
 * 先取出根结点，在根结点上做比较，为NULL则添加
 * 从根结点搜索出目标儿子结点，将这个儿子结点作为新的根结点
 * 循环第一步
 * */

 /**
  * directory: /bin, fnode: mkdir => /bin/mkdir
  * 1、根目录/是强制要求，不符合直接返回 => OK
  * 2、directory经过的目录，ftree的结点中都有，在最后的目录下添加
  * 3、directory经过的目录，中间结点位置P开始不匹配，在P中开始添加新目录，直到在最后的目录添加
  * */
static FNode *add_fnode(FTree *ftree, FNode *root, char token[], File *file){
    if(!root){
        root = (FNode *)malloc(sizeof(FNode));
        if(!root){
            perror("init fnode error");
            return NULL;
        }
        strcpy(root->file.file_name, token);
        strcpy(root->file.file_content, token);
        root->file.is_directory = file->is_directory;
        root->files = NULL;
        root->parent = NULL;
        if(ftree->size == 0)
            ftree->root = root;
        ftree->size++;
        return root;
    }
    FNode *node = tree_search(root->files, token);
    if(!node){
        FNode *fnode = add_fnode(ftree, node, token, file);
        fnode->parent = root;
        Tree *tree = root->files == NULL ? tree_init() : root->files;
        tree_add(tree, fnode);
        root->files = tree;
        char *t = strtok(NULL, "/");
        if(!t)
            return fnode;
        else
            return add_fnode(ftree, fnode, t, file);
    }
    else
        return add_fnode(ftree, node, strtok(NULL, "/"), file);
}

int ftree_add(FTree *ftree, char directory[], File *file){
    if(ftree == NULL || directory == NULL || file == NULL || directory[0] != '/')
        return 0;
    char path[strlen(directory) + strlen(file->file_name)];
    strcpy(path, directory);
    strcat(path, "/");
    strcat(path, file->file_name);
    char *token = strtok(path, "/");
    if(token == NULL || strlen(token) == 0)
        token = "/";
    return add_fnode(ftree, ftree->root, token, file) != NULL;
}

static int node_exist(FNode *root, char token[]){
    FNode *node = tree_search(root->files, token);
    char *t = strtok(NULL, "/");
    if(node){
        if(!t)
            return 1;
        else
            return node_exist(node, t);
    }
    else
        return 0;
}

int ftree_is_exist(FTree *ftree, char file_name_absolute[]){
    if(file_name_absolute == NULL)
        return 0;
    if(strlen(file_name_absolute) == 1 && file_name_absolute[0] == '/')
        return 1;
    if(ftree == NULL || file_name_absolute[0] != '/')
        return 0;
    char path[strlen(file_name_absolute)];
    strcpy(path, file_name_absolute);
    char *token = strtok(path, "/");
    return node_exist(ftree->root, token);
}

static void traverse(FNode *root);
static void run(FNode *fnode){
    printf("/%s", fnode->file.file_name);
    traverse(fnode);
}

static void traverse(FNode *root){
    if(!root->files){
        printf("\n");
        return;
    }
    tree_for_each(root->files, &run);
}

void ftree_traverse(FTree *ftree){
    traverse(ftree->root);
    printf("\n");
}

/**
 * 后缀遍历：
 * 从根取出所有儿子结点，作为新的根结点
 * 删除根结点
 * */
static int tree_each_remove_task(Node *root, void (*task)(FNode *)){
    if(!root)
        return 0;
    tree_each_remove_task(root->left, task);
    tree_each_remove_task(root->right, task);
    task(root->fnode);
    return 1;
}
static int tree_each_remove(Tree *tree, void (*task)(FNode *)){
    if(tree == NULL || tree->count == 0 || task == NULL)
        return 0;
    return tree_each_remove_task(tree->root, task);
}

static int remove_ftree(FNode *root);
static void task(FNode *root){
    remove_ftree(root);
}

static int remove_ftree(FNode *root){
    if(!root)
        return 0;
    Tree *tree = root->files;
    tree_each_remove(tree, &task);
    tree_clear(tree);
    free(root);
    return 1;
}

int ftree_clear(FTree *ftree){
    if(ftree == NULL)
        return 0;
    int result = remove_ftree(ftree->root);
    free(ftree);
    return result;
}
