#include "List.h"

#include "Data.h"

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

struct ListNode {
    struct Data *data;
    
    struct ListNode *next;
    struct ListNode *prev;
    
    struct List *list;
};

struct List {
    size_t size;
    
    struct ListNode *head;
    struct ListNode *tail;
};

// PRIVATE // TODO: add setters

static struct ListNode * CreateNode(const struct Data *data) {
    struct ListNode *node = malloc(sizeof *node);
    
    node->data = Data_Copy(data); // copiaza datele in nod
    
    // initializeaza legaturile nodului
    node->next = NULL;
    node->prev = NULL;
    
    return node;
}

static struct ListNode * CopyNode(const struct ListNode *node) {
    if (!node) { goto error; }
    
    return CreateNode(node->data);
    
error:
    return NULL;
}

static struct List * Create() {
    // creeaza lista
    struct List *list = malloc(sizeof *list);
    
    // creeaza nodurile santinela ale listei
    struct ListNode *head = CreateNode(NULL);
    struct ListNode *tail = CreateNode(NULL);
    
    // leaga cele doua noduri santinela intre ele
    head->next = tail;
    tail->prev = head;
    
    // seteaza lista caruia nodurile santinela apratin
    head->list = list;
    tail->list = list;
    
    // initializeaza lista
    
    list->size = 0;
    
    list->head = head;
    list->tail = tail;
    
    return list;
}

static struct Data * GetNodeData(const struct ListNode *node) {
    if (!node) { goto error; }
    
    return node->data;
    
error:
    return NULL;
}

static struct ListNode * GetNodeNext(const struct ListNode *node) {
    if (!node) { goto error; }
    
    return node->next;
    
error:
    return NULL;
}

static struct ListNode * GetNodePrev(const struct ListNode *node) {
    if (!node) { goto error; }
    
    return node->prev;
    
error:
    return NULL;
}

static struct List * GetNodeList(const struct ListNode *node) {
    if (!node) { goto error; }
    
    return node->list;
    
error:
    return NULL;
}

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

static struct ListNode * GetHeadNode(const struct List *list) {
    if (!list) { goto error; }
    
    return list->head;
    
error:
    return NULL;
}

static struct ListNode * GetTailNode(const struct List *list) {
    if (!list) { goto error; }
    
    return list->tail;
    
error:
    return NULL;
}

static bool IsNodeHeadOrTail(const struct ListNode *node) {
    const struct List *list = GetNodeList(node);
    
    if (!list) { goto error; }
    
    return (node == GetHeadNode(list)) || (node == GetTailNode(list));
    
error:
    return false;
}

static bool IsNodeAfter(const struct ListNode *node,
                        const struct ListNode *ref) {
    if (!node || !ref) { goto error; }
    
    const struct List *nodeList = GetNodeList(node);
    const struct List *refList = GetNodeList(ref);
    
    if (!nodeList || !refList) { goto error; }
    if (nodeList != refList) { goto error; }
    
    return GetNodeNext(ref) == node;
    
error:
    return false;
}

static bool IsNodeBefore(const struct ListNode *node,
                         const struct ListNode *ref) {
    if (!node || !ref) { goto error; }
    
    const struct List *nodeList = GetNodeList(node);
    const struct List *refList = GetNodeList(ref);
    
    if (!nodeList || !refList) { goto error; }
    if (nodeList != refList) { goto error; }
    
    return GetNodePrev(ref) == node;
    
error:
    return false;
}

static bool IsNodeBetween(const struct ListNode *node,
                          const struct ListNode *left,
                          const struct ListNode *right) {
    if (!node || !left || !right) { goto error; }
    
    return IsNodeAfter(node, left) && IsNodeBefore(node, right);
    
error:
    return false;
}

static bool IsNodeSuccessor(const struct ListNode *node,
                            const struct ListNode *ref) {
    if (!node || !ref) { goto error; }
    
    const struct List *nodeList = GetNodeList(node);
    const struct List *refList = GetNodeList(ref);
    
    if (!nodeList || !refList) { goto error; }
    if (nodeList != refList) { goto error; }
    
    if (node == ref) { return false; }
    if (node == GetTailNode(nodeList)) { return true; }
    if (ref == GetHeadNode(nodeList)) { return true; }
    
    while ((node = GetNodePrev(node)) != GetHeadNode(nodeList)) {
        if (node == ref) { return true; }
    }
    
    return false;
    
error:
    return false;
}

