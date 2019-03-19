#pragma once

#include <stdbool.h>
#include <stddef.h>

struct LinkedListData {
    void *mem;
    size_t size;
};

struct LinkedListNode {
    struct LinkedListData *data;
    
    struct LinkedListNode *next;
    struct LinkedListNode *prev;
};

struct LinkedList {
    size_t size;
    
    struct LinkedListNode *head;
    struct LinkedListNode *tail;
};

typedef int (*LinkedList_comp_func_t)(const struct LinkedListData *first, const struct LinkedListData *second);
typedef void (*LinkedList_print_func_t)(const struct LinkedListData *data);

// LinkedListData functions

size_t LinkedListData_GetSize(const struct LinkedListData *linkedListData);

void LinkedListData_Destroy(struct LinkedListData *linkedListData);

// LinkedListNode functions

struct LinkedListData * LinkedListNode_GetData(const struct LinkedListNode *linkedListNode);

struct LinkedListNode * LinkedListNode_GetNext(const struct LinkedListNode *linkedListNode);

struct LinkedListNode * LinkedListNode_GetPrev(const struct LinkedListNode *linkedListNode);

// LinkedList functions

struct LinkedList * LinkedList_Create();

size_t LinkedList_GetSize(const struct LinkedList *linkedList);

bool LinkedList_IsEmpty(const struct LinkedList *linkedList);

struct LinkedListNode * LinkedList_GetFirst(const struct LinkedList *linkedList);

struct LinkedListNode * LinkedList_GetLast(const struct LinkedList *linkedList);

struct LinkedListNode * LinkedList_Contains(
    const struct LinkedList *linkedList,
    const struct LinkedListNode *linkedListNode
);

struct LinkedListNode * LinkedList_ContainsData(
    const struct LinkedList *linkedList,
    const struct LinkedListData *linkedListData,
    LinkedList_comp_func_t LinkedList_comp_func
);

struct LinkedListNode * LinkedList_AddBetween(
    struct LinkedList *linkedList,
    const struct LinkedListNode *linkedListNode,
    struct LinkedListNode *linkedListLeft,
    struct LinkedListNode *linkedListRight
);

struct LinkedListNode * LinkedList_AddAfter(
    struct LinkedList *linkedList,
    const struct LinkedListNode *linkedListNode,
    struct LinkedListNode *linkedListRef
);

struct LinkedListNode * LinkedList_AddBefore(
    struct LinkedList *linkedList,
    const struct LinkedListNode *linkedListNode,
    struct LinkedListNode *linkedListRef
);

struct LinkedListNode * LinkedList_AddFirst(
    struct LinkedList *linkedList,
    const struct LinkedListNode *linkedListNode
);

struct LinkedListNode * LinkedList_AddLast(
    struct LinkedList *linkedList,
    const struct LinkedListNode *linkedListNode
);

struct LinkedListNode * LinkedList_AddDataBetween(
    struct LinkedList *linkedList,
    const struct LinkedListData *linkedListData,
    struct LinkedListNode *linkedListLeft,
    struct LinkedListNode *linkedListRight
);

struct LinkedListNode * LinkedList_AddDataAfter(
    struct LinkedList *linkedList,
    const struct LinkedListData *linkedListData,
    struct LinkedListNode *linkedListRef
);

struct LinkedListNode * LinkedList_AddDataBefore(
    struct LinkedList *linkedList,
    const struct LinkedListData *linkedListData,
    struct LinkedListNode *linkedListRef
);

struct LinkedListNode * LinkedList_AddDataFirst(
    struct LinkedList *linkedList,
    const struct LinkedListData *linkedListData
);

struct LinkedListNode * LinkedList_AddDataLast(
    struct LinkedList *linkedList,
    const struct LinkedListData *linkedListData
);

struct LinkedListData * LinkedList_Remove(
    struct LinkedList *linkedList,
    struct LinkedListNode *linkedListNode,
    bool pop
);

struct LinkedListData * LinkedList_RemoveFirst(struct LinkedList *linkedList, bool pop);

struct LinkedListData * LinkedList_RemoveLast(struct LinkedList *linkedList, bool pop);

struct LinkedListData * LinkedList_RemoveData(
    struct LinkedList *linkedList,
    const struct LinkedListData *linkedListData,
    LinkedList_comp_func_t LinkedList_comp_func,
    bool pop
);

struct LinkedList * LinkedList_GetSub(
    const struct LinkedList *linkedList,
    const struct LinkedListNode *linkedListLeft,
    // TODO: remove linkedList prefix from variables and
    // use private prefix for private variables where necessary
    const struct LinkedListNode *linkedListRight
);

struct LinkedList * LinkedList_GetSubCentered(
    const struct LinkedList *linkedList,
    const struct LinkedListNode *linkedListNode,
    size_t radius
);

void LinkedList_Print(
    const struct LinkedList *linkedList,
    LinkedList_print_func_t LinkedList_print_func
);

void LinkedList_Destroy(struct LinkedList *linkedList);
