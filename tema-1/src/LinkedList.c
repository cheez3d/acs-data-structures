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
    void *data;
    size_t size;
    
    // struct Data *data;
    
    struct Node *next;
    struct Node *prev;
};

struct List {
    size_t size;
    
    struct Node *head;
    struct Node *tail;
};

typedef int (*comp_func_t)(const void *first, const void *second);
typedef void (*print_func_t)(const void *data);
// typedef void (*process_func_t)(struct Node *node); // TODO: investigate this

// PRIVATE

// Node functions

static void * CreateData(const void *data, size_t size) {
    if (data == NULL) { return NULL; }
    
    return memcpy(malloc(size), data, size);
}

static void DestroyData(void *data) {
    if (data == NULL) { return; }
    
    free(data);
}

static struct Node * CreateNode(const void *data, size_t size) {
    // creeaza nodul de adaugat
    struct Node *node = malloc(sizeof *node);
    
    node->data = CreateData(data, size); // copiaza datele in nod
    
    // initializeaza legaturile nodului
    node->next = NULL;
    node->prev = NULL;
    
    return node; // returneaza nodul
}

static void * GetData(const struct Node *node) {
    if (node == NULL) { return NULL; }
    
    return node->data;
}

static struct Node * GetNext(const struct Node *node) {
    if (node == NULL) { return NULL; }
    
    return node->next;
}

static struct Node * GetPrev(const struct Node *node) {
    if (node == NULL) { return NULL; }
    
    return node->prev;
}

static void * DestroyNode(struct Node *node) {
    if (node == NULL) { return NULL; }
    
    void *data = node->data; // DestroyData(node->data); // distruge datele nodului
    
    free(node); // elibereaza memoria ocupata de nod
    
    return data;
}

// List functions

static struct List * Create() {
    // creeaza lista
    struct List *list = malloc(sizeof *list);
    
    // creeaza nodurile santinela de inceput si de sfarsit al listei
    struct Node *head = CreateNode(NULL, 0);
    struct Node *tail = CreateNode(NULL, 0);
    
    // leaga cele doua noduri santinela intre ele
    head->next = tail;
    tail->prev = head;
    
    // initializeaza lista
    list->size = 0;
    
    list->head = head;
    list->tail = tail;
    
    return list; // returneaza lista
}

static size_t GetSize(const struct List *list) {
    if (list == NULL) { return 0; }
    
    return list->size;
}

static bool IsEmpty(const struct List *list) {
    if (list == NULL) { return true; }
    
    return GetSize(list) == 0;
}

static struct Node * GetHead(const struct List *list) {
    if (list == NULL) { return NULL; }
    
    return list->head;
}

static struct Node * GetTail(const struct List *list) {
    if (list == NULL) { return NULL; }
    
    return list->tail;
}

static struct Node * GetBegin(const struct List *list) {
    return GetNext(GetHead(list));
}

static struct Node * GetEnd(const struct List *list) {
    return GetPrev(GetTail(list));
}

static struct Node * ContainsNode(const struct List *list, const struct Node *node) {
    if (node == GetHead(list)) { return GetHead(list); }
    if (node == GetTail(list)) { return GetTail(list); }
    
    // incepe cautarea simultan atat de la inceputul, cat si de la sfarsitul listei
    struct Node *left = GetBegin(list);
    struct Node *right = GetEnd(list);
    
    // cat timp cele doua directii de cautare nu s-au intalnit
    // (se verifica intalnirea atat pentru cazul cand lista are un numar
    //  impar de elemente, cat si pentru cazul cand lista are un numar
    //  par de elemente)
    while (!(GetPrev(left) == GetNext(right)) && !(GetPrev(left) == right && GetNext(right) == left)) {
        // verifica daca nodul a fost gasit
        if (left == node) { return left; }
        if (right == node) { return right; }
        
        // avanseaza cautarea
        left = GetNext(left);
        right = GetPrev(right);
    }
    
    return NULL;
}

static struct Node * ContainsData(const struct List *list, const void *data, comp_func_t comp_func) {
    if (comp_func == NULL) { return NULL; }
    
