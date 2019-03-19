#pragma once

#include <stdbool.h>

struct LinkedListData {
    void *mem;
    size_t size;
};

struct LinkedListNode {
    void *data;
    size_t size;
    
    // struct LinkedListData *data;
    
    struct LinkedListNode *next;
    struct LinkedListNode *prev;
};

struct LinkedList {
    size_t size;
    
    struct LinkedListNode *head;
    struct LinkedListNode *tail;
};

typedef int (*LinkedList_comp_func_t)(const void *first, const void *second);
typedef void (*LinkedList_print_func_t)(const void *data);

// LinkedListNode functions

void * LinkedListNode_CreateData(const void *data, size_t size);

void LinkedListNode_DestroyData(void *data);

// struct LinkedListNode * LinkedListNode_Create(const void *data, size_t size);

void * LinkedListNode_GetData(const struct LinkedListNode *linkedListNode);

struct LinkedListNode * LinkedListNode_GetNext(const struct LinkedListNode *linkedListNode);

struct LinkedListNode * LinkedListNode_GetPrev(const struct LinkedListNode *linkedListNode);

// void LinkedListNode_Destroy(struct LinkedListNode *linkedListNode);

// LinkedList functions

struct LinkedList * LinkedList_Create();

size_t LinkedList_GetSize(const struct LinkedList *linkedList);

bool LinkedList_IsEmpty(const struct LinkedList *linkedList);

struct LinkedListNode * LinkedList_GetBegin(const struct LinkedList *linkedList);

struct LinkedListNode * LinkedList_GetEnd(const struct LinkedList *linkedList);

struct LinkedListNode * LinkedList_ContainsNode(
    const struct LinkedList *linkedList,
    const struct LinkedListNode *linkedListNode
);

struct LinkedListNode * LinkedList_ContainsData(
    const struct LinkedList *linkedList,
    const void *data,
    LinkedList_comp_func_t LinkedList_comp_func
);

struct LinkedListNode * LinkedList_AddNodeBetween(
    struct LinkedList *linkedList,
    struct LinkedListNode *linkedListNode,
    struct LinkedListNode *linkedListLeft,
    struct LinkedListNode *linkedListRight
);

struct LinkedListNode * LinkedList_AddNodeAfter(
    struct LinkedList *linkedList,
    struct LinkedListNode *linkedListNode,
    struct LinkedListNode *linkedListRef
);

struct LinkedListNode * LinkedList_AddNodeBefore(
    struct LinkedList *linkedList,
    struct LinkedListNode *linkedListNode,
    struct LinkedListNode *linkedListRef
);

struct LinkedListNode * LinkedList_AddNodeBegin(
    struct LinkedList *linkedList,
    struct LinkedListNode *linkedListNode
);

struct LinkedListNode * LinkedList_AddNodeEnd(
    struct LinkedList *linkedList,
    struct LinkedListNode *linkedListNode
);

struct LinkedListNode * LinkedList_AddDataBetween(
    struct LinkedList *linkedList,
    const void *data, size_t size,
    struct LinkedListNode *linkedListLeft,
    struct LinkedListNode *linkedListRight
);

struct LinkedListNode * LinkedList_AddDataAfter(
    struct LinkedList *linkedList,
    const void *data, size_t size,
    struct LinkedListNode *linkedListRef
);

struct LinkedListNode * LinkedList_AddDataBefore(
    struct LinkedList *linkedList,
    const void *data, size_t size,
    struct LinkedListNode *linkedListRef
);

struct LinkedListNode * LinkedList_AddDataBegin(
    struct LinkedList *linkedList,
    const void *data, size_t size
);

struct LinkedListNode * LinkedList_AddDataEnd(
    struct LinkedList *linkedList,
    const void *data, size_t size
);

void * LinkedList_RemoveNode(
    struct LinkedList *linkedList,
    struct LinkedListNode *linkedListNode
);

void * LinkedList_RemoveBegin(struct LinkedList *linkedList);

void * LinkedList_RemoveEnd(struct LinkedList *linkedList);

void * LinkedList_RemoveData(
    struct LinkedList *linkedList,
    const void *data,
    LinkedList_comp_func_t LinkedList_comp_func
);

void LinkedList_Print(
    const struct LinkedList *linkedList,
    LinkedList_print_func_t LinkedList_print_func
);

void LinkedList_Destroy(struct LinkedList *linkedList);
