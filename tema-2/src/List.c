#include "List.h"

#include "Data.h"

#include "helpers.h"

#include <stdarg.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define NDEBUG
#include <assert.h>

// PRIVATE

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

static struct Data * GetNodeData(const struct ListNode *node) {
	assert(node);
	
	return node->data;
}

static struct ListNode * GetNodeNext(const struct ListNode *node) {
	assert(node);
	
	return node->next;
}

static struct ListNode * GetNodePrev(const struct ListNode *node) {
	assert(node);
	
	return node->prev;
}

static struct List * GetNodeList(const struct ListNode *node) {
	assert(node);
	
	return node->list;
}

static void SetNodeData(struct ListNode *node, const struct Data *data) {
	assert(node);
	
	node->data = (struct Data *)data;
}

static void SetNodeNext(struct ListNode *node, const struct ListNode *next) {
	assert(node);
	
	node->next = (struct ListNode *)next;
}

static void SetNodePrev(struct ListNode *node, const struct ListNode *prev) {
	assert(node);
	
	node->prev = (struct ListNode *)prev;
}

static void SetNodeList(struct ListNode *node, const struct List *list) {
	assert(node);
	
	node->list = (struct List *)list;
}

static struct ListNode * CreateNode(const struct Data *data) {
	struct ListNode *node = malloc(sizeof *node);
	
	SetNodeData(node, data ? Data_Copy(data) : NULL); // copiaza datele in nod
	
	// initializeaza legaturile nodului
	SetNodeNext(node, NULL);
	SetNodePrev(node, NULL);
	
	SetNodeList(node, NULL);
	
	return node;
}

static struct ListNode * CopyNode(const struct ListNode *node) {
	assert(node);
	
	return CreateNode(GetNodeData(node));
}

static struct ListNode * Node_Data_Unwrap(const struct Data *data) {
	assert(data);
	assert(data->mem);
	
	return *(struct ListNode **)data->mem;
}

static size_t GetSize(const struct List *list) {
	assert(list);
	
	return list->size;
}

static struct ListNode * GetHeadNode(const struct List *list) {
	assert(list);
	
	return list->head;
}

static struct ListNode * GetTailNode(const struct List *list) {
	assert(list);
	
	return list->tail;
}

static void SetSize(struct List *list, size_t size) {
	assert(list);
	
	list->size = size;
}

static void SetHead(struct List *list, const struct ListNode *head) {
	assert(list);
	
	list->head = (struct ListNode *)head;
}

static void SetTail(struct List *list, const struct ListNode *tail) {
	assert(list);
	
	list->tail = (struct ListNode *)tail;
}

static struct List * Create() {
	// creeaza lista
	struct List *list = malloc(sizeof *list);
	
	// creeaza nodurile santinela ale listei
	struct ListNode *head = CreateNode(NULL);
	struct ListNode *tail = CreateNode(NULL);
	
	// leaga cele doua noduri santinela intre ele
	SetNodeNext(head, tail);
	SetNodePrev(tail, head);
	
	// seteaza lista caruia nodurile santinela apratin
	SetNodeList(head, list);
	SetNodeList(tail, list);
	
	// initializeaza lista
	
	SetSize(list, 0);
	
	SetHead(list, head);
	SetTail(list, tail);
	
	return list;
}

static bool UNUSED IsNodeOrphan(const struct ListNode *node) {
	assert(node);
	
	return GetNodeList(node) == NULL;
}

static bool UNUSED IsNodeHeadOrTail(const struct ListNode *node) {
	assert(node);
	
	const struct List *list = GetNodeList(node);
	
	assert(list);
	
	return (node == GetHeadNode(list)) || (node == GetTailNode(list));
}

static bool AreNodesConnected_va_list(size_t count,
                                      const struct ListNode *first,
                                      va_list ap)
{
	assert(count >= 2);
	
	assert(first);
	
	const struct List *firstList = GetNodeList(first);
	
	assert(firstList);
	
	for (size_t i = 2; i < count; ++i) {
		const struct ListNode *node = va_arg(ap, const struct ListNode *);
		
		assert(node);
		
		const struct List *list = GetNodeList(node);
		
		assert(list);
		
		if (list != firstList) {
			return false;
		}
	}
	
	return true;
}

static bool AreNodesConnected_va_arg(size_t count,
                                     const struct ListNode *first,
                                     ...)
{
	assert(count >= 2);
	
	assert(first);
	
	va_list ap;
	va_start(ap, first);
	
	bool res = AreNodesConnected_va_list(count, first, ap);
	
	va_end(ap);
	
	return res;
}