    // incepe cautarea de la inceputul listei
    struct Node *node = GetBegin(list);
    
    while (node != GetTail(list)) {
        // verifica daca a fost gasit un nod ale carui date corespund
        if (comp_func(GetData(node), data) == 0) { return node; }
        
        // avanseaza cautarea
        node = GetNext(node);
    }
    
    return NULL;
}

static struct Node * AddNodeBetween(struct List *list, /*TODO: const*/ struct Node *node, struct Node *left, struct Node *right) {
    if (list == NULL) { return NULL; }
    if (node == NULL) { return NULL; }
    if (left == NULL) { return NULL; }
    if (right == NULL) { return NULL; }
    
    // verifica daca nodurile intre care se insereaza se afla in lista
    if ((ContainsNode(list, left) == NULL) || (ContainsNode(list, right) == NULL)) { return NULL; }
    
    // verifica daca se incearca inserarea inainte de nodul santinela de inceput
    // sau dupa nodul santinela de sfarsit
    if ((right == GetHead(list)) || (left == GetTail(list))) { return NULL; }
    
    // verifica daca nodurile intre care se insereaza se succed
    if ((GetNext(left) != right) || (GetPrev(right) != left)) { return NULL; }
    
    // TODO: continue from here
    // struct *inserted = CreateNode(GetData(node), ); // TODO: add size member to Node struct
    
    // configureaza legaturile nodului
    node->prev = left;
    node->next = right;
    
    // insereaza nodul in lista
    left->next = node;
    right->prev = node;
    
    ++(list->size); // incrementeaza numarul de noduri din lista
    
    return node; // returneaza nodul inserat
}

static struct Node * AddNodeAfter(struct List *list, struct Node *node, struct Node *ref) {
    return AddNodeBetween(list, node, ref, GetNext(ref));
}

static struct Node * AddNodeBefore(struct List *list, struct Node *node, struct Node *ref) {
    return AddNodeBetween(list, node, GetPrev(ref), ref);
}

static struct Node * AddNodeBegin(struct List *list, struct Node *node) {
    return AddNodeAfter(list, node, GetHead(list));
}

static struct Node * AddNodeEnd(struct List *list, struct Node *node) {
    return AddNodeBefore(list, node, GetTail(list));
}

static struct Node * AddDataBetween(struct List *list, const void *data, size_t size, struct Node *left, struct Node *right) {
    struct Node *node = CreateNode(data, size);
    
    if (AddNodeBetween(list, node, left, right) == NULL) {
        DestroyNode(node);
        
        return NULL;
    }
    
    return node;
}

static struct Node * AddDataAfter(struct List *list, const void *data, size_t size, struct Node *ref) {
    return AddDataBetween(list, data, size, ref, GetNext(ref));
}

static struct Node * AddDataBefore(struct List *list, const void *data, size_t size, struct Node *ref) {
    return AddDataBetween(list, data, size, GetPrev(ref), ref);
}

static struct Node * AddDataBegin(struct List *list, const void *data, size_t size) {
    return AddDataAfter(list, data, size, GetHead(list));
}

static struct Node * AddDataEnd(struct List *list, const void *data, size_t size) {
    return AddDataBefore(list, data, size, GetTail(list));
}

static void * RemoveNode(struct List *list, struct Node *node) {
    if (list == NULL) { return NULL; }
    if (node == NULL) { return NULL; }
    
    if (GetSize(list) == 0) { return NULL; } // verifica daca lista este goala
    
    // verifica daca se incearca eliminarea vreunui nod santinela
    if (node == GetHead(list)) { return NULL; }
    if (node == GetTail(list)) { return NULL; }
    
    // verifica daca nodul se afla in lista
    if (ContainsNode(list, node) == NULL) { return NULL; }
    
    // inlatura nodul din lista
    if (GetNext(node) != NULL) { GetNext(node)->prev = GetPrev(node); }
    if (GetPrev(node) != NULL) { GetPrev(node)->next = GetNext(node); }
    
    --(list->size); // decrementeaza numarul de noduri din lista
    
    return DestroyNode(node); // distruge nodul
}

