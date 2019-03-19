// TODO: add setters

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "LinkedList.h"

struct Data {
    void *mem;
    size_t size;
};

struct Node {
    struct Data *data;
    
    struct Node *next;
    struct Node *prev;
};

struct List {
    size_t size;
    
    struct Node *head;
    struct Node *tail;
};

typedef int (*comp_func_t)(const struct Data *first, const struct Data *second);
typedef void (*print_func_t)(const struct Data *data);
// typedef void (*process_func_t)(struct Node *node); // TODO: investigate this

// PRIVATE

// Data functions

static struct Data * CreateData(const void *mem, size_t size) {
    if (!mem) { goto error; }
    if (size == 0) { goto error; }
    
    struct Data *data = malloc(sizeof *data);
    
    data->mem = memcpy(malloc(size), mem, size);
    data->size = size;
    
    return data;
    
error:
    return NULL;
}

static struct Data * CopyData(const struct Data *data) {
    if (!data) { goto error; }
    
    return CreateData(data->mem, data->size);
    
error:
    return NULL;
}

static size_t GetDataSize(const struct Data *data) {
    if (!data) { goto error; }
    
    return data->size;
    
error:
    return 0;
}

static void DestroyData(struct Data *data) {
    if (!data) { goto error; }
    
    free(data->mem);
    
    free(data);
    
    return;
    
error:
    return;
}

// Node functions

static struct Node * CreateNode(const struct Data *data) {
    struct Node *node = malloc(sizeof *node); // creeaza nodul de adaugat
    
    node->data = CopyData(data); // copiaza datele in nod
    
    // initializeaza legaturile nodului
    node->next = NULL;
    node->prev = NULL;
    
    return node; // returneaza nodul
}

static struct Node * CopyNode(const struct Node *node) {
    if (!node) { goto error; }
    
    return CreateNode(node->data);
    
error:
    return NULL;
}

static struct Data * GetNodeData(const struct Node *node) {
    if (!node) { goto error; }
    
    return node->data;
    
error:
    return NULL;
}

static struct Node * GetNodeNext(const struct Node *node) {
    if (!node) { goto error; }
    
    return node->next;
    
error:
    return NULL;
}

static struct Node * GetNodePrev(const struct Node *node) {
    if (!node) { goto error; }
    
    return node->prev;
    
error:
    return NULL;
}

static struct Data * DestroyNode(struct Node *node) {
    if (!node) { goto error; }
    
    struct Data *data = node->data;
    
    free(node); // elibereaza memoria ocupata de nod
    
    return data;
    
error:
    return NULL;
}

// List functions

static struct List * Create() {
    // creeaza lista
    struct List *list = malloc(sizeof *list);
    
    // creeaza nodurile santinela de inceput si de sfarsit al listei
    struct Node *head = CreateNode(NULL);
    struct Node *tail = CreateNode(NULL);
    
    // leaga cele doua noduri santinela intre ele
    head->next = tail;
    tail->prev = head;
    
    // initializeaza lista
    list->size = 0;
    
    list->head = head;
    list->tail = tail;
    
    return list; // returneaza lista
}

// static struct List * Copy(const struct List *list) {
    
// }

static size_t GetSize(const struct List *list) {
    if (!list) { goto error; }
    
    return list->size;
    
error:
    return 0;
}

static bool IsEmpty(const struct List *list) {
    if (!list) { goto error; }
    
    return GetSize(list) == 0;
    
error:
    return true;
}

static struct Node * GetHead(const struct List *list) {
    if (!list) { goto error; }
    
    return list->head;
    
error:
    return NULL;
}

static struct Node * GetTail(const struct List *list) {
    if (!list) { goto error; }
    
    return list->tail;
    
error:
    return NULL;
}

static struct Node * GetFirst(const struct List *list) {
    return GetNodeNext(GetHead(list));
}

static struct Node * GetLast(const struct List *list) {
    return GetNodePrev(GetTail(list));
}

static struct Node * Contains(const struct List *list, const struct Node *node) {
    if (node == GetHead(list)) { return GetHead(list); }
    if (node == GetTail(list)) { return GetTail(list); }
    
    // incepe cautarea simultan atat de la inceputul, cat si de la sfarsitul listei
    struct Node *left = GetFirst(list);
    struct Node *right = GetLast(list);
    