static bool UNUSED AreNodesConnected(const struct ListNode *first,
                                     const struct ListNode *second)
{
	assert(first);
	assert(second);
	
	return AreNodesConnected_va_arg(2, first, second);
}

static bool IsNodeAfter(const struct ListNode *node,
                        const struct ListNode *ref)
{
	assert(node);
	assert(ref);
	
	assert(AreNodesConnected(node, ref));
	
	return GetNodeNext(ref) == node;
}

static bool IsNodeBefore(const struct ListNode *node,
                         const struct ListNode *ref)
{
	assert(node);
	assert(ref);
	
	assert(AreNodesConnected(node, ref));
	
	return GetNodePrev(ref) == node;
}

static bool IsNodeBetween(const struct ListNode *node,
                          const struct ListNode *left,
                          const struct ListNode *right)
{
	assert(node);
	assert(left);
	assert(right);
	
	assert(AreNodesConnected_va_arg(3, node, left, right));
	
	return IsNodeAfter(node, left) && IsNodeBefore(node, right);
}

static bool IsNodeSuccessor(const struct ListNode *node,
                            const struct ListNode *ref)
{
	assert(node);
	assert(ref);
	
	assert(AreNodesConnected(node, ref));
	
	const struct List *list = GetNodeList(node);
	
	if (node == ref) { return false; }
	if (node == GetTailNode(list)) { return true; }
	if (ref == GetHeadNode(list)) { return true; }
	
	while ((node = GetNodePrev(node)) != GetHeadNode(list)) {
		if (node == ref) { return true; }
	}
	
	return false;
}

static bool IsNodePredecessor(const struct ListNode *node,
                              const struct ListNode *ref)
{
	assert(node);
	assert(ref);
	
	assert(AreNodesConnected(node, ref));
	
	const struct List *list = GetNodeList(node);
	
	if (node == ref) { return false; }
	if (node == GetHeadNode(list)) { return true; }
	if (ref == GetTailNode(list)) { return true; }
	
	while ((node = GetNodeNext(node)) != GetTailNode(list)) {
		if (node == ref) { return true; }
	}
	
	return false;
}

static bool IsNodeBoundedBy(const struct ListNode *node,
                            const struct ListNode *left,
                            const struct ListNode *right)
{
	assert(node);
	assert(left);
	assert(right);
	
	assert(AreNodesConnected_va_arg(3, node, left, right));
	
	return (IsNodeSuccessor(node, left) && IsNodePredecessor(node, right)) ||
	       (IsNodeSuccessor(node, right) && IsNodePredecessor(node, left));
}

static bool AreNodesNeighbors(const struct ListNode *left,
                              const struct ListNode *right) {
	assert(left);
	assert(right);
	
	assert(AreNodesConnected(left, right));
	
	return ((GetNodeNext(left) == right) && (GetNodePrev(right) == left)) ||
	       ((GetNodeNext(right) == left) && (GetNodePrev(left) == right));
}

static struct ListNode * GetFirstNode(const struct List *list) {
	assert(list);
	
	return GetNodeNext(GetHeadNode(list));
}

static struct ListNode * GetLastNode(const struct List *list) {
	assert(list);
	
	return GetNodePrev(GetTailNode(list));
}

static bool IsEmpty(const struct List *list) {
	assert(list);
	
	return GetSize(list) == 0;
}

static bool ContainsNode(const struct List *list,
                         const struct ListNode *node)
{
	assert(list);
	assert(node);
	
	return GetNodeList(node) == list;
}

static struct ListNode * ContainsData(const struct List *list,
                                      const struct Data *data,
                                      Data_comp_func_t comp_func)
{
	assert(list);
	assert(data);
	assert(comp_func);
	
	struct ListNode *curr = GetFirstNode(list);
	
	while (curr != GetTailNode(list)) {
		// verifica daca a fost gasit un nod
		// ale carui date corespund cu cele cautate
		if (comp_func(GetNodeData(curr), data) == 0) { return curr; }
		
		curr = GetNodeNext(curr);
	}
	
	return NULL;
}