static bool IsNodePredecessor(const struct ListNode *node,
                              const struct ListNode *ref) {
    if (!node || !ref) { goto error; }
    
    const struct List *nodeList = GetNodeList(node);
    const struct List *refList = GetNodeList(ref);
    
    if (!nodeList || !refList) { goto error; }
    if (nodeList != refList) { goto error; }
    
    if (node == ref) { return false; }
    if (node == GetHeadNode(nodeList)) { return true; }
    if (ref == GetTailNode(nodeList)) { return true; }
    
    while ((node = GetNodeNext(node)) != GetTailNode(nodeList)) {
        if (node == ref) { return true; }
    }
    
    return false;
    
error:
    return false;
}

static bool IsNodeBounded(const struct ListNode *node,
                          const struct ListNode *left,
                          const struct ListNode *right) {
    if (!node || !left || !right) { goto error; }
    
    const struct List *nodeList = GetNodeList(node);
    const struct List *leftList = GetNodeList(left);
    const struct List *rightList = GetNodeList(right);
    
    if (!nodeList || !leftList || !rightList) { goto error; }
    if (nodeList != leftList) { goto error; }
    if (nodeList != rightList) { goto error; }
    if (leftList != rightList) { goto error; }
    
    return (IsNodeSuccessor(node, left) && IsNodePredecessor(node, right)) ||
           (IsNodeSuccessor(node, right) && IsNodePredecessor(node, left));
    
error:
    return false;
}

static bool AreNodesNeighbors(const struct ListNode *left,
                              const struct ListNode *right) {
    const struct List *leftList = GetNodeList(left);
    const struct List *rightList = GetNodeList(right);
    
    if (!leftList) { goto error; }
    if (!rightList) { goto error; }
    if (leftList != rightList) { goto error; }
    
    return ((GetNodeNext(left) == right) && (GetNodePrev(right) == left)) ||
           ((GetNodeNext(right) == left) && (GetNodePrev(left) == right));
    
error:
    return false;
}

static struct ListNode * GetFirstNode(const struct List *list) {
    return GetNodeNext(GetHeadNode(list));
}

static struct ListNode * GetLastNode(const struct List *list) {
    return GetNodePrev(GetTailNode(list));
}

static bool ContainsNode(const struct List *list,
                         const struct ListNode *node) {
    if (!list) { goto error; }
    if (!node) { goto error; }
    
    return GetNodeList(node) == list;
    
error:
    return NULL;
}

static struct ListNode * ContainsData(const struct List *list,
                                      const struct Data *data,
                                      List_comp_func_t comp_func) {
    if (!comp_func) { goto error; }
    
    struct ListNode *curr = GetFirstNode(list);
    
    while (curr != GetTailNode(list)) {
        // verifica daca a fost gasit un nod
        // ale carui date corespund cu cele cautate
        if (comp_func(GetNodeData(curr), data) == 0) { return curr; }
        
        curr = GetNodeNext(curr);
    }
    
    return NULL;
    
error:
    return NULL;
}

static struct ListNode * GetNodesMiddle(const struct List *list,
                                        const struct ListNode *left,
                                        const struct ListNode *right) {
    if (!list) { goto error; }
    if (!left) { goto error; }
    if (!right) { goto error; }
    
    if (!ContainsNode(list, left)) { goto error; }
    if (!ContainsNode(list, right)) { goto error; }
    
    if (left == right) { return (struct ListNode *)left; }
    
    // verifica daca cele doua noduri de delimitare
    // nu sunt in ordinea potrivita
    if (!IsNodePredecessor(left, right)) {
        const struct ListNode *temp = left;
        left = right;
        right = temp;
    }
    
    // parcurgem lista atat nod cu nod, cat si din 2 in 2 noduri
    
    const struct ListNode *doubleStep = left;
    const struct ListNode *step = left;
    
    while (GetNodeNext(doubleStep) &&
           (GetNodeNext(doubleStep) != GetNodeNext(right)) &&
           GetNodeNext(GetNodeNext(doubleStep)) &&
           (GetNodeNext(GetNodeNext(doubleStep)) != GetNodeNext(right))) {
        doubleStep = GetNodeNext(GetNodeNext(doubleStep));
        
        step = GetNodeNext(step);
    }
    
    return (struct ListNode *)step;
    
error:
    return NULL;
}

static struct ListNode * GetMiddleNode(const struct List *list) {
    if (!list) { goto error; }
    
    return GetNodesMiddle(list, GetHeadNode(list), GetTailNode(list));
    
error:
    return NULL;
}

static struct Data * DestroyNode(struct ListNode *node) {
    if (!node) { goto error; }
    
