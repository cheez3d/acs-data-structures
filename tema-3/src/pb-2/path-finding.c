#include "Data.h"
#include "Graph.h"
#include "Heap.h"
#include "List.h"
#include "Room.h"

#include <limits.h>

// #define NDEBUG
#include <assert.h>

enum Metadata {
	IS_VISITED,
	IS_DONE,
	TIME,
	PREV,
};

static int time_comp_func(const unsigned first, const unsigned second) {
	if (first < second) { return -1; }
	if (first > second) { return 1; }
	
	return 0;
}

static int GraphNode_time_comp_func(const struct GraphNode *first,
                                    const struct GraphNode *second)
{
	assert(first);
	assert(second);
	
	struct List *metadata = GraphNode_GetMetadata(first);
	
	struct Data *firstTimeWrapper =
		ListNode_GetData(List_GetNodeAtIndex(metadata, TIME));
	
	unsigned firstTime = **(unsigned **)firstTimeWrapper;
	
	metadata = GraphNode_GetMetadata(second);
	
	struct Data *secondTimeWrapper =
		ListNode_GetData(List_GetNodeAtIndex(metadata, TIME));
	
	unsigned secondTime = **(unsigned **)secondTimeWrapper;
	
	return time_comp_func(firstTime, secondTime);
}

static void GraphNode_time_update_func(struct GraphNode *node,
                                       const struct GraphNode *new)
{
	assert(node);
	
	struct List *metadata = GraphNode_GetMetadata(node);
	
	struct Data *timeWrapper =
		ListNode_GetData(List_GetNodeAtIndex(metadata, TIME));
	
	metadata = GraphNode_GetMetadata(new);
	
	struct Data *newTimeWrapper =
		ListNode_GetData(List_GetNodeAtIndex(metadata, TIME));
	
	unsigned newTime = **(unsigned **)newTimeWrapper;
	
	**(unsigned **)timeWrapper = newTime;
}

static int GraphNode_Data_time_comp_func(const struct Data *first,
                                         const struct Data *second)
{
	assert(first);
	assert(second);
	
	return GraphNode_time_comp_func(GraphNode_Data_Unwrap(first),
	                                GraphNode_Data_Unwrap(second));
}

static void GraphNode_Data_time_update_func(struct Data *data,
                                            const struct Data *new)
{
	assert(data);
	
	GraphNode_time_update_func(GraphNode_Data_Unwrap(data),
	                           GraphNode_Data_Unwrap(new));
}

