#include "BSTree.h"

#include "Data.h"
#include "List.h"

#include "helpers.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define NDEBUG
#include <assert.h>

// PRIVATE

struct BSTreeNode {
	struct Data *data;
	
	struct BSTreeNode *left;
	struct BSTreeNode *right;
	struct BSTreeNode *parent;
	
	struct BSTree *tree;
};

struct BSTree {
	size_t size;
	
	struct BSTreeNode *root;
};

static struct Data * GetNodeData(const struct BSTreeNode *node) {
	assert(node);
	
	return node->data;
}

static struct BSTreeNode * GetNodeLeft(const struct BSTreeNode *node) {
	assert(node);
	
	return node->left;
}

static struct BSTreeNode * GetNodeRight(const struct BSTreeNode *node) {
	assert(node);
	
	return node->right;
}

static struct BSTreeNode * GetNodeParent(const struct BSTreeNode *node) {
	assert(node);
	
	return node->parent;
}

static struct BSTree * GetNodeTree(const struct BSTreeNode *node) {
	assert(node);
	
	return node->tree;
}

static void SetNodeData(struct BSTreeNode *node, const struct Data *data) {
	assert(node);
	
	node->data = (struct Data *)data;
}

static void SetNodeParent(struct BSTreeNode *node,
                          const struct BSTreeNode *parent)
{
	assert(node);
	// parent poate fi si NULL
	
	node->parent = (struct BSTreeNode *)parent;
}

static void SetNodeLeft(struct BSTreeNode *node,
                        const struct BSTreeNode *left)
{
	assert(node);
	// left poate fi si NULL
	
	node->left = (struct BSTreeNode *)left;
}

static void SetNodeRight(struct BSTreeNode *node,
                         const struct BSTreeNode *right)
{
	assert(node);
	// right poate fi si NULL
	
	node->right = (struct BSTreeNode *)right;
}

static void SetNodeTree(struct BSTreeNode *node, const struct BSTree *tree) {
	assert(node);
	
	node->tree = (struct BSTree *)tree;
}

static bool IsNodeLeaf(const struct BSTreeNode *node) {
	assert(node);
	
	return !GetNodeLeft(node) && !GetNodeRight(node);
}

static struct BSTreeNode * CreateNode(const struct Data *data) {
	struct BSTreeNode *node = malloc(sizeof *node);
	
	SetNodeData(node, data ? Data_Copy(data) : NULL);
	
	SetNodeLeft(node, NULL);
	SetNodeRight(node, NULL);
	SetNodeParent(node, NULL);
	
	SetNodeTree(node, NULL);
	
	return node;
}

static struct BSTreeNode * CopyNode(const struct BSTreeNode *node) {
	assert(node);
	
	return CreateNode(GetNodeData(node));
}

static bool UNUSED IsNodeOrphan(const struct BSTreeNode *node) {
	assert(node);
	
	return GetNodeTree(node) == NULL;
}

static struct BSTreeNode * GetNodeMin(const struct BSTreeNode *node) {
	assert(node);
	
	assert(!IsNodeOrphan(node));
	
	while (GetNodeLeft(node)) { node = GetNodeLeft(node); }
	
	return (struct BSTreeNode *)node;
}

static struct BSTreeNode * GetNodeFirstIn(const struct BSTreeNode *node) {
	assert(node);
	
	assert(!IsNodeOrphan(node));
	
	return GetNodeMin(node);
}

static struct BSTreeNode * GetNodeFirstPost(const struct BSTreeNode *node) {
	assert(node);
	
	assert(!IsNodeOrphan(node));
	
	while (GetNodeLeft(node)) { node = GetNodeLeft(node); }
	while (GetNodeRight(node)) { node = GetNodeRight(node); }
	
	if (IsNodeLeaf(node)) { return (struct BSTreeNode *)node; }
	
	return GetNodeFirstPost(node);
}

static struct BSTreeNode * GetNodeInSuccessor(const struct BSTreeNode *node) {
	assert(node);
	
	assert(!IsNodeOrphan(node));
	
	struct BSTreeNode *right = GetNodeRight(node);
	
	if (right) { return GetNodeMin(right); }
	
	struct BSTreeNode *parent = GetNodeParent(node);
	
	while (parent && GetNodeRight(parent) == node) {
		node = parent;
		
		parent = GetNodeParent(parent);
	}
	
	return parent;
}