    // verifica daca se incearca distrugerea
    // unui nod care apartine unei liste
    if (GetNodeList(node)) { goto error; }
    
    struct Data *data = node->data;
    
    free(node); // elibereaza memoria ocupata de nod
    
    return data;
    
error:
    return NULL;
}

static void GetSpanNode(const struct List *list,
                         const struct ListNode *start,
                         size_t span,
                         bool forward,
                         struct ListNode **out_end,
                         size_t *out_span) {
    if (!list) { goto error; }
    if (!start) { goto error; }
    
    if (!out_end) { goto error; }
    
    if (!ContainsNode(list, start)) { goto error; }
    if (GetSize(list) == 0) { goto error; }
    if (IsNodeHeadOrTail(start)) { goto error; }
    
    struct ListNode *bound = forward ?
                             GetLastNode(list) :
                             GetFirstNode(list);
    
    struct ListNode * (*step_func)(const struct ListNode *) = forward ?
                                                              GetNodeNext :
                                                              GetNodePrev;
    
    const struct ListNode *curr = start;
    size_t currSpan = 0;
    
    while (currSpan < span) {
        if (curr == bound) { break; }
        
        curr = step_func(curr);
        
        ++currSpan;
    }
    
    *out_end = (struct ListNode *)curr;
    if (out_span) { *out_span = currSpan; }
    
    return;
    
error:
    *out_end = NULL;
    if (out_span) { *out_span = 0; }
    
    return;
}

static void GetRadiusNodes(const struct List *list,
                           const struct ListNode *center,
                           size_t radius,
                           struct ListNode **out_left,
                           struct ListNode **out_right,
                           size_t *out_leftRadius,
                           size_t *out_rightRadius) {
    // TODO: add proper error system instead of relying on functions
    // returning NULL pointers or values indicating error depending on
    // function return type
    
    if (!list || !center) { goto error; }
    if (!out_left || !out_right) { goto error; }
    
    if (!ContainsNode(list, center)) { goto error; }
    if (GetSize(list) == 0) { goto error; }
    if (IsNodeHeadOrTail(center)) { goto error; }
    
    const struct ListNode *currLeft = center;
    size_t leftRadius = 0;
    
    while (leftRadius < radius) {
        if (currLeft == GetFirstNode(list)) { break; }
        
        currLeft = GetNodePrev(currLeft);
        
        ++leftRadius;
    }
    
    const struct ListNode *currRight = center;
    size_t rightRadius = 0;
    
    while (rightRadius < radius) {
        if (currRight == GetLastNode(list)) { break; }
        
        currRight = GetNodeNext(currRight);
        
        ++rightRadius;
    }
    
    *out_left = (struct ListNode *)currLeft;
    *out_right = (struct ListNode *)currRight;
    if (out_leftRadius) { *out_leftRadius = leftRadius; }
    if (out_rightRadius) { *out_rightRadius = rightRadius; }
    
    return;
    
error:
    *out_left = NULL;
    *out_right = NULL;
    if (out_leftRadius) { *out_leftRadius = 0; }
    if (out_rightRadius) { *out_rightRadius = 0; }
    
    return;
}

static struct ListNode * AddNodeBetween(struct List *list,
                                        struct ListNode *node,
                                        struct ListNode *left,
                                        struct ListNode *right) {
    if (!list) { goto error; }
    if (!node) { goto error; }
    if (!left) { goto error; }
    if (!right) { goto error; }
    
    // verifica daca nodurile se afla in lista
    if (!ContainsNode(list, left)) { goto error; }
    if (!ContainsNode(list, right)) { goto error; }
    
    // verifica daca se incearca inserarea
    // inainte de nodul santinela de inceput
    // sau dupa nodul santinela de sfarsit
    if (right == GetHeadNode(list)) { goto error; }
    if (left == GetTailNode(list)) { goto error; }
    
    // verifica daca nodurile intre care se insereaza sunt vecine
    if (!AreNodesNeighbors(left, right)) { goto error; }
    
    // configureaza legaturile nodului
    node->prev = left;
    node->next = right;
    
    node->list = list; // seteaza lista caruia nodul apartine
    
    // insereaza nodul in lista
    left->next = node;
    right->prev = node;
    
    ++(list->size); // mareste numarul de noduri din lista
    
    return node; // returneaza nodul inserat
    
error:
    return NULL;
}

static struct ListNode * AddNodeAfter(struct List *list,
                                      struct ListNode *node,
                                      struct ListNode *ref) {
    return AddNodeBetween(list, node, ref, GetNodeNext(ref));
}

