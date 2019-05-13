#include "Tree.h"

#include "Data.h"
#include "List.h"

#include "helpers.h"

#include <stdbool.h>
#include <stdlib.h>

#define NDEBUG
#include <assert.h>

// PRIVATE

struct TreeNode {
	struct Data *data;
	
	struct List *children;
	struct TreeNode *parent;
	
	struct Tree *tree;
};

struct Tree {
	struct TreeNode *root;
};

static struct Data * GetNodeData(const struct TreeNode *node) {
	assert(node);
	
	return node->data;
}

static struct List * GetNodeChildren(const struct TreeNode *node) {
	assert(node);
	
	return node->children;
}

static struct TreeNode * GetNodeParent(const struct TreeNode *node) {
	assert(node);
	
	return node->parent;
}

static struct Tree * GetNodeTree(const struct TreeNode *node) {
	assert(node);
	
	return node->tree;
}

static void SetNodeData(struct TreeNode *node, const struct Data *data) {
	assert(node);
	
	node->data = (struct Data *)data;
}

static void SetNodeParent(struct TreeNode *node,
                          const struct TreeNode *parent)
{
	assert(node);
	
	node->parent = (struct TreeNode *)parent;
}

static void SetNodeTree(struct TreeNode *node, const struct Tree *tree) {
	assert(node);
	// tree poate fi si NULL
	
	node->tree = (struct Tree *)tree;
}

static struct TreeNode * CreateNode(const struct Data *data) {
	// data poate fi si NULL
	
	struct TreeNode *node = malloc(sizeof *node);
	
	SetNodeData(node, data ? Data_Copy(data) : NULL);
	
	node->children = List_Create();
	SetNodeParent(node, NULL);
	
	SetNodeTree(node, NULL);
	
	return node;
}

// static struct TreeNode * CopyNode(const struct TreeNode *node) {
	// assert(node);
	
	// return CreateNode(GetNodeData(node));
// }

static int Node_comp_func(const struct TreeNode *first,
                          const struct TreeNode *second)
{
	assert(first);
	assert(second);
	
	if (first < second) { return -1; }
	if (first > second) { return 1; }
	
	return 0;
}

static struct TreeNode * Node_Data_Unwrap(const struct Data *data) {
	assert(data);
	assert(data->mem);
	
	return *(struct TreeNode **)data->mem;
}

static int Node_Data_comp_func(const struct Data *first,
                               const struct Data *second)
{
	assert(first);
	assert(second);
	
	return Node_comp_func(Node_Data_Unwrap(first),
	                      Node_Data_Unwrap(second));
}

static struct TreeNode * GetRoot(const struct Tree *tree) {
	assert(tree);
	
	return tree->root;
}

static void SetRoot(struct Tree *tree, struct TreeNode *root) {
	assert(tree);
	// root poate fi si NULL
	
	tree->root = root;
}

static struct Tree * Create() {
	struct Tree *tree = malloc(sizeof *tree);
	
	SetRoot(tree, NULL);
	
	return tree;
}

static bool IsNodeRoot(const struct TreeNode *node) {
	assert(node);
	
	return GetNodeParent(node) == NULL;
}

static bool UNUSED IsNodeOrphan(const struct TreeNode *node) {
	assert(node);
	
	return GetNodeTree(node) == NULL;
}

static struct Data * DestroyNode(struct TreeNode *node) {
	assert(node);
	
	// verifica daca se incearca distrugerea
	// unui nod care inca apartine unui arbore
	assert(IsNodeOrphan(node));
	
	struct Data *data = GetNodeData(node);
	
	List_Destroy(GetNodeChildren(node));
	
	free(node);
	
	return data;
}

static bool IsEmpty(const struct Tree *tree) {
	assert(tree);
	
	return GetRoot(tree) == NULL;
}

static bool UNUSED ContainsNode(const struct Tree *tree,
                                const struct TreeNode *node)
{
	assert(tree);
	assert(node);
	
	return GetNodeTree(node) == tree;
}

static struct TreeNode * ContainsData(const struct Tree *tree,
                                      const struct Data *data,
                                      Data_comp_func_t comp_func)
{
	assert(tree);
	assert(data);
	assert(comp_func);
	
	if (IsEmpty(tree)) { return NULL; }
	
	struct List *nodeQueue = List_Create();
	
	struct TreeNode *root = GetRoot(tree);
	
	const struct Data rootWrapper =
		(struct Data){ &root, sizeof root };
	
	List_AddDataLast(nodeQueue, &rootWrapper);
	
	while (!List_IsEmpty(nodeQueue)) {
		struct Data *nodeWrapper =
			List_RemoveFirstNode(nodeQueue, true);
		
		struct TreeNode *node = Node_Data_Unwrap(nodeWrapper);
		
		Data_Destroy(nodeWrapper);
		
		if (comp_func(GetNodeData(node), data) == 0) {
			List_Destroy(nodeQueue);
			
			return node;
		}
		
		struct ListNode *child =
			List_GetFirstNode(GetNodeChildren(node));
		
		while (child) {
			List_AddNodeCopyLast(nodeQueue, child);
			
			child = ListNode_GetNext(child);
		}
	}
	
	List_Destroy(nodeQueue);
	
	return NULL;
}

