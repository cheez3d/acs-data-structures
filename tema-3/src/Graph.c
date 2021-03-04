#include "Graph.h"

#include "Data.h"
#include "List.h"

#include "helpers.h"

#include <stdbool.h>
#include <stdlib.h>

#define NDEBUG
#include <assert.h>

// PRIVATE

struct GraphNode {
	struct Data *data;
	
	struct List *neighbors;
	
	struct List *metadata;
	
	struct Graph *graph;
};

struct Graph {
	struct List *nodes;
	
	bool isWeighted;
};

static struct Data * GetNodeData(const struct GraphNode *node) {
	assert(node);
	
	return node->data;
}

static struct List * GetNodeNeighbors(const struct GraphNode *node) {
	assert(node);
	
	return node->neighbors;
}

static struct List * GetNodeMetadata(const struct GraphNode *node) {
	assert(node);
	
	return node->metadata;
}

static struct Graph * GetNodeGraph(const struct GraphNode *node) {
	assert(node);
	
	return node->graph;
}

static void SetNodeData(struct GraphNode *node, const struct Data *data) {
	assert(node);
	
	node->data = (struct Data *)data;
}

static void SetNodeGraph(struct GraphNode *node, const struct Graph *graph) {
	assert(node);
	
	node->graph = (struct Graph *)graph;
}

static struct GraphNode * CreateNode(const struct Data *data) {
	struct GraphNode *node = malloc(sizeof *node);
	
	SetNodeData(node, data ? Data_Copy(data) : NULL);
	
	node->neighbors = List_Create();
	
	node->metadata = List_Create();
	
	SetNodeGraph(node, NULL);
	
	return node;
}

// static struct GraphNode * CopyNode(const struct GraphNode *node) {
	// assert(node);
	
	// return CreateNode(GetNodeData(node));
// }

static int Node_comp_func(const struct GraphNode *first,
                          const struct GraphNode *second)
{
	assert(first);
	assert(second);
	
	if (first < second) { return -1; }
	if (first > second) { return 1; }
	
	return 0;
}

static struct GraphNode * Node_Data_Unwrap(const struct Data *data) {
	assert(data);
	assert(data->mem);
	
	return *(struct GraphNode **)data->mem;
}

static int Node_Data_comp_func(const struct Data *first,
                               const struct Data *second)
{
	assert(first);
	assert(second);
	
	return Node_comp_func(Node_Data_Unwrap(first),
	                      Node_Data_Unwrap(second));
}

static struct List * GetNodes(const struct Graph *graph) {
	assert(graph);
	
	return graph->nodes;
}

static bool IsWeighted(const struct Graph *graph) {
	assert(graph);
	
	return graph->isWeighted;
}

static struct Graph * Create(bool isWeighted) {
	struct Graph *graph = malloc(sizeof *graph);
	
	graph->nodes = List_Create();
	
	graph->isWeighted = isWeighted;
	
	return graph;
}

static bool UNUSED IsNodeOrphan(const struct GraphNode *node) {
	assert(node);
	
	return GetNodeGraph(node) == NULL;
}

static struct Data * DestroyNode(struct GraphNode *node) {
	assert(node);
	
	// verifica daca se incearca distrugerea
	// unui nod care inca apartine unui graf
	assert(IsNodeOrphan(node));
	
	struct Data *data = GetNodeData(node);
	
	List_Destroy(GetNodeNeighbors(node));
	
	List_Destroy(node->metadata);
	
	free(node);
	
	return data;
}

static bool IsEmpty(const struct Graph *graph) {
	assert(graph);
	
	return List_IsEmpty(GetNodes(graph));
}

static bool UNUSED ContainsNode(const struct Graph *graph,
                                const struct GraphNode *node)
{
	assert(graph);
	assert(node);
	
	return GetNodeGraph(node) == graph;
}