static struct ListNode * AddNodeBefore(struct List *list,
                                      struct ListNode *node,
                                      struct ListNode *ref) {
    return AddNodeBetween(list, node, GetNodePrev(ref), ref);
}

static struct ListNode * AddNodeFirst(struct List *list,
                                      struct ListNode *node) {
    return AddNodeAfter(list, node, GetHeadNode(list));
}

static struct ListNode * AddNodeLast(struct List *list,
                                     struct ListNode *node) {
    return AddNodeBefore(list, node, GetTailNode(list));
}

static struct ListNode * AddDataBetween(struct List *list,
                                        const struct Data *data,
                                        struct ListNode *left,
                                        struct ListNode *right) {
    struct ListNode *node = CreateNode(data);
    
    if (!AddNodeBetween(list, node, left, right)) { goto error; }
    
    return node;
    
error:
    DestroyNode(node);
    
    return NULL;
}

static struct ListNode * AddDataAfter(struct List *list,
                                      const struct Data *data,
                                      struct ListNode *ref) {
    return AddDataBetween(list, data, ref, GetNodeNext(ref));
}

static struct ListNode * AddDataBefore(struct List *list,
                                       const struct Data *data,
                                       struct ListNode *ref) {
    return AddDataBetween(list, data, GetNodePrev(ref), ref);
}

static struct ListNode * AddDataFirst(struct List *list,
                                      const struct Data *data) {
    return AddDataAfter(list, data, GetHeadNode(list));
}

static struct ListNode * AddDataLast(struct List *list,
                                     const struct Data *data) {
    return AddDataBefore(list, data, GetTailNode(list));
}

static struct List * CreateSubNodes(const struct List *list,
                                    const struct ListNode *left,
                                    const struct ListNode *right) {
    if (!list) { goto error; }
    if (!left) { goto error; }
    if (!right) { goto error; }
    
    // verifica daca nodurile se afla in lista
    if (!ContainsNode(list, left)) { goto error; }
    if (!ContainsNode(list, right)) { goto error; }
    
    // verifica daca cele doua noduri de delimitare
    // nu sunt in ordinea potrivita
    if (!IsNodePredecessor(left, right)) {
        const struct ListNode *temp = left;
        left = right;
        right = temp;
    }
    
    struct List *sub = Create();
    
    const struct ListNode *curr = left;
    
    while (curr != GetNodeNext(right)) {
        struct ListNode *copy = CopyNode(curr);
        
        AddNodeLast(sub, copy);
        
        curr = GetNodeNext(curr);
    }
    
    return sub;
    
error:
    return NULL;
}
#include <stdio.h>
static struct List * Copy(const struct List *list) {
    if (!list) { goto error; }
    
    struct List *copy = Create();
    
    const struct ListNode *curr = GetFirstNode(list);
    
    while (curr != GetTailNode(list)) {
        struct ListNode *nodeCopy = CopyNode(curr);
        
        AddNodeLast(copy, nodeCopy);
        
        curr = GetNodeNext(curr);
    }
    
    return copy;
    
error:
    return NULL;
}

static struct List * CreateSubSpan(const struct List *list,
                                   const struct ListNode *start,
                                   size_t span,
                                   bool forward) {
    if (!list) { goto error; }
    if (!start) { goto error; }
    
    if (!ContainsNode(list, start)) { goto error; }
    
    struct ListNode *end;
    
    GetSpanNode(list, start, span, forward, &end, NULL);
    
    if (!end) { goto error; }
    
    struct List *sub = CreateSubNodes(list, start, end);
    
    if (!sub) { goto error; }
    
    return sub;
    
error:
    return NULL;
}

static struct List * CreateSubRadius(const struct List *list,
                                     const struct ListNode *center,
                                     size_t radius) {
    if (!list) { goto error; }
    if (!center) { goto error; }
    
    if (!ContainsNode(list, center)) { goto error; }
    
    struct ListNode *left;
    struct ListNode *right;
    
    GetRadiusNodes(list, center, radius, &left, &right, NULL, NULL);
    
    if (!left) { goto error; }
    if (!right) { goto error; }
    
    struct List *sub = CreateSubNodes(list, left, right);
    
    if (!sub) { goto error; }
    
    return sub;
    
error:
    return NULL;
}