    // cat timp cele doua directii de cautare nu s-au intalnit
    // (se verifica intalnirea atat pentru cazul cand lista are un numar
    //  impar de elemente, cat si pentru cazul cand lista are un numar
    //  par de elemente)
    while (!(GetNodePrev(left) == GetNodeNext(right)) && !(GetNodePrev(left) == right && GetNodeNext(right) == left)) {
        // verifica daca nodul a fost gasit
        if (left == node) { return left; }
        if (right == node) { return right; }
        
        // avanseaza cautarea
        left = GetNodeNext(left);
        right = GetNodePrev(right);
    }
    
    return NULL;
}

static struct Node * ContainsData(const struct List *list, const struct Data *data, comp_func_t comp_func) {
    if (!comp_func) { goto error; }
    
    // incepe cautarea de la inceputul listei
    struct Node *node = GetFirst(list);
    
    while (node != GetTail(list)) {
        // verifica daca a fost gasit un nod ale carui date corespund
        // cu cele cautate
        if (comp_func(GetNodeData(node), data) == 0) { return node; }
        
        // avanseaza cautarea
        node = GetNodeNext(node);
    }
    
    return NULL;
    
error:
    return NULL;
}

static struct Node * AddBetween(struct List *list, struct Node *node, struct Node *left, struct Node *right) {
    if (!list) { goto error; }
    if (!node) { goto error; }
    if (!left) { goto error; }
    if (!right) { goto error; }
    
    // verifica daca nodurile intre care se insereaza se afla in lista
    if (!Contains(list, left)) { goto error; }
    if (!Contains(list, right)) { goto error; }
    
    // verifica daca se incearca inserarea inainte de nodul santinela de inceput
    // sau dupa nodul santinela de sfarsit
    if (right == GetHead(list)) { goto error; }
    if (left == GetTail(list)) { goto error; }
    
    // verifica daca nodurile intre care se insereaza se succed
    if (GetNodeNext(left) != right) { goto error; }
    if (GetNodePrev(right) != left) { goto error; }
    
    // configureaza legaturile nodului
    node->prev = left;
    node->next = right;
    
    // insereaza nodul in lista
    left->next = node;
    right->prev = node;
    
    ++(list->size); // incrementeaza numarul de noduri din lista
    
    return node; // returneaza nodul inserat
    
error:
    return NULL;
}

static struct Node * AddAfter(struct List *list, struct Node *node, struct Node *ref) {
    return AddBetween(list, node, ref, GetNodeNext(ref));
}

static struct Node * AddBefore(struct List *list, struct Node *node, struct Node *ref) {
    return AddBetween(list, node, GetNodePrev(ref), ref);
}

static struct Node * AddFirst(struct List *list, struct Node *node) {
    return AddAfter(list, node, GetHead(list));
}

static struct Node * AddLast(struct List *list, struct Node *node) {
    return AddBefore(list, node, GetTail(list));
}

static struct Node * AddDataBetween(struct List *list, const struct Data *data, struct Node *left, struct Node *right) {
    struct Node *node = CreateNode(data);
    
    if (!AddBetween(list, node, left, right)) { goto error; }
    
    return node;
    
error:
    DestroyNode(node);
    
    return NULL;
}

static struct Node * AddDataAfter(struct List *list, const struct Data *data, struct Node *ref) {
    return AddDataBetween(list, data, ref, GetNodeNext(ref));
}

static struct Node * AddDataBefore(struct List *list, const struct Data *data, struct Node *ref) {
    return AddDataBetween(list, data, GetNodePrev(ref), ref);
}

static struct Node * AddDataFirst(struct List *list, const struct Data *data) {
    return AddDataAfter(list, data, GetHead(list));
}

static struct Node * AddDataLast(struct List *list, const struct Data *data) {
    return AddDataBefore(list, data, GetTail(list));
}

static struct Data * Remove(struct List *list, struct Node *node, bool pop) {
    if (!list) { goto error; }
    if (!node) { goto error; }
    
    if (GetSize(list) == 0) { goto error; } // verifica daca lista este goala
    
    // verifica daca se incearca eliminarea vreunui nod santinela
    if (node == GetHead(list)) { goto error; }
    if (node == GetTail(list)) { goto error; }
    
    // verifica daca nodul se afla in lista
    if (!Contains(list, node)) { goto error; }
    