static struct GraphNode * ContainsData(const struct Graph *graph,
                                       const struct Data *data,
                                       Data_comp_func_t comp_func)
{
	assert(graph);
	assert(data);
	assert(comp_func);
	
	if (IsEmpty(graph)) { return NULL; }
	
	struct ListNode *listNode = List_GetFirstNode(GetNodes(graph));
	
	while (listNode) {
		struct GraphNode *node = Node_Data_Unwrap(ListNode_GetData(listNode));
		
		if (comp_func(GetNodeData(node), data) == 0) {
			return node;
		}
		
		listNode = ListNode_GetNext(listNode);
	}
	
	return NULL;
}

static struct GraphNode * AddNode(struct Graph *graph,
                                  struct GraphNode *node)
{
	assert(graph);
	assert(node);
	
	SetNodeGraph(node, graph); // seteaza graful caruia nodul apartine
	
	const struct Data nodeWrapper = (struct Data){ &node, sizeof node };
	
	List_AddDataLast(GetNodes(graph), &nodeWrapper);
	
	return node;
}

static struct GraphNode * AddData(struct Graph *graph,
                                  const struct Data *data)
{
	assert(graph);
	assert(data);
	
	return AddNode(graph, CreateNode(data));
}

static bool ContainsOrientedLink(const struct Graph *graph,
                                 const struct GraphNode *src,
                                 const struct GraphNode *dest)
{
	assert(graph);
	assert(src);
	assert(dest);
	
	const struct Data destWrapper = (struct Data){ &dest, sizeof dest };
	
	struct List *srcNeighbors = GetNodeNeighbors(src);
	
	return List_ContainsDataStep(srcNeighbors,
	                             &destWrapper,
	                             Node_Data_comp_func,
	                             IsWeighted(graph) ? 2 : 1);
}

static bool ContainsLink(const struct Graph *graph,
                         const struct GraphNode *first,
                         const struct GraphNode *second)
{
	assert(graph);
	assert(first);
	assert(second);
	
	return ContainsOrientedLink(graph, first, second) &&
	       ContainsOrientedLink(graph, second, first);
}

static void AddOrientedLink(struct Graph *graph,
                            struct GraphNode *src,
                            struct GraphNode *dest)
{
	assert(graph);
	assert(src);
	assert(dest);
	
	assert(!IsWeighted(graph));
	
	struct List *srcNeighbors = GetNodeNeighbors(src);
	
	// // verifica daca exista deja o legatura de la un nod la celalalt
	// assert(!ContainsOrientedLink(graph, src, dest));
	
	if (!ContainsOrientedLink(graph, src, dest)) {
		const struct Data destWrapper = (struct Data){ &dest, sizeof dest };
		
		// adauga dest in lista de vecini a lui src
		List_AddDataLast(srcNeighbors, &destWrapper);
	}
}

static void AddLink(struct Graph *graph,
                    struct GraphNode *first,
                    struct GraphNode *second)
{
	assert(graph);
	assert(first);
	assert(second);
	
	assert(!IsWeighted(graph));
	
	AddOrientedLink(graph, first, second);
	AddOrientedLink(graph, second, first);
}

static void AddWeightedOrientedLink(struct Graph UNUSED *graph,
                                    struct GraphNode *src,
                                    struct GraphNode *dest,
                                    unsigned weight)
{
	assert(graph);
	assert(src);
	assert(dest);
	
	assert(IsWeighted(graph));
	
	const struct Data destWrapper = (struct Data){ &dest, sizeof dest };
	
	struct List *srcNeighbors = GetNodeNeighbors(src);
	
	// // verifica daca exista deja o legatura de la un nod la celalalt
	// assert(!ContainsLink(graph, src, dest));
	
	struct ListNode *destListNode =
		List_ContainsDataStep(srcNeighbors,
		                      &destWrapper,
		                      Node_Data_comp_func,
		                      2);
	
	if (!destListNode) {
		// adauga dest in lista de vecini a lui src
		List_AddDataLast(srcNeighbors, &destWrapper);
		
		// adauga ponderea pe urmatoarea pozitie in lista
		
		const struct Data weightWrapper = (struct Data){ &weight, sizeof weight };
		
		List_AddDataLast(srcNeighbors, &weightWrapper);
	} else {
		// se seteaza noua pondere daca este mai mica ca cea precedenta
		
		struct ListNode *currentWeightListNode =
			ListNode_GetNext(destListNode);
		
		struct Data *currentWeightWrapper =
			ListNode_GetData(currentWeightListNode);
		
		unsigned currentWeight = **(unsigned **)currentWeightWrapper;
		
		if (weight < currentWeight) {
			**(unsigned **)currentWeightWrapper = weight;
		}
	}
}