static void Print(const struct List *list, List_print_func_t print_func) {
    if (!print_func) { goto error; }
    
    if (GetSize(list) == 0) { return; }
    
    struct ListNode *curr = GetFirstNode(list);
    
    while (curr != GetTailNode(list)) {
        // apeleaza functia ce va procesa datele si le va printa
        print_func(GetNodeData(curr));

        curr = GetNodeNext(curr);
    }
    
    return;
    
error:
    return;
}

static void SwapNodes(struct List *list,
                      struct ListNode *first,
                      struct ListNode *second) {
    // verifica daca nodurile se afla in lista
    if (!ContainsNode(list, first)) { goto error; }
    if (!ContainsNode(list, second)) { goto error; }
    
    // verifica daca vreunul din noduri este nod santinela
    if (IsNodeHeadOrTail(first)) { goto error; }
    if (IsNodeHeadOrTail(second)) { goto error; }
    
    // https://stackoverflow.com/a/27041055
    
    if (first == second) { return; }
    
    bool areNeighbors = AreNodesNeighbors(first, second);
    
    if (areNeighbors && (GetNodeNext(second) == first)) {
        struct ListNode *temp = first;
        first = second;
        second = temp;
    }
    
    struct ListNode *swapper[4];
    
    swapper[0] = GetNodePrev(first);
    swapper[1] = GetNodePrev(second);
    swapper[2] = GetNodeNext(first);
    swapper[3] = GetNodeNext(second);
    
    if (areNeighbors) {
        first->prev = swapper[2];
        second->prev = swapper[0];
        first->next = swapper[3];
        second->next = swapper[1];
    } else {
        first->prev = swapper[1];
        second->prev = swapper[0];
        first->next = swapper[3];
        second->next = swapper[2];
    }
    
    first->prev->next = first;
    first->next->prev = first;
    
    second->prev->next = second;
    second->next->prev = second;
    
    return;
    
error:
    return;
}

static void SwapData(struct List *list,
                     struct ListNode *first,
                     struct ListNode *second) {
    // verifica daca nodurile se afla in lista
    if (!ContainsNode(list, first)) { goto error; }
    if (!ContainsNode(list, second)) { goto error; }
    
    // verifica daca vreunul din noduri este nod santinela
    if (IsNodeHeadOrTail(first)) { goto error; }
    if (IsNodeHeadOrTail(second)) { goto error; }
    
    struct Data *temp = first->data;
    first->data = second->data;
    second->data = temp;
    
    return;
    
error:
    return;
}

static struct ListNode * Partition(struct List *list,
                                   struct ListNode *left,
                                   struct ListNode *right,
                                   struct ListNode *pivot,
                                   List_comp_func_t comp_func) {
    if (!list) { goto error; }
    if (!left) { goto error; }
    if (!right) { goto error; }
    if (!pivot) { goto error; }
    
    // verifica daca nodurile se afla in lista
    if (!ContainsNode(list, left)) { goto error; }
    if (!ContainsNode(list, right)) { goto error; }
    if (!ContainsNode(list, pivot)) { goto error; }
    
    // verifica daca vreunul din noduri este nod santinela
    if (IsNodeHeadOrTail(left)) { goto error; }
    if (IsNodeHeadOrTail(right)) { goto error; }
    if (IsNodeHeadOrTail(pivot)) { goto error; }
    
    // verifica daca nodul pivot este vreun nod de delimitare
    // sau daca se afla intre nodurile de delimitare
    if (!((pivot == left) || (pivot == right)) &&
        !IsNodeBounded(pivot, left, right)) { goto error; }
    
    // verifica daca cele doua noduri de delimitare
    // nu sunt in ordinea potrivita
    if (!IsNodePredecessor(left, right)) {
        struct ListNode *temp = left;
        left = right;
        right = temp;
    }
    
    // alegem pivotul ca fiind cel mai din dreapta element
    // verifica daca pivotul nu este cel mai din dreapta element
    if (pivot != right) {
        // fa interschimbarea necesara pentru ca pivotul
        // dat ca parametru sa fie cel mai din dreapta nod
        SwapData(list, pivot, right);
        
        pivot = right;
    }
    
    // incepe cu o un nod mai in inapoi, respectiv mai in inainte
    struct ListNode *currLeft = GetNodePrev(left);
    struct ListNode *currRight = right;
    
    while (true) {
        // aici nu este nevoie sa verificam daca am ajuns
        // la capatul din dreapta pentru ca sigur vom
        // da peste pivot, lucru care ne va scoate din bucla
        while (comp_func(GetNodeData(currLeft = GetNodeNext(currLeft)),
                         GetNodeData(pivot)) < 0);
        
        while ((currRight != left) &&
               (comp_func(GetNodeData(currRight = GetNodePrev(currRight)),
                          GetNodeData(pivot)) > 0));
        
        // verifica daca cele doua directii s-au intersectat
        if ((currLeft == currRight) ||
            IsNodeAfter(currLeft, currRight)) { break; }
        
        SwapData(list, currLeft, currRight);
    }
    
    SwapData(list, currLeft, right);
    
    return currLeft;
    
error:
    return NULL;
}

