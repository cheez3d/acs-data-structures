#ifndef LIST_H
#define LIST_H

#include "BSTree.h"
#include "Data.h"

#include <stdbool.h>
#include <stdio.h>

struct BSTree;

struct ListNode;
struct List;

struct Data * ListNode_GetData(const struct ListNode *node);

struct ListNode * ListNode_GetNext(const struct ListNode *node);

struct ListNode * ListNode_GetPrev(const struct ListNode *node);

struct ListNode * ListNode_Data_Unwrap(const struct Data *data);

size_t List_GetSize(const struct List *list);

struct List * List_Create();

bool List_IsEmpty(const struct List *list);

bool ListNode_IsAfter(const struct ListNode *node,
                      const struct ListNode *ref);

bool ListNode_IsBefore(const struct ListNode *node,
                       const struct ListNode *ref);

bool ListNode_IsBetween(const struct ListNode *node,
                        const struct ListNode *left,
                        const struct ListNode *right);

bool ListNode_IsSuccessor(const struct ListNode *node,
                          const struct ListNode *ref);

bool ListNode_IsPredecessor(const struct ListNode *node,
                            const struct ListNode *ref);

bool ListNode_IsBoundedBy(const struct ListNode *node,
                          const struct ListNode *left,
                          const struct ListNode *right);

bool ListNode_IsNeighborWith(const struct ListNode *left,
                             const struct ListNode *right);

struct ListNode * List_GetFirstNode(const struct List *list);

struct ListNode * List_GetLastNode(const struct List *list);

struct ListNode * List_GetNodeAtIndex(const struct List *list,
                                      size_t index);

struct ListNode * List_GetNodesMiddle(const struct List *list,
                                      const struct ListNode *left,
                                      const struct ListNode *right);

struct ListNode * List_GetMiddleNode(const struct List *list);

bool List_ContainsNode(const struct List *list,
                       const struct ListNode *node);

struct ListNode * List_ContainsDataStep(const struct List *list,
                                        const struct Data *data,
                                        Data_comp_func_t comp_func,
                                        size_t stepSize);

struct ListNode * List_ContainsData(const struct List *list,
                                    const struct Data *data,
                                    Data_comp_func_t comp_func);

void ListNode_GetSpanNode(const struct ListNode *start,
                          size_t span,
                          bool forward,
                          struct ListNode **out_end,
                          size_t *out_span);
                                    
void ListNode_GetRadiusNodes(const struct ListNode *center,
                             size_t radius,
                             struct ListNode **out_left,
                             struct ListNode **out_right,
                             size_t *out_leftRadius,
                             size_t *out_rightRadius);

struct ListNode * List_AddNodeCopyBetween(struct List *list,
                                          const struct ListNode *node,
                                          struct ListNode *left,
                                          struct ListNode *right);

struct ListNode * List_AddNodeCopyAfter(struct List *list,
                                        const struct ListNode *node,
                                        struct ListNode *ref);

struct ListNode * List_AddNodeCopyBefore(struct List *list,
                                         const struct ListNode *node,
                                         struct ListNode *ref);

struct ListNode * List_AddNodeCopyAtIndex(struct List *list,
                                          const struct ListNode *node,
                                          size_t index);

struct ListNode * List_AddNodeCopyFirst(struct List *list,
                                        const struct ListNode *node);

struct ListNode * List_AddNodeCopyLast(struct List *list,
                                       const struct ListNode *node);

struct ListNode * List_AddDataBetween(struct List *list,
                                      const struct Data *data,
                                      struct ListNode *left,
                                      struct ListNode *right);

struct ListNode * List_AddDataAfter(struct List *list,
                                    const struct Data *data,
                                    struct ListNode *ref);

struct ListNode * List_AddDataBefore(struct List *list,
                                     const struct Data *data,
                                     struct ListNode *ref);

struct ListNode * List_AddDataAtIndex(struct List *list,
                                      const struct Data *data,
                                      size_t index);

struct ListNode * List_AddDataFirst(struct List *list,
                                    const struct Data *data);

struct ListNode * List_AddDataLast(struct List *list,
                                   const struct Data *data);

struct List * List_Copy(const struct List *list);

struct List * List_CreateSubFromNodes(const struct List *list,
                                      const struct ListNode *left,
                                      const struct ListNode *right);

struct List * List_CreateSubFromSpan(const struct List *list,
                                     const struct ListNode *start,
                                     size_t span,
                                     bool forward);

struct List * List_CreateSubFromRadius(const struct List *list,
                                       const struct ListNode *center,
                                       size_t radius);

struct List * List_CreateFromBSTree(const struct BSTree *tree);

void List_Process(struct List *list,
                  Data_proc_func_t proc_func);

void List_Print(const struct List *list,
                Data_print_func_t print_func,
                const char *delim,
                const char *endMark,
                FILE *stream);

void List_SwapNodes(struct List *list,
                    struct ListNode *first,
                    struct ListNode *second);

struct ListNode * List_Partition(struct List *list,
                                 struct ListNode *left,
                                 struct ListNode *right,
                                 struct ListNode *pivot,
                                 Data_comp_func_t comp_func);

void List_Sort(struct List *list, Data_comp_func_t comp_func);

struct Data * List_RemoveNode(struct List *list,
                              struct ListNode *node,
                              bool pop);

struct Data * List_RemoveFirstNode(struct List *list, bool pop);

struct Data * List_RemoveLastNode(struct List *list, bool pop);

struct Data * List_RemoveData(struct List *list,
                              const struct Data *data,
                              Data_comp_func_t comp_func,
                              bool pop);

void List_Clear(struct List *list);

void List_Destroy(struct List *list);

#endif // LIST_H
