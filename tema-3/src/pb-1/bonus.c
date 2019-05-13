#include "Actor.h"
#include "BSTree.h"
#include "Data.h"
#include "Graph.h"
#include "Heap.h"
#include "List.h"

#include <stdbool.h>

#define NDEBUG
#include <assert.h>

int List_comp_func_size(const struct List *first, const struct List *second) {
	assert(first);
	assert(second);
	
	size_t firstSize = List_GetSize(first);
	size_t secondSize = List_GetSize(second);
	
	if (firstSize < secondSize) { return -1; }
	if (firstSize > secondSize) { return 1; }
	
	return 0;
}

void List_proc_func_Destroy(struct List *list) {
	List_Destroy(list);
}

struct List * List_Data_Unwrap(const struct Data *data) {
	assert(data);
	
	return **(struct List ***)data;
}

int List_Data_comp_func_size(const struct Data *first,
                             const struct Data *second)
{
	assert(first);
	assert(second);
	
	return List_comp_func_size(List_Data_Unwrap(first),
	                           List_Data_Unwrap(second));
}

void List_Data_proc_func_Destroy(struct Data *data) {
	assert(data);
	
	List_proc_func_Destroy(List_Data_Unwrap(data));
}



void GraphNode_Data_print_func(const struct Data *data, FILE *stream) {
	assert(data);
	
	fprintf(stream, "%p", **(struct GraphNode ***)data);
}



void bron_kerbosch(struct BSTree *R,
                   struct BSTree *P,
                   struct BSTree *X,
                   struct Heap *out_cliques)
{
	// daca a fost gasita o clica
	if (BSTree_IsEmpty(P) && BSTree_IsEmpty(X)) {
		struct List *clique = List_CreateFromBSTree(R);
		
		const struct Data cliqueWrapper =
			(struct Data){ &clique, sizeof clique };
		
		Heap_AddData(out_cliques,
		             &cliqueWrapper,
		             List_Data_comp_func_size);
		
		return; 
	}
	
	if (BSTree_IsEmpty(P)) { return; }
	
	struct BSTreeNode *nodeTreeNode =
		BSTreeNode_GetFirstPost(BSTree_GetRoot(P));
	
	while (nodeTreeNode) {
		struct Data *nodeWrapper = BSTreeNode_GetData(nodeTreeNode);
		struct GraphNode *node = GraphNode_Data_Unwrap(nodeWrapper);
		
		BSTree_AddData(R, nodeWrapper, GraphNode_Data_comp_func);
		
		struct BSTree *neighborsTree =
			BSTree_CreateFromList(GraphNode_GetNeighbors(node),
			                      GraphNode_Data_comp_func);
		
		struct BSTree *P1 = BSTree_Intersect(P,
		                                     neighborsTree,
		                                     GraphNode_Data_comp_func);
		
		struct BSTree *X1 = BSTree_Intersect(X,
		                                     neighborsTree,
		                                     GraphNode_Data_comp_func);
		
		bron_kerbosch(R, P1, X1, out_cliques);
		
		BSTree_Destroy(neighborsTree);
		
		BSTree_Destroy(P1);
		BSTree_Destroy(X1);
		
		BSTree_AddData(X, nodeWrapper, GraphNode_Data_comp_func);
		
		BSTree_RemoveData(R, nodeWrapper, GraphNode_Data_comp_func, false);
		
		struct BSTreeNode *succ = BSTreeNode_GetPostSuccessor(nodeTreeNode);
		
		BSTree_RemoveNode(P, nodeTreeNode, false);
		
		nodeTreeNode = succ;
	}
}

struct List * bonus(struct Graph *graph) {
	struct Heap *nodeCliques = Heap_Create(false);
	
	struct BSTree *R = BSTree_Create(); 
	struct BSTree *P = BSTree_CreateFromList(Graph_GetNodes(graph),
	                                         GraphNode_Data_comp_func);
	struct BSTree *X = BSTree_Create();
	
	bron_kerbosch(R, P, X, nodeCliques);
	
	BSTree_Destroy(R);
	BSTree_Destroy(P);
	BSTree_Destroy(X);
	
	struct Data *maxNodeCliqueWrapper =
		Heap_RemoveFirstData(nodeCliques,
		                     List_Data_comp_func_size);
	
	struct List *maxNodeClique = List_Data_Unwrap(maxNodeCliqueWrapper);
	
	Data_Destroy(maxNodeCliqueWrapper);
	
	Heap_Process(nodeCliques, List_Data_proc_func_Destroy);
	
	Heap_Destroy(nodeCliques);
	
	// inlocuieste in lista nodurile grafului cu actorii asociati acestora
	
	size_t nodeCount = List_GetSize(maxNodeClique);
	
	for (size_t i = 0; i < nodeCount; ++i) {
		struct Data *nodeWrapper = List_RemoveFirstNode(maxNodeClique, true);
		
		struct GraphNode *node = GraphNode_Data_Unwrap(nodeWrapper);
		
		List_AddDataLast(maxNodeClique, GraphNode_GetData(node));
		
		Data_Destroy(nodeWrapper);
	}
	
	struct List *maxActorClique = maxNodeClique;
	
	List_Sort(maxActorClique, Actor_Data_comp_func_name);
	
	return maxActorClique;
}