static void quick_sort(struct List *list,
                       struct ListNode *left,
                       struct ListNode *right,
                       List_comp_func_t comp_func) {
    if ((left == right) || IsNodeAfter(left, right)) { return; }
    
    struct ListNode *pivot = Partition(list, left, right, right, comp_func);
    
    quick_sort(list, left, GetNodePrev(pivot), comp_func);
    // pivotul se afla pe pozitia potrivita
    quick_sort(list, GetNodeNext(pivot), right, comp_func);
}

static void Sort(struct List *list,
                 struct ListNode *left,
                 struct ListNode *right,
                 List_comp_func_t comp_func) {
    if (!list) { goto error; }
    if (!left) { goto error; }
    if (!right) { goto error; }
    
    // verifica daca nodurile se afla in lista
    if (!ContainsNode(list, left)) { goto error; }
    if (!ContainsNode(list, right)) { goto error; }
    
    // verifica daca cele doua noduri de delimitare
    // sunt in ordinea potrivita
    if (!IsNodePredecessor(left, right)) {
        struct ListNode *temp = left;
        left = right;
        right = temp;
    }
    
    // foloseste algoritmul quicksort pentru sortarea listei
    quick_sort(list, left, right, comp_func);
    
    return;
    
error:
    return;
}

static struct Data * RemoveNode(struct List *list,
                                struct ListNode *node,
                                bool pop) {
    if (!list) { goto error; }
    if (!node) { goto error; }
    
    if (!ContainsNode(list, node)) { goto error; }
    
    // verifica daca lista este goala
    if (GetSize(list) == 0) { goto error; }
    
    // verifica daca se incearca eliminarea vreunui nod santinela
    if (node == GetHeadNode(list)) { goto error; }
    if (node == GetTailNode(list)) { goto error; }
    
    // inlatura nodul din lista
    GetNodeNext(node)->prev = GetNodePrev(node);
    GetNodePrev(node)->next = GetNodeNext(node);
    
    --(list->size); // scade numarul de noduri din lista
    
    node->list = NULL;
    
    struct Data *data = DestroyNode(node);
    
    // returneaza sau distruge datele ramase
    return pop ? data : (Data_Destroy(data), NULL);
    
error:
    return NULL;
}

static struct Data * RemoveFirstNode(struct List *list, bool pop) {
    return RemoveNode(list, GetFirstNode(list), pop);
}

static struct Data * RemoveLastNode(struct List *list, bool pop) {
    return RemoveNode(list, GetLastNode(list), pop);
}

static struct Data * RemoveData(struct List *list,
                                const struct Data *data,
                                List_comp_func_t comp_func,
                                bool pop) {
    // cauta un nod care sa corespunda cu datele specificate
    struct ListNode *node = ContainsData(list, data, comp_func);
    
    if (!node) { goto error; } // verifica daca nodul nu a fost gasit
    
    return RemoveNode(list, node, pop); // sterge nodul
    
error:
    return NULL;
}

static void Clear(struct List *list) {
    if (!list) { goto error; }
    
    if (GetSize(list) == 0) { return; }
    
    struct ListNode *curr = GetFirstNode(list);
    
    while (curr != GetTailNode(list)) {
        struct ListNode *next = GetNodeNext(curr);
        
        RemoveNode(list, curr, false);
        
        curr = next;
    }
    
    return;
    
error:
    return;
}

static void Destroy(struct List *list) {
    if (!list) { goto error; }
    
    // distruge toate nodurile listei (pentru eliberarea memoriei)
    
    Clear(list); // distruge nodurile din interiorul listei
    
    // distruge nodurile santinela
    
    struct ListNode *head = GetHeadNode(list);
    struct ListNode *tail = GetTailNode(list);
    
    head->list = NULL;
    tail->list = NULL;
    
    struct Data *data;
    
    data = DestroyNode(head);
    if (data) { Data_Destroy(data); }
    
    data = DestroyNode(tail);
    if (data) { Data_Destroy(data); }
    
    free(list); // elibereaza memoria ocupata de lista
    
    return;
    
error:
    return;
}