static struct ListNode * GetNodesMiddle(const struct List UNUSED *list,
                                        const struct ListNode *left,
                                        const struct ListNode *right)
{
	assert(list);
	assert(left);
	assert(right);
	
	assert(ContainsNode(list, left));
	assert(ContainsNode(list, right));
	
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
	       (GetNodeNext(GetNodeNext(doubleStep)) != GetNodeNext(right)))
	{
		doubleStep = GetNodeNext(GetNodeNext(doubleStep));
		
		step = GetNodeNext(step);
	}
	
	return (struct ListNode *)step;
}

static struct ListNode * GetMiddleNode(const struct List *list) {
	assert(list);
	
	return GetNodesMiddle(list, GetHeadNode(list), GetTailNode(list));
}

static struct Data * DestroyNode(struct ListNode *node) {
	assert(node);
	
	// verifica daca se incearca distrugerea
	// unui nod care inca apartine unei liste
	assert(IsNodeOrphan(node));
	
	struct Data *data = GetNodeData(node);
	
	free(node); // elibereaza memoria ocupata de nod
	
	return data;
}

static void GetNodeSpanNode(const struct ListNode *start,
                            size_t span,
                            bool forward,
                            struct ListNode **out_end,
                            size_t *out_span)
{
	assert(start);
	assert(out_end);
	
	assert(!IsNodeOrphan(start));
	assert(!IsNodeHeadOrTail(start));
	
	struct List *list = GetNodeList(start);
	
	struct ListNode *boundary = forward ?
	                            GetLastNode(list) :
	                            GetFirstNode(list);
	
	struct ListNode * (*step_func)(const struct ListNode *) = forward ?
	                                                          GetNodeNext :
	                                                          GetNodePrev;
	
	const struct ListNode *curr = start;
	size_t currSpan = 0;
	
	while (currSpan < span) {
		if (curr == boundary) { break; }
		
		curr = step_func(curr);
		
		++currSpan;
	}
	
	*out_end = (struct ListNode *)curr;
	if (out_span) { *out_span = currSpan; }
}

static void GetNodeRadiusNodes(const struct ListNode *center,
                               size_t radius,
                               struct ListNode **out_left,
                               struct ListNode **out_right,
                               size_t *out_leftRadius,
                               size_t *out_rightRadius)
{
	assert(center);
	assert(out_left);
	assert(out_right);
	
	assert(!IsNodeOrphan(center));
	assert(!IsNodeHeadOrTail(center));
	
	struct List *list = GetNodeList(center);
	
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
}

static struct ListNode * AddNodeBetween(struct List *list,
                                        struct ListNode *node,
                                        struct ListNode *left,
                                        struct ListNode *right)
{
	assert(list);
	assert(node);
	assert(left);
	assert(right);
	
	// verifica daca nodurile se afla in lista
	assert(ContainsNode(list, left));
	assert(ContainsNode(list, right));
	
	// verifica daca se incearca inserarea
	// inainte de nodul santinela de inceput
	// sau dupa nodul santinela de sfarsit
	assert(right != GetHeadNode(list));
	assert(left != GetTailNode(list));
	
	// verifica daca nodurile intre care se insereaza sunt vecine
	assert(AreNodesNeighbors(left, right));
	
	// configureaza legaturile nodului
	SetNodePrev(node, left);
	SetNodeNext(node, right);
	
	SetNodeList(node, list); // seteaza lista caruia nodul apartine
	
	// insereaza nodul in lista
	SetNodeNext(left, node);
	SetNodePrev(right, node);
	
	SetSize(list, GetSize(list) + 1); // mareste numarul de noduri din lista
	
	return node; // returneaza nodul inserat
}

static struct ListNode * AddNodeAfter(struct List *list,
                                      struct ListNode *node,
                                      struct ListNode *ref)
{
	assert(list);
	assert(node);
	assert(ref);
	
	return AddNodeBetween(list, node, ref, GetNodeNext(ref));
}

static struct ListNode * AddNodeBefore(struct List *list,
                                       struct ListNode *node,
                                       struct ListNode *ref)
{
	assert(list);
	assert(node);
	assert(ref);
	
	return AddNodeBetween(list, node, GetNodePrev(ref), ref);
}

static struct ListNode * AddNodeFirst(struct List *list,
                                      struct ListNode *node)
{
	assert(list);
	assert(node);
	
	return AddNodeAfter(list, node, GetHeadNode(list));
}

static struct ListNode * AddNodeLast(struct List *list,
                                     struct ListNode *node)
{
	assert(list);
	assert(node);
	
	return AddNodeBefore(list, node, GetTailNode(list));
}