static struct BSTreeNode * GetNodePostSuccessor(const struct BSTreeNode *node)
{
	assert(node);
	
	assert(!IsNodeOrphan(node));
	
	struct BSTreeNode *parent = GetNodeParent(node);
	
	if (!parent) { return NULL; } // radacina nu are succesor in post-ordine
	
	struct BSTreeNode *parentRight = GetNodeRight(parent);
	
	if (!parentRight || parentRight == node) {
		return parent;
	}
	
	struct BSTreeNode *curr = parentRight;
	
	while (GetNodeLeft(curr)) {
		curr = GetNodeLeft(curr);
	}
   
    return curr;
}

static struct Data * DestroyNode(struct BSTreeNode *node) {
	assert(node);
	
	// verifica daca se incearca distrugerea
	// unui nod care inca apartine unui arbore
	assert(IsNodeOrphan(node));
	
	struct Data *data = GetNodeData(node);
	
	free(node);
	
	return data;
}

static struct BSTreeNode * Node_Data_Unwrap(const struct Data *data) {
	assert(data);
	assert(data->mem);
	
	return *(struct BSTreeNode **)data->mem;
}

static size_t GetSize(const struct BSTree *tree) {
	assert(tree);
	
	return tree->size;
}

static struct BSTreeNode * GetRoot(const struct BSTree *tree) {
	assert(tree);
	
	return tree->root;
}

static void SetSize(struct BSTree *tree, size_t size) {
	assert(tree);
	
	tree->size = size;
}

static void SetRoot(struct BSTree *tree, struct BSTreeNode *root) {
	assert(tree);
	
	tree->root = root;
}

static struct BSTree * Create() {
	struct BSTree *tree = malloc(sizeof *tree);
	
	SetSize(tree, 0);
	
	SetRoot(tree, NULL);
	
	return tree;
}

static bool IsEmpty(const struct BSTree *tree) {
	assert(tree);
	
	return GetSize(tree) == 0;
}

static bool UNUSED ContainsNode(const struct BSTree *tree,
                                const struct BSTreeNode *node)
{
	assert(tree);
	assert(node);
	
	return GetNodeTree(node) == tree;
}

static struct BSTreeNode * ContainsData(const struct BSTree *tree,
                                        const struct Data *data,
                                        Data_comp_func_t comp_func)
{
	assert(tree);
	assert(data);
	assert(comp_func);
	
	if (IsEmpty(tree)) { return NULL; }
	
	struct BSTreeNode *node = GetRoot(tree);
	
	while (node) {
		if (comp_func(data, GetNodeData(node)) == 0) {
			return node;
		} else if (comp_func(data, GetNodeData(node)) == -1) {
			node = GetNodeLeft(node);
		} else if (comp_func(data, GetNodeData(node)) == 1) {
			node = GetNodeRight(node);
		}
	}
	
	return NULL;
}

static struct BSTreeNode * AddNode(struct BSTree *tree,
                                   struct BSTreeNode *node,
                                   Data_comp_func_t comp_func)
{
	assert(tree);
	assert(node);
	assert(comp_func);
	
	struct BSTreeNode *parent = GetRoot(tree);
	
	if (!parent) {
		SetRoot(tree, node);
	} else {
		while (true) {
			if (comp_func(GetNodeData(node),
			              GetNodeData(parent)) <= 0)
			{
				if (!GetNodeLeft(parent)) {
					SetNodeLeft(parent, node);
					
					break;
				}
				
				parent = GetNodeLeft(parent);
			} else if (comp_func(GetNodeData(node),
			                     GetNodeData(parent)) == 1)
			{
				if (!GetNodeRight(parent)) {
					SetNodeRight(parent, node);
					
					break;
				}
				
				parent = GetNodeRight(parent);
			}
		}
	}
	
	SetNodeParent(node, parent); // seteaza parintele nodului
	
	SetNodeTree(node, tree); // seteaza arborele caruia nodul apartine
	
	SetSize(tree, GetSize(tree) + 1);
	
	return node;
}

static struct BSTreeNode * AddData(struct BSTree *tree,
                                   const struct Data *data,
                                   Data_comp_func_t comp_func)
{
	assert(tree);
	assert(data);
	assert(comp_func);
	
	return AddNode(tree, CreateNode(data), comp_func);
}

static struct BSTreeNode * Copy_util(const struct BSTreeNode *refNode,
                                     struct BSTree *treeCopy)
{
	assert(refNode);
	assert(treeCopy);
	