static void AddWeightedLink(struct Graph *graph,
                            struct GraphNode *first,
                            struct GraphNode *second,
                            unsigned weight)
{
	assert(graph);
	assert(first);
	assert(second);
	
	assert(IsWeighted(graph));
	
	AddWeightedOrientedLink(graph, first, second, weight);
	AddWeightedOrientedLink(graph, second, first, weight);
}

static size_t GetConnectedComponentCount(struct Graph *graph) {
	assert(graph);
	
	if (IsEmpty(graph)) { return 0; }
	
	// marcheaza toate nodurile ca fiind nevizitate
	
	struct ListNode *listNode = List_GetFirstNode(GetNodes(graph));
	
	while (listNode) {
		struct Data *nodeWrapper = ListNode_GetData(listNode);
		struct GraphNode *node = Node_Data_Unwrap(nodeWrapper);
		
		bool isVisited = false;
		
		const struct Data isVisitedWrapper =
			(struct Data){ &isVisited, sizeof isVisited };
		
		List_AddDataLast(GetNodeMetadata(node), &isVisitedWrapper);
		
		listNode = ListNode_GetNext(listNode);
	}
	
	// determina numarul de componente conexe
	
	size_t connectedComponentCount = 0;
	
	struct List *nodeQueue = List_Create();
	
	listNode = List_GetFirstNode(GetNodes(graph));
	
	while (listNode) {
		struct Data *nodeWrapper = ListNode_GetData(listNode);
		struct GraphNode *node = Node_Data_Unwrap(nodeWrapper);
		
		struct List *metadata = GetNodeMetadata(node);
		
		struct Data *isVisitedWrapper =
			ListNode_GetData(List_GetLastNode(metadata));
		
		bool isVisited = **(bool **)isVisitedWrapper;
		
		if (isVisited) { goto skip_node; }
		else { ++connectedComponentCount; }
		
		List_AddDataLast(nodeQueue, nodeWrapper);
		
		while (!List_IsEmpty(nodeQueue)) {
			struct Data *processedNodeWrapper =
				List_RemoveFirstNode(nodeQueue, true);
			
			struct GraphNode *processedNode =
				Node_Data_Unwrap(processedNodeWrapper);
			
			// marcheaza nodul ca fiind vizitat
			
			metadata = GetNodeMetadata(processedNode);
			
			isVisitedWrapper =
				ListNode_GetData(List_GetLastNode(metadata));
			
			**(bool **)isVisitedWrapper = true;
			
			// exploreaza vecinii nodului procesat
			
			struct ListNode *neighborListNode =
				List_GetFirstNode(GetNodeNeighbors(processedNode));
			
			while (neighborListNode) {
				struct Data *neighborWrapper =
					ListNode_GetData(neighborListNode);
				
				struct GraphNode *neighbor =
					Node_Data_Unwrap(neighborWrapper);
				
				metadata = GetNodeMetadata(neighbor);
				
				isVisitedWrapper =
					ListNode_GetData(List_GetLastNode(metadata));
				
				isVisited = **(bool **)isVisitedWrapper;
				
				if (isVisited) { goto skip_neighbor_node; }
				
				List_AddDataLast(nodeQueue, neighborWrapper);
				
			skip_neighbor_node:
				neighborListNode = ListNode_GetNext(neighborListNode);
			}
			
			Data_Destroy(processedNodeWrapper);
		}
		
	skip_node:
		listNode = ListNode_GetNext(listNode);
	}
	
	List_Destroy(nodeQueue);
	
	// sterge marcajele
	
	listNode = List_GetFirstNode(GetNodes(graph));
	
	while (listNode) {
		struct Data *nodeWrapper = ListNode_GetData(listNode);
		struct GraphNode *node = Node_Data_Unwrap(nodeWrapper);
		
		List_RemoveLastNode(GetNodeMetadata(node), false);
		
		listNode = ListNode_GetNext(listNode);
	}
	
	return connectedComponentCount;
}