static struct ListNode * AddDataBetween(struct List *list,
                                        const struct Data *data,
                                        struct ListNode *left,
                                        struct ListNode *right)
{
	assert(list);
	assert(data);
	assert(left);
	assert(right);
	
	assert(AreNodesNeighbors(left, right));
	
	return AddNodeBetween(list, CreateNode(data), left, right);;
}

static struct ListNode * AddDataAfter(struct List *list,
                                      const struct Data *data,
                                      struct ListNode *ref)
{
	assert(list);
	assert(data);
	assert(ref);
	
	return AddDataBetween(list, data, ref, GetNodeNext(ref));
}

static struct ListNode * AddDataBefore(struct List *list,
                                       const struct Data *data,
                                       struct ListNode *ref)
{
	assert(list);
	assert(data);
	assert(ref);
	
	return AddDataBetween(list, data, GetNodePrev(ref), ref);
}

static struct ListNode * AddDataFirst(struct List *list,
                                      const struct Data *data)
{
	assert(list);
	assert(data);
	
	return AddDataAfter(list, data, GetHeadNode(list));
}

static struct ListNode * AddDataLast(struct List *list,
                                     const struct Data *data)
{
	assert(list);
	assert(data);
	
	return AddDataBefore(list, data, GetTailNode(list));
}

static struct List * CreateSubFromNodes(const struct List UNUSED *list,
                                        const struct ListNode *left,
                                        const struct ListNode *right)
{
	assert(list);
	assert(left);
	assert(right);
	
	// verifica daca nodurile se afla in lista
	assert(ContainsNode(list, left));
	assert(ContainsNode(list, right));
	
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
}

static struct List * Copy(const struct List *list) {
	assert(list);
	
	struct List *copy = Create();
	
	const struct ListNode *curr = GetFirstNode(list);
	
	while (curr != GetTailNode(list)) {
		struct ListNode *nodeCopy = CopyNode(curr);
		
		AddNodeLast(copy, nodeCopy);
		
		curr = GetNodeNext(curr);
	}
	
	return copy;
}

static struct List * CreateSubFromSpan(const struct List *list,
                                       const struct ListNode *start,
                                       size_t span,
                                       bool forward)
{
	assert(list);
	assert(start);
	
	assert(ContainsNode(list, start));
	
	struct ListNode *end;
	GetNodeSpanNode(start, span, forward, &end, NULL);
	
	return CreateSubFromNodes(list, start, end);
}

static struct List * CreateSubFromRadius(const struct List *list,
                                         const struct ListNode *center,
                                         size_t radius)
{
	assert(list);
	assert(center);
	
	assert(ContainsNode(list, center));
	
	struct ListNode *left;
	struct ListNode *right;
	GetNodeRadiusNodes(center, radius, &left, &right, NULL, NULL);
	
	return CreateSubFromNodes(list, left, right);;
}

static void Process(struct List *list,
                    Data_proc_func_t proc_func)
{
	assert(list);
	assert(proc_func);
	
	struct ListNode *curr = GetFirstNode(list);
	
	while (curr != GetTailNode(list)) {
		proc_func(GetNodeData(curr));
		
		curr = GetNodeNext(curr);
	}
}

static void Print(const struct List *list,
                  Data_print_func_t print_func,
                  const char *delim,
                  const char *endMark,
                  FILE *stream)
{
	assert(list);
	assert(print_func);
	assert(stream);
	
	struct ListNode *curr = GetFirstNode(list);
	
	while (curr != GetTailNode(list)) {
		print_func(GetNodeData(curr), stream);
		
		if (curr != GetLastNode(list)) {
			if (delim) { fprintf(stream, "%s", delim); }
		}
		
		curr = GetNodeNext(curr);
	}
	
	if (endMark) { fprintf(stream, "%s", endMark); }
}

static void SwapNodes(struct List UNUSED *list,
                      struct ListNode *first,
                      struct ListNode *second)
{
	assert(list);
	assert(first);
	assert(second);
	
	// verifica daca nodurile se afla in lista
	assert(ContainsNode(list, first));
	assert(ContainsNode(list, second));
	
	// verifica daca vreunul din noduri este nod santinela
	assert(!IsNodeHeadOrTail(first));
	assert(!IsNodeHeadOrTail(second));
	
	// https://stackoverflow.com/a/27041055
	
	if (first == second) { return; }
	
	bool areNodesNeighbors = AreNodesNeighbors(first, second);
	