	struct BSTreeNode *copyLeft = NULL;
	struct BSTreeNode *copyRight = NULL;
	
	struct BSTreeNode *refLeft = GetNodeLeft(refNode);
	if (refLeft) { copyLeft = Copy_util(refLeft, treeCopy); }
	
	struct BSTreeNode *refRight = GetNodeRight(refNode);
	if (refRight) { copyRight = Copy_util(refRight, treeCopy); }
	
	struct BSTreeNode *copy = CopyNode(refNode);
	
	SetNodeLeft(copy, copyLeft);
	SetNodeRight(copy, copyRight);
	
	SetNodeTree(copy, treeCopy);
	
	SetSize(treeCopy, GetSize(treeCopy) + 1);
	
	if (copyLeft) { SetNodeParent(copyLeft, copy); }
	if (copyRight) { SetNodeParent(copyRight, copy); }
	
	return copy;
}

static struct BSTree * Copy(const struct BSTree *tree) {
	assert(tree);
	
	struct BSTree *copy = Create();
	
	if (IsEmpty(tree)) { return copy; }
	
	struct BSTreeNode *root = Copy_util(GetRoot(tree), copy);
	
	SetRoot(copy, root);
	
	return copy;
}

static struct BSTree * CreateFromList(const struct List *list,
                                      Data_comp_func_t comp_func)
{
	assert(list);
	assert(comp_func);
	
	struct BSTree *copy = Create();
	
	if (List_IsEmpty(list)) { return copy; }
	
	const struct ListNode *listNode = List_GetFirstNode(list);
	
	while (listNode) {
		AddData(copy, ListNode_GetData(listNode), comp_func);
		
		listNode = ListNode_GetNext(listNode);
	}
	
	return copy;
}

static struct BSTree * Intersect(const struct BSTree *first,
                                 const struct BSTree *second,
                                 Data_comp_func_t comp_func)
{
	assert(first);
	assert(second);
	assert(comp_func);
	
	struct BSTree *intersection = Create();
	
	if (IsEmpty(first) || IsEmpty(second)) { return intersection; }
	
	struct BSTreeNode *firstNode = GetNodeFirstIn(GetRoot(first));
	struct BSTreeNode *secondNode = GetNodeFirstIn(GetRoot(second));
	
	while (firstNode && secondNode) {
		if (comp_func(GetNodeData(firstNode),
		              GetNodeData(secondNode)) == -1)
		{
			firstNode = GetNodeInSuccessor(firstNode);
			
			if (!firstNode) { break; }
		}
		else if (comp_func(GetNodeData(firstNode),
		                   GetNodeData(secondNode)) == 1)
		{
			secondNode = GetNodeInSuccessor(secondNode);
			
			if (!secondNode) { break; }
		}
		
		if (comp_func(GetNodeData(firstNode),
		              GetNodeData(secondNode)) == 0)
		{
			struct BSTreeNode *copy = CopyNode(firstNode);
			
			AddNode(intersection, copy, comp_func);
			
			firstNode = GetNodeInSuccessor(firstNode);
			secondNode = GetNodeInSuccessor(secondNode);
		}
	}
	
	return intersection;
}

static void Print(const struct BSTree *tree,
                  Data_print_func_t print_func,
                  const char *delim,
                  const char *endMark,
                  FILE *stream)
{
	assert(tree);
	assert(print_func);
	// delim poate fi NULL
	// endMark poate fi NULL
	assert(stream);
	
	if (IsEmpty(tree)) { goto print_end_mark; }
	
	struct List *nodeQueue = List_Create();
	
	struct BSTreeNode *root = GetRoot(tree);
	
	const struct Data rootWrapper =
		(struct Data){ &root, sizeof root };
	
	List_AddDataLast(nodeQueue, &rootWrapper);
	
	while (!List_IsEmpty(nodeQueue)) {
		struct Data *nodeWrapper =
			List_RemoveFirstNode(nodeQueue, true);
		
		struct BSTreeNode *node = Node_Data_Unwrap(nodeWrapper);
		
		Data_Destroy(nodeWrapper);
		
		print_func(GetNodeData(node), stream);
		
		struct BSTreeNode *left = GetNodeLeft(node);
		struct BSTreeNode *right = GetNodeRight(node);
		
		if (left) {
			const struct Data leftWrapper =
				(struct Data){ &left, sizeof left };
			
			List_AddDataLast(nodeQueue, &leftWrapper);
		}
		
		if (right) {
			const struct Data rightWrapper =
				(struct Data){ &right, sizeof right };
			
			List_AddDataLast(nodeQueue, &rightWrapper);
		}
		
		// daca lista e goala acesta e ultimul nod
		if (!List_IsEmpty(nodeQueue)) {
			if (delim) { fprintf(stream, "%s", delim); }
		}
	}
	
	List_Destroy(nodeQueue);
	
print_end_mark:
	if (endMark) { fprintf(stream, "%s", endMark); }
}