void path_finding(struct Graph *graph,
                  struct GraphNode *startingRoomNode,
                  size_t exitCount,
                  struct List *out_path,
                  unsigned *out_time)
{
	struct Heap *priorityQueue =
		Heap_Create(GraphNode_Data_time_comp_func,
		            GraphNode_Data_time_update_func,
		            true);
	
	// initializeaza toate datele necesare algoritmului
	
	struct ListNode *listNode = List_GetFirstNode(Graph_GetNodes(graph));
	
	while (listNode) {
		struct Data *nodeWrapper = ListNode_GetData(listNode);
		struct GraphNode *node = GraphNode_Data_Unwrap(nodeWrapper);
		
		bool isVisited = false;
		
		const struct Data isVisitedWrapper =
			(struct Data){ &isVisited, sizeof isVisited };
		
		List_AddDataAtIndex(GraphNode_GetMetadata(node),
		                    &isVisitedWrapper,
		                    IS_VISITED);
		
		bool isDone = false;
		
		const struct Data isDoneWrapper =
			(struct Data){ &isDone, sizeof isDone };
		
		List_AddDataAtIndex(GraphNode_GetMetadata(node),
		                    &isDoneWrapper,
		                    IS_DONE);
		
		unsigned time = UINT_MAX;
		
		if (GraphNode_comp_func(node, startingRoomNode) == 0) {
			time = 0;
			
			// verifica daca trebuie asteptat in camera de inceput
			// pentru plecarea unui gardian
			
			struct Data *roomWrapper = GraphNode_GetData(startingRoomNode);
			
			struct Room *room = Room_Data_Unwrap(roomWrapper);
			
			if (!Room_IsExit(room)) {
				while (Room_ContainsGuardAtTime(room, time)) {
					++time;
				}
			}
			
			// adauga nodul de inceput in coada cu prioritati
			Heap_AddData(priorityQueue, nodeWrapper);
			
			// marcheaza nodul ca fiind vizitat
			
			struct List *metadata = GraphNode_GetMetadata(node);
			
			struct Data *isVisitedWrapper =
				ListNode_GetData(List_GetNodeAtIndex(metadata, IS_VISITED));
			
			**(bool **)isVisitedWrapper = true;
		}
		
		const struct Data timeWrapper = (struct Data){ &time, sizeof time };
		
		List_AddDataAtIndex(GraphNode_GetMetadata(node),
		                    &timeWrapper,
		                    TIME);
		
		struct GraphNode *prev = NULL;
		
		const struct Data prevWrapper = (struct Data){ &prev, sizeof prev };
		
		List_AddDataAtIndex(GraphNode_GetMetadata(node),
		                    &prevWrapper,
		                    PREV);
		
		listNode = ListNode_GetNext(listNode);
	}
	
	struct Heap *exitRoomNodeHeap =
		Heap_Create(GraphNode_Data_time_comp_func,
		            GraphNode_Data_time_update_func,
		            true);
	
	while (!Heap_IsEmpty(priorityQueue)) {
		struct Data *minTimeNodeWrapper = Heap_RemoveFirstData(priorityQueue);
		
		struct GraphNode *minTimeNode =
			GraphNode_Data_Unwrap(minTimeNodeWrapper);
		
		struct List *metadata = GraphNode_GetMetadata(minTimeNode);
		
		struct Data *isDoneWrapper =
			ListNode_GetData(List_GetNodeAtIndex(metadata, IS_DONE));
		
		**(bool **)isDoneWrapper = true;
		
		struct Data *minTimeWrapper =
			ListNode_GetData(List_GetNodeAtIndex(metadata, TIME));
		
		unsigned minTime = **(unsigned **)minTimeWrapper;
		
		struct Data *roomWrapper = GraphNode_GetData(minTimeNode);
			
		struct Room *room = Room_Data_Unwrap(roomWrapper);
		
		if (Room_IsExit(room)) {
			Heap_AddData(exitRoomNodeHeap, minTimeNodeWrapper);
		}
		
		Data_Destroy(minTimeNodeWrapper);
		
		if (Heap_GetSize(exitRoomNodeHeap) == exitCount) { break; }
		
		// exploreaza camerele vecine
		
		struct ListNode *neighborListNode =
			List_GetFirstNode(GraphNode_GetNeighbors(minTimeNode));
		
		while (neighborListNode) {
			struct Data *neighborWrapper =
				ListNode_GetData(neighborListNode);
			
			struct GraphNode *neighbor =
				GraphNode_Data_Unwrap(neighborWrapper);
			
			struct List *neighborMetadata = GraphNode_GetMetadata(neighbor);
			
			// avanseaza nodul din lista pentru
			// extrage ponderea legaturii intre cele 2 noduri
			neighborListNode = ListNode_GetNext(neighborListNode);
			
			struct Data *isNeighborDoneWrapper =
				ListNode_GetData(List_GetNodeAtIndex(neighborMetadata,
				                                     IS_DONE));
			
			bool isNeighborDone = **(bool **)isNeighborDoneWrapper;
			
			if (isNeighborDone) { goto skip_neighbor_node; }
			
			// extrage ponderea legaturii intre cele 2 noduri
			
			struct Data *portalTimeWrapper =
				ListNode_GetData(neighborListNode);
			
			unsigned portalTime = **(unsigned **)portalTimeWrapper;
			
			unsigned newTime = minTime + portalTime;
			
			struct Data *neighborRoomWrapper = GraphNode_GetData(neighbor);
			struct Room *neighborRoom = Room_Data_Unwrap(neighborRoomWrapper);
			
			// verifica daca trebuie asteptat in camera
			// pentru plecarea unui gardian
			if (!Room_IsExit(neighborRoom)) {
				while (Room_ContainsGuardAtTime(neighborRoom, newTime)) {
					++newTime;
				}
			}
			
			// verifica daca noul timp este mai optim decat
			// vechiul timp pana la camera vecina
			
			struct Data *neighborTimeWrapper =
				ListNode_GetData(List_GetNodeAtIndex(neighborMetadata, TIME));
			
			unsigned neighborTime = **(unsigned **)neighborTimeWrapper;
			
			if (time_comp_func(newTime, neighborTime) == -1) {
				struct Data *isNeighborVisitedWrapper =
					ListNode_GetData(List_GetNodeAtIndex(neighborMetadata,
					                                     IS_VISITED));
				
				bool isNeighborVisited = **(bool **)isNeighborVisitedWrapper;
				
				**(unsigned **)neighborTimeWrapper = newTime;
				
				if (!isNeighborVisited) {
					// daca nodul nu e inca in coada
					// adauga nodul camerei vecine in coada cu prioritati
					Heap_AddData(priorityQueue, neighborWrapper);
					
					// marcheaza nodul camerei vecine ca fiind vizitat
					// pentru a nu-l mai adauga din nou in coada
					**(bool **)isNeighborVisitedWrapper = true;
				} else {
					// actualizeaza timpul in coada cu prioritati
					
					Data_comp_func_t comp_func = GraphNode_Data_time_comp_func;
					
					// filtreaza in sus nodul in coada
					
					size_t i = Heap_ContainsData(priorityQueue,
					                             neighborWrapper,
					                             GraphNode_Data_comp_func);
					
					struct Data **container = Heap_GetContainer(priorityQueue);
					
					size_t parent_i = Heap_index_parent(i);
					
					while (i > 0 &&
					       comp_func(container[i], container[parent_i]) == -1)
					{
						struct Data *temp = container[i];
						container[i] = container[parent_i];
						container[parent_i] = temp;
						
						i = parent_i;
						parent_i = Heap_index_parent(i);
					}
				}
				
				struct Data *neighborPrevWrapper =
					ListNode_GetData(List_GetNodeAtIndex(neighborMetadata,
					                                     PREV));
				
				**(struct GraphNode ***)neighborPrevWrapper = minTimeNode;
			}
			
		skip_neighbor_node:
			neighborListNode = ListNode_GetNext(neighborListNode);
		}
	}
	
	Heap_Destroy(priorityQueue);
	
	// extrage iesirea pana la care drumul este cel mai scurt
	
	struct GraphNode *exitRoomNode =
		GraphNode_Data_Unwrap(Heap_GetFirstData(exitRoomNodeHeap));
	
	Heap_Destroy(exitRoomNodeHeap);
	
	// construieste drumul cel mai scurt
	
	struct GraphNode *node = exitRoomNode;
	
	while (node) {
		struct Data *roomWrapper = GraphNode_GetData(node);
		
		List_AddDataFirst(out_path, roomWrapper);
		
		struct List *metadata = GraphNode_GetMetadata(node);
		
		if (GraphNode_comp_func(node, exitRoomNode) == 0) {
			struct Data *timeWrapper =
				ListNode_GetData(List_GetNodeAtIndex(metadata, TIME));
			
			unsigned time = **(unsigned **)timeWrapper;
			
			*out_time = time;
		}
		
		struct Data *prevWrapper =
			ListNode_GetData(List_GetNodeAtIndex(metadata, PREV));
		
		struct GraphNode *prev = GraphNode_Data_Unwrap(prevWrapper);
		
		node = prev;
	}
	
	// sterge datele ce au fost folosite de algoritm
	Graph_ClearNodesMetadata(graph);
}