	if (areNodesNeighbors && (GetNodeNext(second) == first)) {
		struct ListNode *temp = first;
		first = second;
		second = temp;
	}
	
	struct ListNode *swapper[4];
	swapper[0] = GetNodePrev(first);
	swapper[1] = GetNodePrev(second);
	swapper[2] = GetNodeNext(first);
	swapper[3] = GetNodeNext(second);
	
	if (areNodesNeighbors) {
		SetNodePrev(first, swapper[2]);
		SetNodePrev(second, swapper[0]);
		SetNodeNext(first, swapper[3]);
		SetNodeNext(second, swapper[1]);
	} else {
		SetNodePrev(first, swapper[1]);
		SetNodePrev(second, swapper[0]);
		SetNodeNext(first, swapper[3]);
		SetNodeNext(second, swapper[2]);
	}
	
	SetNodeNext(GetNodePrev(first), first);
	SetNodePrev(GetNodeNext(first), first);
	
	SetNodeNext(GetNodePrev(second), second);
	SetNodePrev(GetNodeNext(second), second);
}

static void SwapData(struct List UNUSED *list,
                     struct ListNode *first,
                     struct ListNode *second)
{
	assert(list);
	assert(first);
	assert(second);
	
	// verifica daca nodurile se afla in lista
	assert(ContainsNode(list, first));
	assert(ContainsNode(list, second));
	
	// verifica daca vreunul din noduri este nod santinela
	assert(!IsNodeHeadOrTail(first));
	assert(!IsNodeHeadOrTail(second));
	
	struct Data *temp = GetNodeData(first);
	SetNodeData(first, GetNodeData(second));
	SetNodeData(second, temp);
}

static struct ListNode * Partition(struct List *list,
                                   struct ListNode *left,
                                   struct ListNode *right,
                                   struct ListNode *pivot,
                                   Data_comp_func_t comp_func)
{
	assert(list);
	assert(left);
	assert(right);
	assert(pivot);
	assert(comp_func);
	
	// verifica daca nodurile se afla in lista
	assert(ContainsNode(list, left));
	assert(ContainsNode(list, right));
	assert(ContainsNode(list, pivot));
	
	// verifica daca vreunul din noduri este nod santinela
	assert(!IsNodeHeadOrTail(left));
	assert(!IsNodeHeadOrTail(right));
	assert(!IsNodeHeadOrTail(pivot));
	
	// verifica daca nodul pivot este vreun nod de delimitare
	// sau daca se afla intre nodurile de delimitare
	assert((pivot == left) || (pivot == right) || IsNodeBoundedBy(pivot, left, right));
	
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
}

static void quick_sort(struct List *list,
                       struct ListNode *left,
                       struct ListNode *right,
                       Data_comp_func_t comp_func)
{
	assert(list);
	assert(left);
	assert(right);
	assert(comp_func);
	
	if ((left == right) || IsNodeAfter(left, right)) { return; }
	
	struct ListNode *pivot = Partition(list, left, right, right, comp_func);
	
	quick_sort(list, left, GetNodePrev(pivot), comp_func);
	// pivotul se afla pe pozitia potrivita
	quick_sort(list, GetNodeNext(pivot), right, comp_func);
}

static void Sort(struct List *list,
                 struct ListNode *left,
                 struct ListNode *right,
                 Data_comp_func_t comp_func)
{
	assert(list);
	assert(left);
	assert(right);
	assert(comp_func);
	
	// verifica daca nodurile se afla in lista
	assert(ContainsNode(list, left));
	assert(ContainsNode(list, right));
	
	// verifica daca cele doua noduri de delimitare
	// sunt in ordinea potrivita
	if (!IsNodePredecessor(left, right)) {
		struct ListNode *temp = left;
		left = right;
		right = temp;
	}
	
	// foloseste algoritmul quicksort pentru sortarea listei
	quick_sort(list, left, right, comp_func);
}

static struct Data * RemoveNode(struct List *list,
                                struct ListNode *node,
                                bool pop)
{
	assert(list);
	assert(node);
	
	assert(ContainsNode(list, node));
	
	// verifica daca se incearca eliminarea vreunui nod santinela
	assert(!IsNodeHeadOrTail(node));
	
	// inlatura nodul din lista
	SetNodePrev(GetNodeNext(node), GetNodePrev(node));
	SetNodeNext(GetNodePrev(node), GetNodeNext(node));
	
	SetSize(list, GetSize(list) - 1); // scade numarul de noduri din lista
	
	SetNodeNext(node, NULL);
	SetNodePrev(node, NULL);
	
	SetNodeList(node, NULL);
	
	struct Data *data = DestroyNode(node);
	
	// returneaza sau distruge datele ramase
	return pop ? data : (Data_Destroy(data), NULL);
}