static void Process(struct Graph *graph,
                    Data_proc_func_t proc_func)
{
	assert(graph);
	assert(proc_func);
	
	struct ListNode *listNode = List_GetFirstNode(GetNodes(graph));
	
	while (listNode) {
		struct GraphNode *node = Node_Data_Unwrap(ListNode_GetData(listNode));
		
		proc_func(GetNodeData(node));
		
		listNode = ListNode_GetNext(listNode);
	}
}

static void ClearNodesMetadata(struct Graph *graph) {
	struct ListNode *listNode = List_GetFirstNode(GetNodes(graph));
	
	while (listNode) {
		struct Data *nodeWrapper = ListNode_GetData(listNode);
		struct GraphNode *node = Node_Data_Unwrap(nodeWrapper);
		
		List_Clear(GetNodeMetadata(node));
		
		listNode = ListNode_GetNext(listNode);
	}
}

static struct Data * RemoveNode(struct Graph *graph,
                                struct GraphNode *node,
                                bool pop)
{
	assert(graph);
	assert(node);
	
	assert(ContainsNode(graph, node));
	
	struct List *neighbors = GetNodeNeighbors(node);
	
	// sterge nodul din listele de adiacenta ale vecinilor
	// (sterge legaturile nodului cu toate celelalte noduri
	//  din graf cu care acesta este legat)
	
	const struct Data nodeWrapper = (struct Data){ &node, sizeof node };
	
	struct ListNode *neighborListNode = List_GetFirstNode(neighbors);
	
	while (neighborListNode) {
		struct GraphNode *neighbor =
			Node_Data_Unwrap(ListNode_GetData(neighborListNode));
		
		if (Node_comp_func(neighbor, node) == 0) {
			goto skip_neighbor_node;
		}
		
		struct List *neighborNeighbors = GetNodeNeighbors(neighbor);
		
		struct ListNode *listNode =
			List_ContainsDataStep(neighborNeighbors,
			                      &nodeWrapper,
			                      Node_Data_comp_func,
			                      IsWeighted(graph) ? 2 : 1);
		if (neighbor == node) fprintf(stderr, "==\n");
		// daca exista o legatura
		if (listNode) {
			if (IsWeighted(graph)) {
				// sterge ponderea legaturii
				List_RemoveNode(neighborNeighbors,
				                ListNode_GetNext(listNode),
				                false);
			}
			
			// sterge legatura propriu-zisa a vecinului
			List_RemoveNode(neighborNeighbors, listNode, false);
		}
		
	skip_neighbor_node:
		if (IsWeighted(graph)) {
			neighborListNode = ListNode_GetNext(neighborListNode);
		}
		
		neighborListNode = ListNode_GetNext(neighborListNode);
	}
	
	List_Clear(neighbors);
	
	SetNodeGraph(node, NULL);
	
	struct Data *data = DestroyNode(node);
	
	// returneaza sau distruge datele ramase
	return pop ? data : (Data_Destroy(data), NULL);
}

static void Clear(struct Graph *graph) {
	assert(graph);
	
	if (IsEmpty(graph)) { return; }
	
	struct List *nodes = GetNodes(graph);
	
	// sterge toate nodurile grafului
	
	while (!List_IsEmpty(nodes)) {
		struct Data *nodeWrapper = List_RemoveFirstNode(nodes, true);
		
		struct GraphNode *node = Node_Data_Unwrap(nodeWrapper);
		
		Data_Destroy(nodeWrapper);
		
		RemoveNode(graph, node, false);
	}
}

static void Destroy(struct Graph *graph) {
	assert(graph);
	
	// distruge toate nodurile grafului
	// (pentru eliberarea memoriei)
	Clear(graph);
	
	List_Destroy(GetNodes(graph));
	
	free(graph);
}