static struct List * GetLeafNodes(const struct Tree *tree) {
	assert(tree);
	
	struct List *leaves = List_Create();
	
	if (IsEmpty(tree)) { return leaves; }
	
	struct List *nodeQueue = List_Create();
	
	struct TreeNode *root = GetRoot(tree);
	
	const struct Data rootWrapper =
		(struct Data){ &root, sizeof root };
	
	List_AddDataLast(nodeQueue, &rootWrapper);
	
	while (!List_IsEmpty(nodeQueue)) {
		struct ListNode *listNode = List_GetFirstNode(nodeQueue);
		
		struct TreeNode *node =
			Node_Data_Unwrap(ListNode_GetData(listNode));
		
		if (List_IsEmpty(GetNodeChildren(node))) {
			List_AddNodeCopyLast(leaves, listNode);
		} else {
			struct ListNode *child =
				List_GetFirstNode(GetNodeChildren(node));
			
			while (child) {
				List_AddNodeCopyLast(nodeQueue, child);
				
				child = ListNode_GetNext(child);
			}
		}
		
		List_RemoveFirstNode(nodeQueue, false);
	}
	
	List_Destroy(nodeQueue);
	
	return leaves;
}

static struct TreeNode * AddNode(struct Tree *tree,
                                 struct TreeNode *node,
                                 struct TreeNode *parent)
{
	assert(tree);
	assert(node);
	// parent poate fi si NULL
	
	if (!parent) { // daca inseram primul nod in arbore
		assert(IsEmpty(tree)); // verifica sa nu mai exista deja o radacina
		
		SetRoot(tree, node);
	} else {
		// verifica daca nodul parinte se afla in arbore
		assert(ContainsNode(tree, parent));
	}
	
	SetNodeParent(node, parent); // seteaza parintele nodului
	
	SetNodeTree(node, tree); // seteaza arborele caruia nodul apartine
	
	if (parent) {
		const struct Data nodeWrapper = (struct Data){ &node, sizeof node };
		
		List_AddDataLast(GetNodeChildren(parent), &nodeWrapper);
	}
	
	return node;
}

static struct TreeNode * AddData(struct Tree *tree,
                                 const struct Data *data,
                                 struct TreeNode *parent)
{
	assert(tree);
	assert(data);
	// parent poate fi si NULL
	
	return AddNode(tree, CreateNode(data), parent);
}

static void Process(struct Tree *tree,
                    Data_proc_func_t proc_func)
{
	assert(tree);
	assert(proc_func);
	
	if (IsEmpty(tree)) { return; }
	
	struct List *nodeQueue = List_Create();
	
	struct TreeNode *root = GetRoot(tree);
	
	const struct Data rootWrapper =
		(struct Data){ &root, sizeof root };
	
	List_AddDataLast(nodeQueue, &rootWrapper);
	
	while (!List_IsEmpty(nodeQueue)) {
		struct Data *nodeWrapper =
			List_RemoveFirstNode(nodeQueue, true);
		
		struct TreeNode *node = Node_Data_Unwrap(nodeWrapper);
		
		Data_Destroy(nodeWrapper);
		
		proc_func(GetNodeData(node));
		
		struct ListNode *child =
			List_GetFirstNode(GetNodeChildren(node));
		
		while (child) {
			List_AddNodeCopyLast(nodeQueue, child);
			
			child = ListNode_GetNext(child);
		}
	}
	
	List_Destroy(nodeQueue);
}