static struct Data * RemoveFirstNode(struct List *list, bool pop) {
	assert(list);
	
	return RemoveNode(list, GetFirstNode(list), pop);
}

static struct Data * RemoveLastNode(struct List *list, bool pop) {
	assert(list);
	
	return RemoveNode(list, GetLastNode(list), pop);
}

static struct Data * RemoveData(struct List *list,
                                const struct Data *data,
                                Data_comp_func_t comp_func,
                                bool pop)
{
	assert(list);
	assert(data);
	assert(comp_func);
	
	// cauta un nod care sa corespunda cu datele specificate
	struct ListNode *node = ContainsData(list, data, comp_func);
	
	return node ? RemoveNode(list, node, pop) : NULL;
}

static void Clear(struct List *list) {
	assert(list);
	
	if (IsEmpty(list)) { return; }
	
	struct ListNode *curr = GetFirstNode(list);
	
	while (curr != GetTailNode(list)) {
		struct ListNode *next = GetNodeNext(curr);
		
		RemoveNode(list, curr, false);
		
		curr = next;
	}
}

static void Destroy(struct List *list) {
	assert(list);
	
	// distruge toate nodurile listei (pentru eliberarea memoriei)
	
	Clear(list); // distruge nodurile din interiorul listei
	
	// distruge nodurile santinela
	
	struct ListNode *head = GetHeadNode(list);
	struct ListNode *tail = GetTailNode(list);
	
	SetNodeList(head, NULL);
	SetNodeList(tail, NULL);
	
	struct Data *data;
	
	data = DestroyNode(head);
	if (data) { Data_Destroy(data); }
	
	data = DestroyNode(tail);
	if (data) { Data_Destroy(data); }
	
	free(list);
}

// PUBLIC

struct Data * ListNode_GetData(const struct ListNode *node) {
	assert(node);
	
	return GetNodeData(node);
}

struct ListNode * ListNode_GetNext(const struct ListNode *node) {
	assert(node);
	
	const struct ListNode *next = GetNodeNext(node);
	
	// daca nodul precedent este nodul santinela de inceput,
	// atunci nu il returna
	if (!GetNodeNext(next)) { return NULL; }
	
	return (struct ListNode *)next;
}

struct ListNode * ListNode_GetPrev(const struct ListNode *node) {
	assert(node);
	
	const struct ListNode *prev = GetNodePrev(node);
	
	// daca nodul precedent este nodul santinela de inceput,
	// atunci nu il returna
	if (!GetNodePrev(prev)) { return NULL; }
	
	return (struct ListNode *)prev;
}

struct ListNode * ListNode_Data_Unwrap(const struct Data *data) {
	assert(data);
	
	return Node_Data_Unwrap(data);
}

size_t List_GetSize(const struct List *list) {
	assert(list);
	
	return GetSize(list);
}

struct List * List_Create() {
	return Create();
}

bool List_IsEmpty(const struct List *list) {
	assert(list);
	
	return IsEmpty(list);
}

bool ListNode_IsAfter(const struct ListNode *node,
                      const struct ListNode *ref)
{
	assert(node);
	assert(ref);
	
	return IsNodeAfter(node, ref);
}

bool ListNode_IsBefore(const struct ListNode *node,
                       const struct ListNode *ref)
{
	assert(node);
	assert(ref);
	
	return IsNodeBefore(node, ref);
}

bool ListNode_IsBetween(const struct ListNode *node,
                        const struct ListNode *left,
                        const struct ListNode *right)
{
	assert(node);
	assert(left);
	assert(right);
	
	return IsNodeBetween(node, left, right);
}

bool ListNode_IsSuccessor(const struct ListNode *node,
                          const struct ListNode *ref)
{
	assert(node);
	assert(ref);
	
	return IsNodeSuccessor(node, ref);
}

bool ListNode_IsPredecessor(const struct ListNode *node,
                            const struct ListNode *ref)
{
	assert(node);
	assert(ref);
	
	return IsNodePredecessor(node, ref);
}

bool ListNode_IsBoundedBy(const struct ListNode *node,
                          const struct ListNode *left,
                          const struct ListNode *right)
{
	assert(node);
	assert(left);
	assert(right);
	
	return IsNodeBoundedBy(node, left, right);
}

