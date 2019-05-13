#ifndef BSTREE_H
#define BSTREE_H

#include "Data.h"
#include "List.h"

#include <stdbool.h>
#include <stdio.h>

struct List;

struct BSTreeNode;
struct BSTree;

struct Data * BSTreeNode_GetData(const struct BSTreeNode *node);

struct BSTreeNode * BSTreeNode_GetLeft(const struct BSTreeNode *node);

struct BSTreeNode * BSTreeNode_GetRight(const struct BSTreeNode *node);

struct BSTreeNode * BSTreeNode_GetParent(const struct BSTreeNode *node);

bool BSTreeNode_IsLeaf(const struct BSTreeNode *node);

struct BSTreeNode * BSTreeNode_GetMin(const struct BSTreeNode *node);

struct BSTreeNode * BSTreeNode_GetFirstIn(const struct BSTreeNode *node);

struct BSTreeNode * BSTreeNode_GetFirstPost(const struct BSTreeNode *node);

struct BSTreeNode * BSTreeNode_GetInSuccessor(const struct BSTreeNode *node);

struct BSTreeNode * BSTreeNode_GetPostSuccessor(const struct BSTreeNode *node);

size_t BSTree_GetSize(const struct BSTree *tree);

struct BSTreeNode * BSTree_GetRoot(const struct BSTree *tree);

struct BSTree * BSTree_Create();

bool BSTree_IsEmpty(const struct BSTree *tree);

struct BSTreeNode * BSTree_ContainsData(const struct BSTree *tree,
                                        const struct Data *data,
                                        Data_comp_func_t comp_func);

struct BSTreeNode * BSTree_AddData(struct BSTree *tree,
                                   const struct Data *data,
                                   Data_comp_func_t comp_func);

struct BSTree * BSTree_Copy(const struct BSTree *tree);

struct BSTree * BSTree_CreateFromList(const struct List *list,
                                      Data_comp_func_t comp_func);

struct BSTree * BSTree_Intersect(const struct BSTree *first,
                                 const struct BSTree *second,
                                 Data_comp_func_t comp_func);

void BSTree_Print(const struct BSTree *tree,
                  Data_print_func_t print_func,
                  const char *delim,
                  const char *endMark,
                  FILE *stream);

void BSTree_PrintWithChildren(const struct BSTree *tree,
                              Data_print_func_t print_func,
                              const char *delimChildren,
                              const char *delim,
                              const char *endMark,
                              FILE *stream);

void BSTree_PrintInOrder(const struct BSTree *tree,
                         Data_print_func_t print_func,
                         const char *delim,
                         const char *endMark,
                         FILE *stream);

struct Data * BSTree_RemoveNode(struct BSTree *tree,
                                struct BSTreeNode *node,
                                bool pop);

struct Data * BSTree_RemoveData(struct BSTree *tree,
                                const struct Data *data,
                                Data_comp_func_t comp_func,
                                bool pop);

void BSTree_Clear(struct BSTree *tree);

void BSTree_Destroy(struct BSTree *tree);

#endif // BSTREE_H