// PUBLIC

struct Data * GraphNode_GetData(const struct GraphNode *node) {
	assert(node);
	
	return GetNodeData(node);
}

const struct List * GraphNode_GetNeighbors(const struct GraphNode *node) {
	assert(node);
	
	return GetNodeNeighbors(node);
}

struct List * GraphNode_GetMetadata(const struct GraphNode *node) {
	assert(node);
	
	return GetNodeMetadata(node);
}

int GraphNode_comp_func(const struct GraphNode *first,
                        const struct GraphNode *second)
{
	assert(first);
	assert(second);
	
	return Node_comp_func(first, second);
}

struct GraphNode * GraphNode_Data_Unwrap(const struct Data *data) {
	assert(data);
	
	return Node_Data_Unwrap(data);
}

int GraphNode_Data_comp_func(const struct Data *first,
                             const struct Data *second)
{
	assert(first);
	assert(second);
	
	return Node_Data_comp_func(first, second);
}

struct List * Graph_GetNodes(const struct Graph *graph) {
	assert(graph);
	
	return GetNodes(graph);
}

struct Graph * Graph_Create(bool isWeighted) {
	return Create(isWeighted);
}

bool Graph_IsEmpty(const struct Graph *graph) {
	assert(graph);
	
	return IsEmpty(graph);
}

struct GraphNode * Graph_ContainsData(const struct Graph *graph,
                                      const struct Data *data,
                                      Data_comp_func_t comp_func)
{
	assert(graph);
	assert(data);
	assert(comp_func);
	
	return ContainsData(graph, data, comp_func);
}

struct GraphNode * Graph_AddData(struct Graph *graph,
                                 const struct Data *data)
{
	assert(graph);
	assert(data);
	
	return AddData(graph, data);
}

bool Graph_ContainsOrientedLink(const struct Graph *graph,
                                const struct GraphNode *src,
                                const struct GraphNode *dest)
{
	assert(graph);
	assert(src);
	assert(dest);
	
	return ContainsOrientedLink(graph, src, dest);
}

bool Graph_ContainsLink(const struct Graph *graph,
                        const struct GraphNode *first,
                        const struct GraphNode *second)
{
	assert(graph);
	assert(first);
	assert(second);
	
	return ContainsLink(graph, first, second);
}

void Graph_AddOrientedLink(struct Graph *graph,
                           struct GraphNode *src,
                           struct GraphNode *dest)
{
	assert(graph);
	assert(src);
	assert(dest);
	
	AddOrientedLink(graph, src, dest);
}

void Graph_AddLink(struct Graph *graph,
                   struct GraphNode *first,
                   struct GraphNode *second)
{
	assert(graph);
	assert(first);
	assert(second);
	
	AddLink(graph, first, second);
}

void Graph_AddWeightedOrientedLink(struct Graph *graph,
                                   struct GraphNode *src,
                                   struct GraphNode *dest,
                                   unsigned weight)
{
	assert(graph);
	assert(src);
	assert(dest);
	
	AddWeightedOrientedLink(graph, src, dest, weight);
}

void Graph_AddWeightedLink(struct Graph *graph,
                           struct GraphNode *first,
                           struct GraphNode *second,
                           unsigned weight)
{
	assert(graph);
	assert(first);
	assert(second);
	
	AddWeightedLink(graph, first, second, weight);
}

size_t Graph_GetConnectedComponentCount(struct Graph *graph) {
	assert(graph);
	
	return GetConnectedComponentCount(graph);
}

void Graph_Process(struct Graph *graph,
                   Data_proc_func_t proc_func)
{
	assert(graph);
	assert(proc_func);
	
	Process(graph, proc_func);
}

void Graph_ClearNodesMetadata(struct Graph *graph) {
	assert(graph);
	
	ClearNodesMetadata(graph);
}



void Graph_Clear(struct Graph *graph) {
	assert(graph);
	
	Clear(graph);
}

void Graph_Destroy(struct Graph *graph) {
	assert(graph);
	
	Destroy(graph);
}