    // inlatura nodul din lista
    if (GetNodeNext(node)) { GetNodeNext(node)->prev = GetNodePrev(node); }
    if (GetNodePrev(node)) { GetNodePrev(node)->next = GetNodeNext(node); }
    
    --(list->size); // decrementeaza numarul de noduri din lista
    
    struct Data *data = DestroyNode(node);
    
    return pop ? data : (DestroyData(data), NULL); // returneaza/distruge datele ramase
    
error:
    return NULL;
}

static struct Data * RemoveFirst(struct List *list, bool pop) {
    return Remove(list, GetFirst(list), pop);
}

static struct Data * RemoveLast(struct List *list, bool pop) {
    return Remove(list, GetLast(list), pop);
}

static struct Data * RemoveData(struct List *list, const struct Data *data, comp_func_t comp_func, bool pop) {
    // cauta un nod care sa corespunda cu datele specificate
    struct Node *node = ContainsData(list, data, comp_func);
    
    if (!node) { goto error; } // verifica daca nodul nu a fost gasit
    
    return Remove(list, node, pop); // sterge nodul
    
error:
    return NULL;
}

static struct List * GetSub(const struct List *list, const struct Node *left, const struct Node *right) {
    // if (!list) { goto error; }
    if (!left) { goto error; }
    if (!right) { goto error; }
    
    // verifica daca nodurile intre care extragem sublista se afla in lista
    if (!Contains(list, left)) { goto error; }
    if (!Contains(list, right)) { goto error; }
    
    // verifica daca nodul 'right' nu este la dreapta nodului 'left'
    const struct Node *node = GetNodePrev(left);
    
    while (node != GetHead(list)) {
        if (node == right) { goto error; }
        
        node = GetNodePrev(node);
    }
    
    struct List *sub = Create();
    
    node = left;
    
    while (node != GetNodeNext(right)) {
        struct Node *copy = CopyNode(node);
        
        AddLast(sub, copy);
        
        node = GetNodeNext(node);
    }
    
    return sub;
    
error:
    return NULL;
}

static struct List * GetSubCentered(const struct List *list, const struct Node *node, size_t radius) {
    // if (!list) { goto error; }
    if (!node) { goto error; }
    
    if (!Contains(list, node)) { goto error; }
    
    const struct Node *left = node;
    const struct Node *right = node;
    
    while (radius > 0) {
        left = GetNodePrev(left);
        right = GetNodeNext(right);
        
        if (left == GetHead(list)) { goto error; }
        if (right == GetTail(list)) { goto error; }
        
        --radius;
    }
    
    return GetSub(list, left, right);
    
error:
    return NULL;
}

static void Print(const struct List *list, print_func_t print_func) {
    if (!print_func) { goto error; }
    
    struct Node *node = GetFirst(list); // obtine primul nod din lista
    
    // cat timp nu am ajuns la nodul santinela de sfarsit
    while (node != GetTail(list)) {
        // apeleaza functia ce va procesa datele si le va printa
        print_func(GetNodeData(node));

        node = GetNodeNext(node);
    }
    
    return;
    
error:
    return;
}

static void Destroy(struct List *list) {
    if (!list) { goto error; }
    
    // sterge toate nodurile listei
    struct Node *node = GetHead(list);
    
    while (node) {
        struct Node *next = GetNodeNext(node);
        
        DestroyData(DestroyNode(node));
        
        node = next;
    }
    
    free(list); // elibereaza memoria ocupata de lista
    
    return;
    
error:
    return;
}

// PUBLIC

// LinkedListData functions

size_t LinkedListData_GetSize(const struct LinkedListData *linkedListData) {
    return GetDataSize((const struct Data *)linkedListData);
}

void LinkedListData_Destroy(struct LinkedListData *linkedListData) {
    DestroyData((struct Data *)linkedListData);
}

// LinkedListNode functions

struct LinkedListData * LinkedListNode_GetData(const struct LinkedListNode *linkedListNode) {
    return (struct LinkedListData *)GetNodeData((const struct Node *)linkedListNode);
}

struct LinkedListNode * LinkedListNode_GetNext(const struct LinkedListNode *linkedListNode) {
    const struct Node *nextNode = GetNodeNext((const struct Node *)linkedListNode);
    