static void PrintWithChildren(const struct BSTree *tree,
                              Data_print_func_t print_func,
                              const char *delimChildren,
                              const char *delim,
                              const char *endMark,
                              FILE *stream)
{
	assert(tree);
	assert(print_func);
	// delimChildren poate fi si NULL
	// delim poate fi NULL
	// endMark poate fi NULL
	assert(stream);
	
	if (IsEmpty(tree)) { goto print_end_mark; }
	
	struct List *nodeQueue = List_Create();
	
	struct BSTreeNode *root = GetRoot(tree);
	
	const struct Data rootWrapper =
		(struct Data){ &root, sizeof root };
	
	List_AddDataLast(nodeQueue, &rootWrapper);
	
	while (!List_IsEmpty(nodeQueue)) {
		struct Data *nodeWrapper =
			List_RemoveFirstNode(nodeQueue, true);
		
		struct BSTreeNode *node = Node_Data_Unwrap(nodeWrapper);
		
		Data_Destroy(nodeWrapper);
		
		print_func(GetNodeData(node), stream);
		
		struct BSTreeNode *left = GetNodeLeft(node);
		struct BSTreeNode *right = GetNodeRight(node);
		
		if (delimChildren) { fprintf(stream, "%s", delimChildren); }
		
		if (left) {
			print_func(GetNodeData(left), stream);
			
			const struct Data leftWrapper =
				(struct Data){ &left, sizeof left };
			
			List_AddDataLast(nodeQueue, &leftWrapper);
		} else {
			fprintf(stream, "%p", NULL);
		}
		
		if (delimChildren) { fprintf(stream, "%s", delimChildren); }
		
		if (right) {
			print_func(GetNodeData(right), stream);
			
			const struct Data rightWrapper =
				(struct Data){ &right, sizeof right };
			
			List_AddDataLast(nodeQueue, &rightWrapper);
		} else {
			fprintf(stream, "%p", NULL);
		}
		
		// daca lista e goala acesta e ultimul nod
		if (!List_IsEmpty(nodeQueue)) {
			if (delim) { fprintf(stream, "%s", delim); }
		}
	}
	
	List_Destroy(nodeQueue);
	
print_end_mark:
	if (endMark) { fprintf(stream, "%s", endMark); }
}

static void PrintInOrder(const struct BSTree *tree,
                         Data_print_func_t print_func,
                         const char *delim,
                         const char *endMark,
                         FILE *stream)
{
	assert(tree);
	assert(print_func);
	// delim poate fi NULL
	// endMark poate fi NULL
	assert(stream);
	
	if (IsEmpty(tree)) { goto print_end_mark; }
	
	// afiseaza elementele din arbore in in-ordine
	
	struct List *nodeStack = List_Create();
	
	struct BSTreeNode *curr = GetRoot(tree);
	
	while (curr || !List_IsEmpty(nodeStack)) {
		while (curr) {
			const struct Data currWrapper =
				(struct Data){ &curr, sizeof curr };
			
			List_AddDataFirst(nodeStack, &currWrapper);
			
			curr = GetNodeLeft(curr);
		}
		
		if (!curr && !List_IsEmpty(nodeStack)) {
			struct Data *nodeWrapper =
				List_RemoveFirstNode(nodeStack, true);
			
			struct BSTreeNode *node = Node_Data_Unwrap(nodeWrapper);
			
			Data_Destroy(nodeWrapper);
			
			print_func(GetNodeData(node), stream);
			
			curr = GetNodeRight(node);
			
			if (curr || !List_IsEmpty(nodeStack)) {
				if (delim) { fprintf(stream, "%s", delim); }
			}
		}
	}
	
	List_Destroy(nodeStack);
	
print_end_mark:
	if (endMark) { fprintf(stream, "%s", endMark); }
}

static void SwapNodes(struct BSTree *tree,
                      struct BSTreeNode *first,
                      struct BSTreeNode *second)
{
	assert(tree);
	assert(first);
	assert(second);
	