static void * RemoveBegin(struct List *list) {
    return RemoveNode(list, GetBegin(list));
}

static void * RemoveEnd(struct List *list) {
    return RemoveNode(list, GetEnd(list));
}

static void * RemoveData(struct List *list, const void *data, comp_func_t comp_func) {
    // cauta un nod care sa corespunda cu datele specificate
    struct Node *node = ContainsData(list, data, comp_func);
    
    if (node == NULL) { return NULL; } // verifica daca nodul nu a fost gasit
    
    return RemoveNode(list, node); // sterge nodul
}

static void Print(const struct List *list, print_func_t print_func) {
    if (print_func == NULL) { return; }
    
    struct Node *node = GetBegin(list); // obtine primul nod din lista
    
    // cat timp nu am ajuns la nodul santinela de sfarsit
    while (node != GetTail(list)) {
        // apeleaza functia ce va procesa datele si le va printa
        print_func(GetData(node));

        node = GetNext(node);
    }
}

static void Destroy(struct List *list) {
    if (list == NULL) { return; }
    
    // sterge toate nodurile listei
    struct Node *node = GetHead(list);
    
    while (node != NULL) {
        struct Node *next = GetNext(node);
        
        DestroyNode(node);
        
        node = next;
    }
    
    free(list); // elibereaza memoria ocupata de lista
}

// PUBLIC

// LinkedListNode functions

void * LinkedListNode_CreateData(const void *data, size_t size) {
    return CreateData(data, size);
}

void LinkedListNode_DestroyData(void *data) {
    DestroyData(data);
}

struct LinkedListNode * LinkedListNode_Create(const void *data, size_t size) {
    return (struct LinkedListNode *)CreateNode(data, size);
}

void * LinkedListNode_GetData(const struct LinkedListNode *linkedListNode) {
    return GetData((const struct Node *)linkedListNode);
}

struct LinkedListNode * LinkedListNode_GetNext(const struct LinkedListNode *linkedListNode) {
    return (struct LinkedListNode *)GetNext((const struct Node *)linkedListNode);
}

struct LinkedListNode * LinkedListNode_GetPrev(const struct LinkedListNode *linkedListNode) {
    return (struct LinkedListNode *)GetPrev((const struct Node *)linkedListNode);
}

