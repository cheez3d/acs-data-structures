#ifndef TREE_H
#define TREE_H

#include "Data.h"
#include "List.h"

struct TreeNode;
struct Tree;

struct Data * TreeNode_GetData(const struct TreeNode *node);

const struct List * TreeNode_GetChildren(const struct TreeNode *node);

struct TreeNode * TreeNode_GetParent(const struct TreeNode *node);

struct TreeNode * TreeNode_Data_Unwrap(const struct Data *data);

struct TreeNode * Tree_GetRoot(const struct Tree *tree);

struct Tree * Tree_Create();

bool Tree_IsEmpty(const struct Tree *tree);

struct TreeNode * Tree_ContainsData(const struct Tree *tree,
                                    const struct Data *data,
                                    Data_comp_func_t comp_func);

struct List * Tree_GetLeafNodes(const struct Tree *tree);

struct TreeNode * Tree_AddData(struct Tree *tree,
                               const struct Data *data,
                               struct TreeNode *parent);

void Tree_Process(struct Tree *tree,
                  Data_proc_func_t proc_func);

void Tree_Print(const struct Tree *tree,
                Data_print_func_t print_func,
                const char *delim,
                const char *endMark,
                FILE *stream);

void Tree_PrintWithChildren(const struct Tree *tree,
                            Data_print_func_t print_func,
                            const char *delimChildren,
                            const char *delim,
                            const char *endMark,
                            FILE *stream);

struct Data * Tree_RemoveNode(struct Tree *tree,
                              struct TreeNode *node,
                              bool pop);

void Tree_Destroy(struct Tree *tree);

#endif // TREE_H