	assert(ContainsNode(tree, first));
	assert(ContainsNode(tree, second));
	
	// https://stackoverflow.com/a/44568989/
	
	struct BSTreeNode *newFirstParent = GetNodeParent(second);
	struct BSTreeNode *newFirstLeft = GetNodeLeft(second);
	struct BSTreeNode *newFirstRight = GetNodeRight(second);
	
	struct BSTreeNode **newFirstLink = &tree->root;
	
	if (GetNodeParent(second)) {
		newFirstLink = GetNodeLeft(GetNodeParent(second)) == second ?
		               &GetNodeParent(second)->left :
		               &GetNodeParent(second)->right;
	}
	
	struct BSTreeNode *newSecondParent = GetNodeParent(first);
	struct BSTreeNode *newSecondLeft = GetNodeLeft(first);
	struct BSTreeNode *newSecondRight = GetNodeRight(first);
	
	struct BSTreeNode **newSecondLink = &(tree->root);
	
	if (GetNodeParent(first)) {
		newSecondLink = GetNodeLeft(GetNodeParent(first)) == first ?
		                &GetNodeParent(first)->left :
		                &GetNodeParent(first)->right;
	}
	
	if (GetNodeParent(second) == first) {
		newFirstParent = second;
		
		newFirstLink = NULL;
		
		if (GetNodeLeft(first) == second) {
			newSecondLeft = first;
		} else {
			newSecondRight = first;
		}
	} else if (GetNodeParent(first) == second) {
		newSecondParent = first;
		
		newSecondLink = NULL;
		
		if (GetNodeLeft(second) == first) {
			newFirstLeft = second;
		} else {
			newFirstRight = second;
		}
	}
	
	// actualizeaza legaturile pentru primul nod
	
	SetNodeParent(first, newFirstParent);
	
	SetNodeLeft(first, newFirstLeft);
	
	if (GetNodeLeft(first)) {
		SetNodeParent(GetNodeLeft(first), first);
	}
	
	SetNodeRight(first, newFirstRight);
	
	if (GetNodeRight(first)) {
		SetNodeParent(GetNodeRight(first), first);
	}
	
	if (newFirstLink) {
		*newFirstLink = first;
	}
	
	// actualizeaza legaturile pentru al doilea nod
	
	SetNodeParent(second, newSecondParent);
	
	SetNodeLeft(second, newSecondLeft);
	
	if (GetNodeLeft(second)) {
		SetNodeParent(GetNodeLeft(second), second);
	}
	
	SetNodeRight(second, newSecondRight);
	
	if (GetNodeRight(second)) {
		SetNodeParent(GetNodeRight(second), second);
	}
	
	if (newSecondLink) {
		*newSecondLink = second;
	}
}

static void UNUSED SwapNodesData(struct BSTree UNUSED *tree,
                                 struct BSTreeNode *first,
                                 struct BSTreeNode *second)
{
	assert(tree);
	assert(first);
	assert(second);
	
	assert(ContainsNode(tree, first));
	assert(ContainsNode(tree, second));
	
	struct Data *temp = GetNodeData(first);
	SetNodeData(first, GetNodeData(second));
	SetNodeData(second, temp);
}