bool ListNode_IsNeighborWith(const struct ListNode *node,
                             const struct ListNode *other)
{
	assert(node);
	assert(other);
	
	return AreNodesNeighbors(node, other);
}

struct ListNode * List_GetFirstNode(const struct List *list) {
	assert(list);
	
	// daca lista nu are niciun element nu returna
	// nodul santinela de sfarsit al listei
	// ce este returnat de functia privata
	return !IsEmpty(list) ? GetFirstNode(list) : NULL;
}

struct ListNode * List_GetLastNode(const struct List *list) {
	assert(list);
	
	// daca lista nu are niciun element nu returna
	// nodul santinela de inceput al listei
	// ce este returnat de functia privata
	return !IsEmpty(list) ? GetLastNode(list) : NULL;
}

bool List_ContainsNode(const struct List *list,
                       const struct ListNode *node)
{
	assert(list);
	assert(node);
	
	bool containsNode = ContainsNode(list, node);
	
	if (!containsNode) { return false; }
	
	// nu lua in considerare nodurile santinela
	if (node == GetHeadNode(list)) { return false; }
	if (node == GetTailNode(list)) { return false; }
	
	return true;
}

struct ListNode * List_ContainsData(const struct List *list,
                                    const struct Data *data,
                                    Data_comp_func_t comp_func)
{
	assert(list);
	assert(data);
	assert(comp_func);
	
	return ContainsData(list, data, comp_func);
}

struct ListNode * List_GetNodesMiddle(const struct List *list,
                                      const struct ListNode *left,
                                      const struct ListNode *right)
{
	assert(list);
	assert(left);
	assert(right);
	
	return GetNodesMiddle(list, left, right);
}

struct ListNode * List_GetMiddleNode(const struct List *list) {
	assert(list);
	
	// daca lista nu are niciun element nu returna
	// nodul santinela de inceput al listei
	// ce este returnat de functia privata
	return !IsEmpty(list) ? GetMiddleNode(list) : NULL;
}

void ListNode_GetSpanNode(const struct ListNode *start,
                          size_t span,
                          bool forward,
                          struct ListNode **out_end,
                          size_t *out_span)
{
	assert(start);
	assert(out_end);
	
	GetNodeSpanNode(start,
	                span,
	                forward,
	                out_end,
	                out_span);
}

void ListNode_GetRadiusNodes(const struct ListNode *center,
                             size_t radius,
                             struct ListNode **out_left,
                             struct ListNode **out_right,
                             size_t *out_leftRadius,
                             size_t *out_rightRadius)
{
	assert(center);
	assert(out_left);
	assert(out_right);
	
	GetNodeRadiusNodes(center,
	                   radius,
	                   out_left,
	                   out_right,
	                   out_leftRadius,
	                   out_rightRadius);
}

struct ListNode * List_AddNodeCopyBetween(struct List *list,
                                          const struct ListNode *node,
                                          struct ListNode *left,
                                          struct ListNode *right)
{
	assert(list);
	assert(node);
	assert(left);
	assert(right);
	
	// nu insera direct nodul primit ca parametru, ci o copie a acestuia
	struct ListNode *copy = CopyNode(node);
	
	return AddNodeBetween(list, copy, left, right);
}

struct ListNode * List_AddNodeCopyAfter(struct List *list,
                                        const struct ListNode *node,
                                        struct ListNode *ref)
{
	assert(list);
	assert(node);
	assert(ref);
	
	// nu insera direct nodul primit ca parametru, ci o copie a acestuia
	struct ListNode *copy = CopyNode(node);
	
	return AddNodeAfter(list, copy, ref);
}

struct ListNode * List_AddNodeCopyBefore(struct List *list,
                                         const struct ListNode *node,
                                         struct ListNode *ref)
{
	assert(list);
	assert(node);
	assert(ref);
	
	// nu insera direct nodul primit ca parametru, ci o copie a acestuia
	struct ListNode *copy = CopyNode(node);
	
	return AddNodeBefore(list, copy, ref);
}

struct ListNode * List_AddNodeCopyFirst(struct List *list,
                                        const struct ListNode *node)
{
	assert(list);
	assert(node);
	
	// nu insera direct nodul primit ca parametru, ci o copie a acestuia
	struct ListNode *copy = CopyNode(node);
	
	return AddNodeFirst(list, copy);
}