static void Print(const struct Tree *tree,
                  Data_print_func_t print_func,
                  const char *delim,
                  const char *endMark,
                  FILE *stream)
{
	assert(tree);
	assert(print_func);
	// delim poate fi si NULL
	// endMark poate fi si NULL
	assert(stream);
	
	if (IsEmpty(tree)) { return; }
	
	struct List *nodeQueue = List_Create();
	
	struct TreeNode *root = GetRoot(tree);
	
	const struct Data rootWrapper =
		(struct Data){ &root, sizeof root };
	
	List_AddDataLast(nodeQueue, &rootWrapper);
	
	while (!List_IsEmpty(nodeQueue)) {
		struct Data *nodeWrapper =
			List_RemoveFirstNode(nodeQueue, true);
		
		struct TreeNode *node = Node_Data_Unwrap(nodeWrapper);
		
		Data_Destroy(nodeWrapper);
		
		print_func(GetNodeData(node), stream);
		
		struct ListNode *child =
			List_GetFirstNode(GetNodeChildren(node));
		
		while (child) {
			List_AddNodeCopyLast(nodeQueue, child);
			
			child = ListNode_GetNext(child);
		}
		
		// daca lista e goala acesta e ultimul nod
		if (!List_IsEmpty(nodeQueue)) {
			if (delim) { fprintf(stream, "%s", delim); }
		}
	}
	
	if (endMark) { fprintf(stream, "%s", endMark); }
	
	List_Destroy(nodeQueue);
}
// TODO: add size field to Tree struct and change IsEmpty condition
static void PrintWithChildren(const struct Tree *tree,
                              Data_print_func_t print_func,
                              const char *delimChildren,
                              const char *delim,
                              const char *endMark,
                              FILE *stream)
{
	assert(tree);
	assert(print_func);
	assert(stream);
	
	if (IsEmpty(tree)) { return; }
	
	struct List *nodeQueue = List_Create();
	
	struct TreeNode *root = GetRoot(tree);
	
	const struct Data rootWrapper =
		(struct Data){ &root, sizeof root };
	
	List_AddDataLast(nodeQueue, &rootWrapper);
	
	while (!List_IsEmpty(nodeQueue)) {
		struct Data *nodeWrapper =
			List_RemoveFirstNode(nodeQueue, true);
		
		struct TreeNode *node = Node_Data_Unwrap(nodeWrapper);
		
		Data_Destroy(nodeWrapper);
		
		print_func(GetNodeData(node), stream);
		
		struct List *children = GetNodeChildren(node);
		
		if (!List_IsEmpty(children)) {
			if (delimChildren) { fprintf(stream, "%s", delimChildren); }
		}
		
		struct ListNode *listChild = List_GetFirstNode(children);
		
		while (listChild) {
			struct TreeNode *child =
				TreeNode_Data_Unwrap(ListNode_GetData(listChild));
			
			print_func(GetNodeData(child), stream);
			
			if (listChild != List_GetLastNode(children)) {
				if (delimChildren) { fprintf(stream, "%s", delimChildren); }
			}
			
			List_AddNodeCopyLast(nodeQueue, listChild);
			
			listChild = ListNode_GetNext(listChild);
		}
		
		// daca lista e goala acesta e ultimul nod
		if (!List_IsEmpty(nodeQueue)) {
			if (delim) { fprintf(stream, "%s", delim); }
		}
	}
	
	if (endMark) { fprintf(stream, "%s", endMark); }
	
	List_Destroy(nodeQueue);
}

static struct Data * RemoveNode(struct Tree *tree,
                                struct TreeNode *node,
                                bool pop)
{
	assert(tree);
	assert(node);
	
	assert(ContainsNode(tree, node));
	
	struct List *children = GetNodeChildren(node);
	
	if (IsNodeRoot(node)) {
		// nodul radacina poate fi sters doar daca
		// este singurul nod ramas in arbore
		assert(List_IsEmpty(children));
		
		SetRoot(tree, NULL);
	} else {
		struct TreeNode *parent = GetNodeParent(node);
		
		struct List *parentChildren = GetNodeChildren(parent);
		
		// sterge nodul din lista de copii a parintelui
		
		const struct Data nodeWrapper =
			(struct Data){ &node, sizeof node };
		
		List_RemoveData(parentChildren,
		                &nodeWrapper,
		                Node_Data_comp_func,
		                false);
		
		// seteaza noul parinte pentru copiii nodului si
		// adauga toti copiii nodului la parintele acestuia
		
		struct ListNode *listChild = List_GetFirstNode(children);
		
		while (listChild) {
			struct TreeNode *child =
				Node_Data_Unwrap(ListNode_GetData(listChild));
			
			List_AddNodeCopyLast(parentChildren, listChild);
			
			SetNodeParent(child, parent);
			
			listChild = ListNode_GetNext(listChild);
		}
	}
	
	SetNodeParent(node, NULL);
	List_Clear(children);
	
	SetNodeTree(node, NULL);
	
	struct Data *data = DestroyNode(node);
	
	// returneaza sau distruge datele ramase
	return pop ? data : (Data_Destroy(data), NULL);
}

// TODO: add RemoveData function, and also add it to Graph

