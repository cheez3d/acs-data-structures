#include "Graph.h"
#include "List.h"

#include <stdbool.h>

size_t task2(struct Graph *graph,
             struct GraphNode *first,
             struct GraphNode *second)
{
	// marcheaza toate nodurile grafului ca avand gradul 0
	
	struct ListNode *listNode = List_GetFirstNode(Graph_GetNodes(graph));
	
	while (listNode) {
		struct Data *nodeWrapper = ListNode_GetData(listNode);
		struct GraphNode *node = GraphNode_Data_Unwrap(nodeWrapper);
		
		size_t degree = 0;
		
		const struct Data degreeWrapper =
			(struct Data){ &degree, sizeof degree };
		
		List_AddDataLast(GraphNode_GetMetadata(node), &degreeWrapper);
		
		listNode = ListNode_GetNext(listNode);
	}
	
	// determina gradul lui second
	
	struct List *nodeQueue = List_Create();
	
	const struct Data *firstWrapper =
		&(struct Data){ &first, sizeof first };
	
	List_AddDataLast(nodeQueue, firstWrapper);
	
	while (!List_IsEmpty(nodeQueue)) {
		struct Data *nodeWrapper = List_RemoveFirstNode(nodeQueue, true);
		
		struct GraphNode *node = GraphNode_Data_Unwrap(nodeWrapper);
		
		struct ListNode *neighborListNode =
			List_GetFirstNode(GraphNode_GetNeighbors(node));
		
		while (neighborListNode) {
			struct Data *neighborWrapper = ListNode_GetData(neighborListNode);
			
			struct GraphNode *neighbor =
				GraphNode_Data_Unwrap(neighborWrapper);
			
			// verifica daca vecinul a mai fost vizitat
			
			if (GraphNode_comp_func(neighbor, first) == 0) { goto skip_neighbor_node; }
			
			struct List *neighborMetadata = GraphNode_GetMetadata(neighbor);
			
			struct Data *neighborDegreeWrapper =
				ListNode_GetData(List_GetLastNode(neighborMetadata));
			
			size_t neighborDegree = **(size_t **)neighborDegreeWrapper;
			
			if (neighborDegree != 0) { goto skip_neighbor_node; }
			
			// seteaza gradul vecinului
			
			struct List *metadata = GraphNode_GetMetadata(node);
			
			struct Data *degreeWrapper =
				ListNode_GetData(List_GetLastNode(metadata));
			
			size_t degree = **(size_t **)degreeWrapper;
			
			**(size_t **)neighborDegreeWrapper = degree + 1;
			
			// daca a fost gasit un drum de la first la second
			if (GraphNode_comp_func(neighbor, second) == 0) {
				Data_Destroy(nodeWrapper);
				
				goto finish;
			}
			
			List_AddDataLast(nodeQueue, neighborWrapper);
			
		skip_neighbor_node:
			neighborListNode = ListNode_GetNext(neighborListNode);
		}
		
		Data_Destroy(nodeWrapper);
	}
	
finish:
	List_Destroy(nodeQueue);
	
	struct List *metadata = GraphNode_GetMetadata(second);
	
	struct Data *degreeWrapper =
		ListNode_GetData(List_GetLastNode(metadata));
	
	size_t degree = **(size_t **)degreeWrapper;
	
	Graph_ClearNodesMetadata(graph);
	
	return degree;
}