    // daca nodul urmator este nodul santinela de sfarsit atunci nu il returna
    if (!GetNodeNext(nextNode)) { return NULL; }
    
    return (struct LinkedListNode *)nextNode;
}

struct LinkedListNode * LinkedListNode_GetPrev(const struct LinkedListNode *linkedListNode) {
    const struct Node *prevNode = GetNodePrev((const struct Node *)linkedListNode);
    
    // daca nodul precedent este nodul santinela de inceput atunci nu il returna
    if (!GetNodePrev(prevNode)) { return NULL; }
    
    return (struct LinkedListNode *)prevNode;
}

// LinkedList functions

struct LinkedList * LinkedList_Create() {
    return (struct LinkedList *)Create();
}

size_t LinkedList_GetSize(const struct LinkedList *linkedList) {
    return GetSize((const struct List *)linkedList);
}

bool LinkedList_IsEmpty(const struct LinkedList *linkedList) {
    return IsEmpty((const struct List *)linkedList);
}

struct LinkedListNode * LinkedList_GetFirst(const struct LinkedList *linkedList) {
    const struct List *list = (const struct List *)linkedList;
    
    // daca lista nu are niciun element returneaza NULL in loc
    // de nodul santinela de sfarsit al listei
    return GetSize(list) > 0 ? (struct LinkedListNode *)GetFirst(list) : NULL;
}

struct LinkedListNode * LinkedList_GetLast(const struct LinkedList *linkedList) {
    const struct List *list = (const struct List *)linkedList;
    
    // daca lista nu are niciun element returneaza NULL in loc
    // de nodul santinela de inceput al listei
    return GetSize(list) > 0 ? (struct LinkedListNode *)GetLast(list) : NULL;
}

struct LinkedListNode * LinkedList_Contains(
    const struct LinkedList *linkedList,
    const struct LinkedListNode *linkedListNode
) {
    const struct List *list = (const struct List *)linkedList;
    struct Node *node = Contains((const struct List *)linkedList, (const struct Node *)linkedListNode);
    
    // nu returna santinelele
    if (node == GetHead(list)) { return NULL; }
    if (node == GetTail(list)) { return NULL; }
    
    return (struct LinkedListNode *)node;
}

struct LinkedListNode * LinkedList_ContainsData(
    const struct LinkedList *linkedList,
    const struct LinkedListData *linkedListData,
    LinkedList_comp_func_t LinkedList_comp_func
) {
    return (struct LinkedListNode *)ContainsData(
        (const struct List *)linkedList,
        (const struct Data *)linkedListData,
        (comp_func_t)LinkedList_comp_func
    );
}

struct LinkedListNode * LinkedList_AddBetween(
    struct LinkedList *linkedList,
    const struct LinkedListNode *linkedListNode,
    struct LinkedListNode *linkedListLeft,
    struct LinkedListNode *linkedListRight
) {
    struct Node *node = CopyNode((const struct Node *)linkedListNode);
    
    return (struct LinkedListNode *)AddBetween(
        (struct List *)linkedList,
        node,
        (struct Node *)linkedListLeft,
        (struct Node *)linkedListRight
    );
}

struct LinkedListNode * LinkedList_AddAfter(
    struct LinkedList *linkedList,
    const struct LinkedListNode *linkedListNode,
    struct LinkedListNode *linkedListRef
) {
    struct Node *node = CopyNode((const struct Node *)linkedListNode);
    
    return (struct LinkedListNode *)AddAfter(
        (struct List *)linkedList,
        node,
        (struct Node *)linkedListRef
    );
}

struct LinkedListNode * LinkedList_AddBefore(
    struct LinkedList *linkedList,
    const struct LinkedListNode *linkedListNode,
    struct LinkedListNode *linkedListRef
) {
    struct Node *node = CopyNode((const struct Node *)linkedListNode);
    
    return (struct LinkedListNode *)AddBefore(
        (struct List *)linkedList,
        node,
        (struct Node *)linkedListRef
    );
}

struct LinkedListNode * LinkedList_AddFirst(
    struct LinkedList *linkedList,
    const struct LinkedListNode *linkedListNode
) {
    struct Node *node = CopyNode((const struct Node *)linkedListNode);
    
    return (struct LinkedListNode *)AddFirst((struct List *)linkedList, node);
}