static void Clear(struct Tree *tree) {
	assert(tree);
	
	if (IsEmpty(tree)) { return; }
	
	// adauga toate nodurile arborelui intr-o lista
	
	// creeaza lista care va contine toate nodurile
	struct List *nodes = List_Create();
	
	// creeaza lista folosita pentru parcurgerea arobrelui
	struct List *nodeQueue = List_Create();
	
	struct TreeNode *root = GetRoot(tree);
	
	const struct Data rootWrapper =
		(struct Data){ &root, sizeof root };
	
	List_AddDataLast(nodeQueue, &rootWrapper);
	
	while (!List_IsEmpty(nodeQueue)) {
		// copiaza nodul in lista de noduri
		List_AddNodeCopyFirst(nodes, List_GetFirstNode(nodeQueue));
		
		struct Data *nodeWrapper =
			List_RemoveFirstNode(nodeQueue, true);
		
		struct TreeNode *node = Node_Data_Unwrap(nodeWrapper);
		
		Data_Destroy(nodeWrapper);
		
		struct ListNode *child = List_GetFirstNode(GetNodeChildren(node));
		
		while (child) {
			List_AddNodeCopyLast(nodeQueue, child);
			
			child = ListNode_GetNext(child);
		}
	}
	
	List_Destroy(nodeQueue);
	
	// sterge toate nodurile de la frunza catre radacina
	
	while (!List_IsEmpty(nodes)) {
		struct Data *nodeWrapper = List_RemoveFirstNode(nodes, true);
		
		struct TreeNode *node = Node_Data_Unwrap(nodeWrapper);
		
		Data_Destroy(nodeWrapper);
		
		RemoveNode(tree, node, false);
	}
	
	List_Destroy(nodes);
	
	// SetRoot(tree, NULL); // TODO: check if needed
}

static void Destroy(struct Tree *tree) {
	assert(tree);
	
	// distruge toate nodurile arborelui (pentru eliberarea memoriei)
	Clear(tree);
	
	free(tree);
}

// PUBLIC

struct Data * TreeNode_GetData(const struct TreeNode *node) {
	assert(node);
	
	return GetNodeData(node);
}

const struct List * TreeNode_GetChildren(const struct TreeNode *node) {
	assert(node);
	
	return GetNodeChildren(node);
}

struct TreeNode * TreeNode_GetParent(const struct TreeNode *node) {
	assert(node);
	
	return GetNodeParent(node);
}

struct TreeNode * TreeNode_Data_Unwrap(const struct Data *data) {
	assert(data);
	
	return Node_Data_Unwrap(data);
}

struct TreeNode * Tree_GetRoot(const struct Tree *tree) {
	assert(tree);
	
	return GetRoot(tree);
}

struct Tree * Tree_Create() {
	return Create();
}

bool Tree_IsEmpty(const struct Tree *tree) {
	assert(tree);
	
	return IsEmpty(tree);
}

struct TreeNode * Tree_ContainsData(const struct Tree *tree,
                                    const struct Data *data,
                                    Data_comp_func_t comp_func)
{
	assert(tree);
	assert(data);
	assert(comp_func);
	
	return ContainsData(tree, data, comp_func);
}

struct List * Tree_GetLeafNodes(const struct Tree *tree) {
	assert(tree);
	
	return GetLeafNodes(tree);
}

struct TreeNode * Tree_AddData(struct Tree *tree,
                               const struct Data *data,
                               struct TreeNode *parent)
{
	assert(tree);
	assert(data);
	
	return AddData(tree, data, parent);
}

void Tree_Process(struct Tree *tree,
                  Data_proc_func_t proc_func)
{
	assert(tree);
	assert(proc_func);
	
	Process(tree, proc_func);
}

void Tree_Print(const struct Tree *tree,
                Data_print_func_t print_func,
                const char *delim,
                const char *endMark,
                FILE *stream)
{
	assert(tree);
	assert(print_func);
	assert(stream);
	
	Print(tree, print_func, delim, endMark, stream);
}

void Tree_PrintWithChildren(const struct Tree *tree,
                            Data_print_func_t print_func,
                            const char *delimChildren,
                            const char *delim,
                            const char *endMark,
                            FILE *stream)
{
	assert(tree);
	assert(print_func);
	assert(stream);
	
	PrintWithChildren(tree, print_func, delimChildren, delim, endMark, stream);
}

struct Data * Tree_RemoveNode(struct Tree *tree,
                              struct TreeNode *node,
                              bool pop)
{
	assert(tree);
	assert(node);
	
	return RemoveNode(tree, node, pop);
}

void Tree_Destroy(struct Tree *tree) {
	assert(tree);
	
	Destroy(tree);
}