void LinkedListNode_Destroy(struct LinkedListNode *linkedListNode) {
    DestroyNode((struct Node *)linkedListNode);
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

struct LinkedListNode * LinkedList_GetBegin(const struct LinkedList *linkedList) {
    const struct List *list = (const struct List *)linkedList;
    
    // daca lista nu are niciun element returneaza NULL in loc
    // de nodul santinela de sfarsit al listei
    return GetSize(list) > 0 ? (struct LinkedListNode *)GetBegin(list) : NULL;
}

struct LinkedListNode * LinkedList_GetEnd(const struct LinkedList *linkedList) {
    const struct List *list = (const struct List *)linkedList;
    
    // daca lista nu are niciun element returneaza NULL in loc
    // de nodul santinela de inceput al listei
    return GetSize(list) > 0 ? (struct LinkedListNode *)GetEnd(list) : NULL;
}

struct LinkedListNode * LinkedList_ContainsNode(
    const struct LinkedList *linkedList,
    const struct LinkedListNode *linkedListNode
) {
    const struct List *list = (const struct List *)linkedList;
    struct Node *node = ContainsNode((const struct List *)linkedList, (const struct Node *)linkedListNode);
    
    // nu returna santinelele
    if (node == GetHead(list)) { return NULL; }
    if (node == GetTail(list)) { return NULL; }
    
    return (struct LinkedListNode *)node;
}

struct LinkedListNode * LinkedList_ContainsData(
    const struct LinkedList *linkedList,
    const void *data,
    LinkedList_comp_func_t LinkedList_comp_func
) {
    return (struct LinkedListNode *)ContainsData(
        (const struct List *)linkedList,
        data,
        (comp_func_t)LinkedList_comp_func
    );
}

struct LinkedListNode * LinkedList_AddNodeBetween(
    struct LinkedList *linkedList,
    struct LinkedListNode *linkedListNode,
    struct LinkedListNode *linkedListLeft,
    struct LinkedListNode *linkedListRight
) {
    return (struct LinkedListNode *)AddNodeBetween(
        (struct List *)linkedList,
        (struct Node *)linkedListNode,
        (struct Node *)linkedListLeft,
        (struct Node *)linkedListRight
    );
}

struct LinkedListNode * LinkedList_AddNodeAfter(
    struct LinkedList *linkedList,
    struct LinkedListNode *linkedListNode,
    struct LinkedListNode *linkedListRef
) {
    return (struct LinkedListNode *)AddNodeAfter(
        (struct List *)linkedList,
        (struct Node *)linkedListNode,
        (struct Node *)linkedListRef
    );
}

struct LinkedListNode * LinkedList_AddNodeBefore(
    struct LinkedList *linkedList,
    struct LinkedListNode *linkedListNode,
    struct LinkedListNode *linkedListRef
) {
    return (struct LinkedListNode *)AddNodeBefore(
        (struct List *)linkedList,
        (struct Node *)linkedListNode,
        (struct Node *)linkedListRef
    );
}

struct LinkedListNode * LinkedList_AddNodeBegin(
    struct LinkedList *linkedList,
    struct LinkedListNode *linkedListNode
) {
    return (struct LinkedListNode *)AddNodeBegin(
        (struct List *)linkedList,
        (struct Node *)linkedListNode
    );
}

struct LinkedListNode * LinkedList_AddNodeEnd(
    struct LinkedList *linkedList,
    struct LinkedListNode *linkedListNode
) {
    return (struct LinkedListNode *)AddNodeEnd(
        (struct List *)linkedList,
        (struct Node *)linkedListNode
    );
}

struct LinkedListNode * LinkedList_AddDataBetween(
    struct LinkedList *linkedList,
    const void *data, size_t size,
    struct LinkedListNode *linkedListLeft,
    struct LinkedListNode *linkedListRight
) {
    return (struct LinkedListNode *)AddDataBetween(
        (struct List *)linkedList,
        data, size,
        (struct Node *)linkedListLeft,
        (struct Node *)linkedListRight
    );
}

struct LinkedListNode * LinkedList_AddDataAfter(
    struct LinkedList *linkedList,
    const void *data, size_t size,
    struct LinkedListNode *linkedListRef
) {
    return (struct LinkedListNode *)AddDataAfter(
        (struct List *)linkedList,
        data, size,
        (struct Node *)linkedListRef
    );
}

struct LinkedListNode * LinkedList_AddDataBefore(
    struct LinkedList *linkedList,
    const void *data, size_t size,
    struct LinkedListNode *linkedListRef
) {
    return (struct LinkedListNode *)AddDataBefore(
        (struct List *)linkedList,
        data, size,
        (struct Node *)linkedListRef
    );
}

struct LinkedListNode * LinkedList_AddDataBegin(
    struct LinkedList *linkedList,
    const void *data, size_t size
) {
    return (struct LinkedListNode *)AddDataBegin((struct List *)linkedList, data, size);
}

struct LinkedListNode * LinkedList_AddDataEnd(
    struct LinkedList *linkedList,
    const void *data, size_t size
) {
    return (struct LinkedListNode *)AddDataEnd((struct List *)linkedList, data, size);
}

void * LinkedList_RemoveNode(
    struct LinkedList *linkedList,
    struct LinkedListNode *linkedListNode
) {
    return RemoveNode((struct List *)linkedList, (struct Node *)linkedListNode);
}

void * LinkedList_RemoveBegin(struct LinkedList *linkedList) {
    return RemoveBegin((struct List *)linkedList);
}

void * LinkedList_RemoveEnd(struct LinkedList *linkedList) {
    return RemoveEnd((struct List *)linkedList);
}

void * LinkedList_RemoveData(
    struct LinkedList *linkedList,
    const void *data,
    LinkedList_comp_func_t LinkedList_comp_func
) {
    return RemoveData((struct List *)linkedList, data, (comp_func_t)LinkedList_comp_func);
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