// PUBLIC

struct List * List_Create() {
    return Create();
}

struct Data * ListNode_GetData(const struct ListNode *node) {
    return GetNodeData(node);
}

struct ListNode * ListNode_GetNext(const struct ListNode *node) {
    const struct ListNode *next = GetNodeNext(node);
    
    // daca nodul precedent este nodul santinela de inceput,
    // atunci nu il returna
    if (!GetNodeNext(next)) { return NULL; }
    
    return (struct ListNode *)next;
}

struct ListNode * ListNode_GetPrev(const struct ListNode *node) {
    const struct ListNode *prev = GetNodePrev(node);
    
    // daca nodul precedent este nodul santinela de inceput,
    // atunci nu il returna
    if (!GetNodePrev(prev)) { return NULL; }
    
    return (struct ListNode *)prev;
}

size_t List_GetSize(const struct List *list) {
    return GetSize(list);
}

bool List_IsEmpty(const struct List *list) {
    return IsEmpty(list);
}

bool ListNode_IsHeadOrTail(const struct ListNode *node) {
    return IsNodeHeadOrTail(node);
}

bool ListNode_IsAfter(const struct ListNode *node,
                      const struct ListNode *ref) {
    return IsNodeAfter(node, ref);
}

bool ListNode_IsBefore(const struct ListNode *node,
                       const struct ListNode *ref) {
    return IsNodeBefore(node, ref);
}

bool ListNode_IsBetween(const struct ListNode *node,
                        const struct ListNode *left,
                        const struct ListNode *right) {
    return IsNodeBetween(node, left, right);
}

bool ListNode_IsSuccessor(const struct ListNode *node,
                          const struct ListNode *ref) {
    return IsNodeSuccessor(node, ref);
}

bool ListNode_IsPredecessor(const struct ListNode *node,
                            const struct ListNode *ref) {
    return IsNodePredecessor(node, ref);
}

bool ListNode_IsBounded(const struct ListNode *node,
                        const struct ListNode *left,
                        const struct ListNode *right) {
    return IsNodeBounded(node, left, right);
}

bool ListNode_AreNeighbors(const struct ListNode *left,
                           const struct ListNode *right) {
    return AreNodesNeighbors(left, right);
}

struct ListNode * List_GetFirstNode(const struct List *list) {
    // daca lista nu are niciun element nu returna
    // nodul santinela de sfarsit al listei
    // ce este returnat de functia privata
    return GetSize(list) > 0 ? GetFirstNode(list) : NULL;
}

struct ListNode * List_GetLastNode(const struct List *list) {
    // daca lista nu are niciun element nu returna
    // nodul santinela de inceput al listei
    // ce este returnat de functia privata
    return GetSize(list) > 0 ? GetLastNode(list) : NULL;
}

bool List_ContainsNode(const struct List *list,
                       const struct ListNode *node) {
    bool containsNode = ContainsNode(list, node);
    
    if (!containsNode) { return false; }
    
    // nu lua in considerare nodurile santinela
    if (node == GetHeadNode(list)) { return false; }
    if (node == GetTailNode(list)) { return false; }
    
    return true;
}

struct ListNode * List_ContainsData(const struct List *list,
                                    const struct Data *data,
                                    List_comp_func_t comp_func) {
    return ContainsData(list, data, comp_func);
}

struct ListNode * List_GetNodesMiddle(const struct List *list,
                                      const struct ListNode *left,
                                      const struct ListNode *right) {
    return GetNodesMiddle(list, left, right);
}

struct ListNode * List_GetMiddleNode(const struct List *list) {
    // daca lista nu are niciun element nu returna
    // nodul santinela de inceput al listei
    // ce este returnat de functia privata
    return GetSize(list) > 0 ? GetMiddleNode(list) : NULL;
}

void List_GetSpanNode(const struct List *list,
                      const struct ListNode *start,
                      size_t span,
                      bool forward,
                      struct ListNode **out_end,
                      size_t *out_span) {
    GetSpanNode(list,
                start,
                span,
                forward,
                out_end,
                out_span);
}

void List_GetRadiusNodes(const struct List *list,
                         const struct ListNode *center,
                         size_t radius,
                         struct ListNode **out_left,
                         struct ListNode **out_right,
                         size_t *out_leftRadius,
                         size_t *out_rightRadius) {
    GetRadiusNodes(list,
                   center,
                   radius,
                   out_left,
                   out_right,
                   out_leftRadius,
                   out_rightRadius);
}