static struct Data * RemoveNode(struct BSTree *tree,
                                struct BSTreeNode *node,
                                bool pop)
{
	assert(tree);
	assert(node);
	
	assert(ContainsNode(tree, node));
	
	struct BSTreeNode *left = GetNodeLeft(node);
	struct BSTreeNode *right = GetNodeRight(node);
	
	if (left && right) { // daca nodul are 2 copii
		// gaseste succesorul in-ordine al nodului
		struct BSTreeNode *succ = GetNodeInSuccessor(node);
		
		// adu-l pe succesor in locul nodului sters
		SwapNodes(tree, node, succ);
		// SwapNodesData(tree, node, succ);
		
		return RemoveNode(tree, node, pop);
		// return RemoveNode(tree, succ, pop);
	} else if (left) { // daca nodul are doar un copil stang
		struct BSTreeNode *parent = GetNodeParent(node);
		
		if (parent) { // daca nodul nu este radacina arobrelui
			if (node == GetNodeLeft(parent)) {
				SetNodeLeft(parent, left);
			} else if (node == GetNodeRight(parent)) {
				SetNodeRight(parent, left);
			}
		} else { // daca nodul este radacina arborelui
			SetRoot(tree, left);
		}
		
		SetNodeParent(left, parent);
	} else if (right) { // daca nodul are doar un copil drept
		struct BSTreeNode *parent = GetNodeParent(node);
		
		if (parent) { // daca nodul nu este radacina arobrelui
			if (node == GetNodeLeft(parent)) {
				SetNodeLeft(parent, right);
			} else if (node == GetNodeRight(parent)) {
				SetNodeRight(parent, right);
			}
		} else { // daca nodul este radacina arborelui
			SetRoot(tree, right);
		}
		
		SetNodeParent(right, parent);
	} else { // daca nodul nu are niciun copil
		struct BSTreeNode *parent = GetNodeParent(node);
		
		if (parent) { // daca nodul nu este radacina arobrelui
			
			if (node == GetNodeLeft(parent)) {
				SetNodeLeft(parent, NULL);
			} else if (node == GetNodeRight(parent)) {
				SetNodeRight(parent, NULL);
			}
		} else { // daca nodul este radacina arborelui
			SetRoot(tree, NULL);
		}
	}
	
	SetSize(tree, GetSize(tree) - 1);
	
	SetNodeLeft(node, NULL);
	SetNodeRight(node, NULL);
	SetNodeParent(node, NULL);
	
	SetNodeTree(node, NULL);
	
	struct Data *data = DestroyNode(node);
	
	// returneaza sau distruge datele ramase
	return pop ? data : (Data_Destroy(data), NULL);
}

static struct Data * RemoveData(struct BSTree *tree,
                                const struct Data *data,
                                Data_comp_func_t comp_func,
                                bool pop)
{
	assert(tree);
	assert(data);
	assert(comp_func);
	
	// cauta un nod care sa corespunda cu datele specificate
	struct BSTreeNode *node = ContainsData(tree, data, comp_func);
	
	return node ? RemoveNode(tree, node, pop) : NULL;
}

static void Clear(struct BSTree *tree) {
	assert(tree);
	
	if (IsEmpty(tree)) { return; }
	
	// distruge elementele din arbore in post-ordine
	// (pentru a distruge copii parintilor inainte de a distruge parintii)
	
	struct List *nodeStack = List_Create();
	
	struct BSTreeNode *lastVisited = NULL;
	
	struct BSTreeNode *curr = GetRoot(tree);
	
	while (curr) {
		const struct Data currWrapper =
			(struct Data){ &curr, sizeof curr };
	
		List_AddDataFirst(nodeStack, &currWrapper);
		
		curr = GetNodeLeft(curr);
	}
	
	while (!List_IsEmpty(nodeStack)) {
		struct Data *nodeWrapper =
			ListNode_GetData(List_GetFirstNode(nodeStack));
		
		struct BSTreeNode *node = Node_Data_Unwrap(nodeWrapper);
		
		if (!GetNodeRight(node) || GetNodeRight(node) == lastVisited) {
			RemoveNode(tree, node, false);
			
			lastVisited = node;
			
			List_RemoveFirstNode(nodeStack, false);
		} else {
			curr = GetNodeRight(node);
			
			while (curr) {
				const struct Data currWrapper =
					(struct Data){ &curr, sizeof curr };
			
				List_AddDataFirst(nodeStack, &currWrapper);
				
				curr = GetNodeLeft(curr);
			}
		}
	}
	
	List_Destroy(nodeStack);
}

static void Destroy(struct BSTree *tree) {
	assert(tree);
	
	// distruge toate nodurile arborelui (pentru eliberarea memoriei)
	Clear(tree);
	
	free(tree);
}

// PUBLIC

struct Data * BSTreeNode_GetData(const struct BSTreeNode *node) {
	assert(node);
	
	return GetNodeData(node);
}

struct BSTreeNode * BSTreeNode_GetLeft(const struct BSTreeNode *node) {
	assert(node);
	
	return GetNodeLeft(node);
}

struct BSTreeNode * BSTreeNode_GetRight(const struct BSTreeNode *node) {
	assert(node);
	
	return GetNodeRight(node);
}

struct BSTreeNode * BSTreeNode_GetParent(const struct BSTreeNode *node) {
	assert(node);
	
	return GetNodeParent(node);
}

bool BSTreeNode_IsLeaf(const struct BSTreeNode *node) {
	assert(node);
	
	return IsNodeLeaf(node);
}

