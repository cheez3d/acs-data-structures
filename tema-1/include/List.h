#ifndef LIST_H
#define LIST_H

#include "Data.h"

#include <stdbool.h>
#include <stddef.h>

struct ListNode;
struct List;

typedef int (*List_comp_func_t)(const struct Data *first,
                                const struct Data *second);

typedef void (*List_print_func_t)(const struct Data *data);

struct List * List_Create();

struct Data * ListNode_GetData(const struct ListNode *node);

struct ListNode * ListNode_GetNext(const struct ListNode *node);

struct ListNode * ListNode_GetPrev(const struct ListNode *node);

size_t List_GetSize(const struct List *list);

bool List_IsEmpty(const struct List *list);

bool ListNode_IsHeadOrTail(const struct ListNode *node);

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

bool ListNode_IsBounded(const struct ListNode *node,
                        const struct ListNode *left,
                        const struct ListNode *right);

bool ListNode_AreNeighbors(const struct ListNode *left,
                           const struct ListNode *right);

struct ListNode * List_GetFirstNode(const struct List *list);

struct ListNode * List_GetLastNode(const struct List *list);

struct ListNode * List_GetNodesMiddle(const struct List *list,
                                      const struct ListNode *left,
                                      const struct ListNode *right);

struct ListNode * List_GetMiddleNode(const struct List *list);

bool List_ContainsNode(const struct List *list,
                       const struct ListNode *node);

struct ListNode * List_ContainsData(const struct List *list,
                                    const struct Data *data,
                                    List_comp_func_t comp_func);

void List_GetSpanNode(const struct List *list,
                      const struct ListNode *start,
                      size_t span,
                      bool forward,
                      struct ListNode **out_end,
                      size_t *out_span);
                                    
void List_GetRadiusNodes(const struct List *list,
                         const struct ListNode *center,
                         size_t radius,
                         struct ListNode **out_left,
                         struct ListNode **out_right,
                         size_t *out_leftRadius,
                         size_t *out_rightRadius);

struct ListNode * List_AddNodeBetween(struct List *list,
                                      const struct ListNode *node,
                                      struct ListNode *left,
                                      struct ListNode *right);

struct ListNode * List_AddNodeAfter(struct List *list,
                                    const struct ListNode *node,
                                    struct ListNode *ref);

struct ListNode * List_AddNodeBefore(struct List *list,
                                     const struct ListNode *node,
                                     struct ListNode *ref);

struct ListNode * List_AddNodeFirst(struct List *list,
                                    const struct ListNode *node);

struct ListNode * List_AddNodeLast(struct List *list,
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

struct ListNode * List_AddDataFirst(struct List *list,
                                    const struct Data *data);

struct ListNode * List_AddDataLast(struct List *list,
                                   const struct Data *data);

struct List * List_Copy(const struct List *list);

struct List * List_CreateSubNodes(const struct List *list,
                                  const struct ListNode *left,
                                  const struct ListNode *right);

struct List * List_CreateSubSpan(const struct List *list,
                                        const struct ListNode *start,
                                        size_t span,
                                        bool forward);

struct List * List_CreateSubRadius(const struct List *list,
                                   const struct ListNode *center,
                                   size_t radius);

void List_Print(const struct List *list, List_print_func_t print_func);

void List_SwapNodes(struct List *list,
                    struct ListNode *first,
                    struct ListNode *second);

struct ListNode * List_Partition(struct List *list,
                                 struct ListNode *left,
                                 struct ListNode *right,
                                 struct ListNode *pivot,
                                 List_comp_func_t comp_func);

void List_Sort(struct List *list, List_comp_func_t comp_func);

struct Data * List_RemoveNode(struct List *list,
                              struct ListNode *node,
                              bool pop);

struct Data * List_RemoveFirstNode(struct List *list, bool pop);

struct Data * List_RemoveLastNode(struct List *list, bool pop);

struct Data * List_RemoveData(struct List *list,
                              const struct Data *data,
                              List_comp_func_t comp_func,
                              bool pop);

void List_Clear(struct List *list);

void List_Destroy(struct List *list);

#endif // LIST_H