struct ListNode * List_AddNodeBetween(struct List *list,
                                      const struct ListNode *node,
                                      struct ListNode *left,
                                      struct ListNode *right) {
    // nu insera direct nodul primit ca parametru, ci o copie a acestuia
    struct ListNode *copy = CopyNode(node);
    
    return AddNodeBetween(list, copy, left, right);
}

struct ListNode * List_AddNodeAfter(struct List *list,
                                    const struct ListNode *node,
                                    struct ListNode *ref) {
    // nu insera direct nodul primit ca parametru, ci o copie a acestuia
    struct ListNode *copy = CopyNode(node);
    
    return AddNodeAfter(list, copy, ref);
}

struct ListNode * List_AddNodeBefore(struct List *list,
                                     const struct ListNode *node,
                                     struct ListNode *ref) {
    // nu insera direct nodul primit ca parametru, ci o copie a acestuia
    struct ListNode *copy = CopyNode(node);
    
    return AddNodeBefore(list, copy, ref);
}

struct ListNode * List_AddNodeFirst(struct List *list,
                                    const struct ListNode *node) {
    // nu insera direct nodul primit ca parametru, ci o copie a acestuia
    struct ListNode *copy = CopyNode(node);
    
    return AddNodeFirst(list, copy);
}

struct ListNode * List_AddNodeLast(struct List *list,
                                   const struct ListNode *node) {
    // nu insera direct nodul primit ca parametru, ci o copie a acestuia
    struct ListNode *copy = CopyNode(node);
    
    return AddNodeLast(list, copy);
}

struct ListNode * List_AddDataBetween(struct List *list,
                                      const struct Data *data,
                                      struct ListNode *left,
                                      struct ListNode *right) {
    return AddDataBetween(list, data, left, right);
}

struct ListNode * List_AddDataAfter(struct List *list,
                                    const struct Data *data,
                                    struct ListNode *ref) {
    return AddDataAfter(list, data, ref);
}

struct ListNode * List_AddDataBefore(struct List *list,
                                     const struct Data *data,
                                     struct ListNode *ref) {
    return AddDataBefore(list, data, ref);
}

struct ListNode * List_AddDataFirst(struct List *list,
                                    const struct Data *data) {
    return AddDataFirst(list, data);
}

struct ListNode * List_AddDataLast(struct List *list,
                                   const struct Data *data) {
    return AddDataLast(list, data);
}

struct List * List_Copy(const struct List *list) {
    return Copy(list);
}

struct List * List_CreateSubNodes(const struct List *list,
                                  const struct ListNode *left,
                                  const struct ListNode *right) {
    return CreateSubNodes(list, left, right);
}

struct List * List_CreateSubSpan(const struct List *list,
                                        const struct ListNode *start,
                                        size_t span,
                                        bool forward) {
    return CreateSubSpan(list, start, span, forward);
}

struct List * List_CreateSubRadius(const struct List *list,
                                   const struct ListNode *center,
                                   size_t radius) {
    return CreateSubRadius(list, center, radius);
}

void List_Print(const struct List *list, List_print_func_t print_func) {
    Print(list, print_func);
}

void List_SwapNodes(struct List *list,
                    struct ListNode *first,
                    struct ListNode *second) {
    SwapNodes(list, first, second);
}

struct ListNode * List_Partition(struct List *list,
                                 struct ListNode *left,
                                 struct ListNode *right,
                                 struct ListNode *pivot,
                                 List_comp_func_t comp_func) {
    if (GetSize(list) == 0) { return NULL; }
    
    return Partition(list, left, right, pivot, comp_func);
}

void List_Sort(struct List *list, List_comp_func_t comp_func) {
    if (GetSize(list) == 0) { return; }
    
    Sort(list, GetFirstNode(list), GetLastNode(list), comp_func);
}

struct Data * List_RemoveNode(struct List *list,
                              struct ListNode *node,
                              bool pop) {
    return RemoveNode(list, node, pop);
}

struct Data * List_RemoveFirstNode(struct List *list, bool pop) {
    return RemoveFirstNode(list, pop);
}

struct Data * List_RemoveLastNode(struct List *list, bool pop) {
    return RemoveLastNode(list, pop);
}

struct Data * List_RemoveData(struct List *list,
                              const struct Data *data,
                              List_comp_func_t comp_func,
                              bool pop) {
    return RemoveData(list, data, comp_func, pop);
}

void List_Clear(struct List *list) {
    Clear(list);
}

void List_Destroy(struct List *list) {
    Destroy(list);
}