struct LinkedListNode * LinkedList_AddLast(
    struct LinkedList *linkedList,
    const struct LinkedListNode *linkedListNode
) {
    struct Node *node = CopyNode((const struct Node *)linkedListNode);
    
    return (struct LinkedListNode *)AddLast((struct List *)linkedList, node);
}

struct LinkedListNode * LinkedList_AddDataBetween(
    struct LinkedList *linkedList,
    const struct LinkedListData *linkedListData,
    struct LinkedListNode *linkedListLeft,
    struct LinkedListNode *linkedListRight
) {
    return (struct LinkedListNode *)AddDataBetween(
        (struct List *)linkedList,
        (const struct Data *)linkedListData,
        (struct Node *)linkedListLeft,
        (struct Node *)linkedListRight
    );
}

struct LinkedListNode * LinkedList_AddDataAfter(
    struct LinkedList *linkedList,
    const struct LinkedListData *linkedListData,
    struct LinkedListNode *linkedListRef
) {
    return (struct LinkedListNode *)AddDataAfter(
        (struct List *)linkedList,
        (const struct Data *)linkedListData,
        (struct Node *)linkedListRef
    );
}

struct LinkedListNode * LinkedList_AddDataBefore(
    struct LinkedList *linkedList,
    const struct LinkedListData *linkedListData,
    struct LinkedListNode *linkedListRef
) {
    return (struct LinkedListNode *)AddDataBefore(
        (struct List *)linkedList,
        (const struct Data *)linkedListData,
        (struct Node *)linkedListRef
    );
}

struct LinkedListNode * LinkedList_AddDataFirst(
    struct LinkedList *linkedList,
    const struct LinkedListData *linkedListData
) {
    return (struct LinkedListNode *)AddDataFirst(
        (struct List *)linkedList,
        (const struct Data *)linkedListData
    );
}

struct LinkedListNode * LinkedList_AddDataLast(
    struct LinkedList *linkedList,
    const struct LinkedListData *linkedListData
) {
    return (struct LinkedListNode *)AddDataLast(
        (struct List *)linkedList,
        (const struct Data *)linkedListData
    );
}

struct LinkedListData * LinkedList_Remove(
    struct LinkedList *linkedList,
    struct LinkedListNode *linkedListNode,
    bool pop
) {
    return (struct LinkedListData *)Remove((struct List *)linkedList, (struct Node *)linkedListNode, pop);
}

struct LinkedListData * LinkedList_RemoveFirst(struct LinkedList *linkedList, bool pop) {
    return (struct LinkedListData *)RemoveFirst((struct List *)linkedList, pop);
}

struct LinkedListData * LinkedList_RemoveLast(struct LinkedList *linkedList, bool pop) {
    return (struct LinkedListData *)RemoveLast((struct List *)linkedList, pop);
}

struct LinkedListData * LinkedList_RemoveData(
    struct LinkedList *linkedList,
    const struct LinkedListData *linkedListData,
    LinkedList_comp_func_t LinkedList_comp_func,
    bool pop
) {
    return (struct LinkedListData *)RemoveData(
        (struct List *)linkedList,
        (const struct Data *)linkedListData,
        (comp_func_t)LinkedList_comp_func,
        pop
    );
}

struct LinkedList * LinkedList_GetSub(
    const struct LinkedList *linkedList,
    const struct LinkedListNode *linkedListLeft,
    // TODO: remove linkedList prefix from variables and
    // use private prefix for private variables where necessary
    const struct LinkedListNode *linkedListRight
) {
    return (struct LinkedList *)GetSub(
        (const struct List *)linkedList,
        (const struct Node *)linkedListLeft,
        (const struct Node *)linkedListRight
    );
}

struct LinkedList * LinkedList_GetSubCentered(
    const struct LinkedList *linkedList,
    const struct LinkedListNode *linkedListNode,
    size_t radius
) {
    return (struct LinkedList *)GetSubCentered(
        (const struct List *)linkedList,
        (const struct Node *)linkedListNode,
        radius
    );
}

void LinkedList_Print(
    const struct LinkedList *linkedList,
    LinkedList_print_func_t LinkedList_print_func
) {
    Print((const struct List *)linkedList, (print_func_t)LinkedList_print_func);
}

void LinkedList_Destroy(struct LinkedList *linkedList) {
    Destroy((struct List *)linkedList);
}