struct ListNode * List_AddNodeCopyLast(struct List *list,
                                       const struct ListNode *node)
{
	assert(list);
	assert(node);
	
	// nu insera direct nodul primit ca parametru, ci o copie a acestuia
	struct ListNode *copy = CopyNode(node);
	
	return AddNodeLast(list, copy);
}

struct ListNode * List_AddDataBetween(struct List *list,
                                      const struct Data *data,
                                      struct ListNode *left,
                                      struct ListNode *right)
{
	assert(list);
	assert(data);
	assert(left);
	assert(right);
	
	return AddDataBetween(list, data, left, right);
}

struct ListNode * List_AddDataAfter(struct List *list,
                                    const struct Data *data,
                                    struct ListNode *ref)
{
	assert(list);
	assert(data);
	assert(ref);
	
	return AddDataAfter(list, data, ref);
}

struct ListNode * List_AddDataBefore(struct List *list,
                                     const struct Data *data,
                                     struct ListNode *ref)
{
	assert(list);
	assert(data);
	assert(ref);
	
	return AddDataBefore(list, data, ref);
}

struct ListNode * List_AddDataFirst(struct List *list,
                                    const struct Data *data)
{
	assert(list);
	assert(data);
	
	return AddDataFirst(list, data);
}

struct ListNode * List_AddDataLast(struct List *list,
                                   const struct Data *data)
{
	assert(list);
	assert(data);
	
	return AddDataLast(list, data);
}

struct List * List_Copy(const struct List *list) {
	assert(list);
	
	return Copy(list);
}

struct List * List_CreateSubFromNodes(const struct List *list,
                                  const struct ListNode *left,
                                  const struct ListNode *right)
{
	assert(list);
	assert(left);
	assert(right);
	
	return CreateSubFromNodes(list, left, right);
}

struct List * List_CreateSubFromSpan(const struct List *list,
                                     const struct ListNode *start,
                                     size_t span,
                                     bool forward)
{
	assert(list);
	assert(start);
	
	return CreateSubFromSpan(list, start, span, forward);
}

struct List * List_CreateSubFromRadius(const struct List *list,
                                       const struct ListNode *center,
                                       size_t radius)
{
	assert(list);
	assert(center);
	
	return CreateSubFromRadius(list, center, radius);
}

void List_Process(struct List *list,
                  Data_proc_func_t proc_func)
{
	assert(list);
	assert(proc_func);
	
	Process(list, proc_func);
}

void List_Print(const struct List *list,
                Data_print_func_t print_func,
                const char *delim,
                const char *endMark,
                FILE *stream)
{
	assert(list);
	assert(print_func);
	assert(stream);
	
	Print(list, print_func, delim, endMark, stream);
}

void List_SwapNodes(struct List *list,
                    struct ListNode *first,
                    struct ListNode *second)
{
	assert(list);
	assert(first);
	assert(second);
	
	SwapNodes(list, first, second);
}

struct ListNode * List_Partition(struct List *list,
                                 struct ListNode *left,
                                 struct ListNode *right,
                                 struct ListNode *pivot,
                                 Data_comp_func_t comp_func)
{
	assert(list);
	assert(left);
	assert(right);
	assert(pivot);
	assert(comp_func);
	
	return Partition(list, left, right, pivot, comp_func);
}

void List_Sort(struct List *list, Data_comp_func_t comp_func) {
	assert(list);
	assert(comp_func);
	
	Sort(list, GetFirstNode(list), GetLastNode(list), comp_func);
}

struct Data * List_RemoveNode(struct List *list,
                              struct ListNode *node,
                              bool pop)
{
	assert(list);
	assert(node);
	
	return RemoveNode(list, node, pop);
}

struct Data * List_RemoveFirstNode(struct List *list, bool pop) {
	assert(list);
	
	return RemoveFirstNode(list, pop);
}

struct Data * List_RemoveLastNode(struct List *list, bool pop) {
	assert(list);
	
	return RemoveLastNode(list, pop);
}

struct Data * List_RemoveData(struct List *list,
                              const struct Data *data,
                              Data_comp_func_t comp_func,
                              bool pop)
{
	assert(list);
	assert(data);
	assert(comp_func);
	
	return RemoveData(list, data, comp_func, pop);
}

void List_Clear(struct List *list) {
	assert(list);
	
	Clear(list);
}

void List_Destroy(struct List *list) {
	assert(list);
	
	Destroy(list);
}
