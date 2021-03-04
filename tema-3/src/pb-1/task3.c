#include "Actor.h"
#include "Graph.h"
#include "List.h"

enum Metadata {
	IS_VISITED,
	DISCOVERY_TIME,
	LOW_TIME,
	PARENT,
};

void tarjan(struct GraphNode *node,
            unsigned *time,
            struct List *out_bridgeNodeList)
{
	size_t childCount = 0;
	
	// marcheaza nodul ca fiind vizitat
	
	struct List *metadata = GraphNode_GetMetadata(node);
	
	struct Data *isVisitedWrapper =
		ListNode_GetData(List_GetNodeAtIndex(metadata, IS_VISITED));
	
	**(bool **)isVisitedWrapper = true;
	
	// initializeaza timpii
	
	struct Data *discoveryTimeWrapper =
		ListNode_GetData(List_GetNodeAtIndex(metadata, DISCOVERY_TIME));
	
	struct Data *lowTimeWrapper =
		ListNode_GetData(List_GetNodeAtIndex(metadata, LOW_TIME));
	
	**(unsigned **)discoveryTimeWrapper =
	**(unsigned **)lowTimeWrapper = ++*time;
	
	struct ListNode *neighborListNode =
		List_GetFirstNode(GraphNode_GetNeighbors(node));
	
	while (neighborListNode) {
		struct Data *neighborWrapper =
			ListNode_GetData(neighborListNode);
		
		struct GraphNode *neighbor =
			GraphNode_Data_Unwrap(neighborWrapper);
		
		struct List *neighborMetadata =
			GraphNode_GetMetadata(neighbor);
		
		struct Data *isNeighborVisitedWrapper =
			ListNode_GetData(List_GetNodeAtIndex(neighborMetadata,
			                                     IS_VISITED));
		
		bool isNeighborVisited = **(bool **)isNeighborVisitedWrapper;
		
		struct Data *parentWrapper =
			ListNode_GetData(List_GetNodeAtIndex(metadata, PARENT));
		
		struct GraphNode *parent = GraphNode_Data_Unwrap(parentWrapper);
		
		unsigned lowTime = **(unsigned **)lowTimeWrapper;
		
		struct Data *neighborDiscoveryTimeWrapper =
			ListNode_GetData(List_GetNodeAtIndex(neighborMetadata,
			                                     DISCOVERY_TIME));
		
		unsigned neighborDiscoveryTime =
			**(unsigned **)neighborDiscoveryTimeWrapper;
		
		if (!isNeighborVisited) {
			++childCount;
			
			struct Data *neighborParentWrapper =
				ListNode_GetData(List_GetNodeAtIndex(neighborMetadata,
				                                     PARENT));
			
			**(struct GraphNode ***)neighborParentWrapper = node;
			
			tarjan(neighbor, time, out_bridgeNodeList);
			
			struct Data *neighborLowTimeWrapper =
				ListNode_GetData(List_GetNodeAtIndex(neighborMetadata,
				                                     LOW_TIME));
			
			unsigned neighborLowTime = **(unsigned **)neighborLowTimeWrapper;
			
			**(unsigned **)lowTimeWrapper =
				lowTime < neighborLowTime ? lowTime : neighborLowTime;
			
			unsigned discoveryTime = **(unsigned **)discoveryTimeWrapper;
			
			
			
			if ((!parent && childCount > 1) ||
			    (parent && neighborLowTime >= discoveryTime))
			{
				const struct Data nodeWrapper =
					(struct Data){ &node, sizeof node };
				
				List_AddDataLast(out_bridgeNodeList, &nodeWrapper);
			}
		} else if (!parent || GraphNode_comp_func(neighbor, parent) != 0) {
			**(unsigned **)lowTimeWrapper = lowTime < neighborDiscoveryTime ?
			                                lowTime :
			                                neighborDiscoveryTime;
		}
		
		neighborListNode = ListNode_GetNext(neighborListNode);
	}
}

struct List * task3(struct Graph *graph) {
	struct List *bridgeNodeList = List_Create();
	
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
		
		unsigned discoveryTime = 0;
		
		const struct Data discoveryTimeWrapper =
			(struct Data){ &discoveryTime, sizeof discoveryTime };
		
		List_AddDataAtIndex(GraphNode_GetMetadata(node),
		                    &discoveryTimeWrapper,
		                    DISCOVERY_TIME);
		
		unsigned lowTime = 0;
		
		const struct Data lowTimeWrapper =
			(struct Data){ &lowTime, sizeof lowTime };
		
		List_AddDataAtIndex(GraphNode_GetMetadata(node),
		                    &lowTimeWrapper,
		                    LOW_TIME);
		
		struct GraphNode *parent = NULL;
		
		const struct Data parentWrapper =
			(struct Data){ &parent, sizeof parent };
		
		List_AddDataAtIndex(GraphNode_GetMetadata(node),
		                    &parentWrapper,
		                    PARENT);
		
		listNode = ListNode_GetNext(listNode);
	}
	
	// ruleaza algoritmul pentru determinarea nodurilor critice
	
	unsigned time = 0;
	
	listNode = List_GetFirstNode(Graph_GetNodes(graph));
	
	while (listNode) {
		struct Data *nodeWrapper = ListNode_GetData(listNode);
		struct GraphNode *node = GraphNode_Data_Unwrap(nodeWrapper);
		
		struct List *metadata = GraphNode_GetMetadata(node);
		
		struct Data *isVisitedWrapper =
			ListNode_GetData(List_GetLastNode(metadata));
		
		bool isVisited = **(bool **)isVisitedWrapper;
		
		if (isVisited) { goto skip_node; }
		
		tarjan(node, &time, bridgeNodeList);
		
	skip_node:
		listNode = ListNode_GetNext(listNode);
	}
	
	// sterge datele ce au fost folosite de algoritm
	Graph_ClearNodesMetadata(graph);
	
	// inlocuieste in lista nodurile critice
	// ale grafului cu actorii asociati acestora
	
	size_t bridgeNodeCount = List_GetSize(bridgeNodeList);
	
	for (size_t i = 0; i < bridgeNodeCount; ++i) {
		struct Data *nodeWrapper = List_RemoveFirstNode(bridgeNodeList, true);
		
		struct GraphNode *node = GraphNode_Data_Unwrap(nodeWrapper);
		
		List_AddDataLast(bridgeNodeList, GraphNode_GetData(node));
		
		Data_Destroy(nodeWrapper);
	}
	
	struct List *bridgeActorList = bridgeNodeList;
	
	List_Sort(bridgeActorList, Actor_Data_comp_func_name);
	
	return bridgeActorList;
}