struct BSTreeNode * BSTreeNode_GetMin(const struct BSTreeNode *node) {
	assert(node);
	
	return GetNodeMin(node);
}

struct BSTreeNode * BSTreeNode_GetFirstIn(const struct BSTreeNode *node) {
	assert(node);
	
	return GetNodeFirstIn(node);
}

struct BSTreeNode * BSTreeNode_GetFirstPost(const struct BSTreeNode *node) {
	assert(node);
	
	return GetNodeFirstPost(node);
}

struct BSTreeNode * BSTreeNode_GetInSuccessor(const struct BSTreeNode *node) {
	assert(node);
	
	return GetNodeInSuccessor(node);
}

struct BSTreeNode * BSTreeNode_GetPostSuccessor(const struct BSTreeNode *node)
{
	assert(node);
	
	return GetNodePostSuccessor(node);
}

size_t BSTree_GetSize(const struct BSTree *tree) {
	assert(tree);
	
	return GetSize(tree);
}

struct BSTreeNode * BSTree_GetRoot(const struct BSTree *tree) {
	assert(tree);
	
	return GetRoot(tree);
}

struct BSTree * BSTree_Create() {
	return Create();
}

bool BSTree_IsEmpty(const struct BSTree *tree) {
	assert(tree);
	
	return IsEmpty(tree);
}

struct BSTreeNode * BSTree_ContainsData(const struct BSTree *tree,
                                        const struct Data *data,
                                        Data_comp_func_t comp_func)
{
	assert(tree);
	assert(data);
	assert(comp_func);
	
	return ContainsData(tree, data, comp_func);
}

struct BSTreeNode * BSTree_AddData(struct BSTree *tree,
                                   const struct Data *data,
                                   Data_comp_func_t comp_func)
{
	assert(tree);
	assert(data);
	assert(comp_func);
	
	return AddData(tree, data, comp_func);
}

struct BSTree * BSTree_Copy(const struct BSTree *tree) {
	assert(tree);
	
	return Copy(tree);
}

struct BSTree * BSTree_CreateFromList(const struct List *list,
                                      Data_comp_func_t comp_func)
{
	assert(list);
	assert(comp_func);
	
	return CreateFromList(list, comp_func);
	
}

struct BSTree * BSTree_Intersect(const struct BSTree *first,
                                 const struct BSTree *second,
                                 Data_comp_func_t comp_func)
{
	assert(first);
	assert(second);
	assert(comp_func);
	
	return Intersect(first, second, comp_func);
}

void BSTree_Print(const struct BSTree *tree,
                  Data_print_func_t print_func,
                  const char *delim,
                  const char *endMark,
                  FILE *stream)
{
	assert(tree);
	assert(print_func);
	// delim poate fi NULL
	// endMark poate fi NULL
	assert(stream);
	
	Print(tree, print_func, delim, endMark, stream);
}

void BSTree_PrintWithChildren(const struct BSTree *tree,
                              Data_print_func_t print_func,
                              const char *delimChildren,
                              const char *delim,
                              const char *endMark,
                              FILE *stream)
{
	assert(tree);
	assert(print_func);
	// delimChildren poate fi si NULL
	// delim poate fi NULL
	// endMark poate fi NULL
	assert(stream);
	
	PrintWithChildren(tree, print_func, delimChildren, delim, endMark, stream);
}

void BSTree_PrintInOrder(const struct BSTree *tree,
                         Data_print_func_t print_func,
                         const char *delim,
                         const char *endMark,
                         FILE *stream)
{
	assert(tree);
	assert(print_func);
	// delim poate fi NULL
	// endMark poate fi NULL
	assert(stream);
	
	PrintInOrder(tree, print_func, delim, endMark, stream);
}

struct Data * BSTree_RemoveNode(struct BSTree *tree,
                                struct BSTreeNode *node,
                                bool pop)
{
	assert(tree);
	assert(node);
	
	return RemoveNode(tree, node, pop);
}

struct Data * BSTree_RemoveData(struct BSTree *tree,
                                const struct Data *data,
                                Data_comp_func_t comp_func,
                                bool pop)
{
	assert(tree);
	assert(data);
	assert(comp_func);
	
	return RemoveData(tree, data, comp_func, pop);
}

void BSTree_Clear(struct BSTree *tree) {
	assert(tree);
	
	Clear(tree);
}

void BSTree_Destroy(struct BSTree *tree) {
	assert(tree);
	
	Destroy(tree);
}
