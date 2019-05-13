#include "Actor.h"
#include "Graph.h"
#include "List.h"
#include "Movie.h"

void graph_construction(struct Graph *graph,
                        struct List *movieList)
{
	struct ListNode *movieListNode = List_GetFirstNode(movieList);
	
	while (movieListNode) {
		struct Movie *movie =
			Movie_Data_Unwrap(ListNode_GetData(movieListNode));
		
		struct List *actorList = movie->actorList;
		
		struct ListNode *actorListNode = List_GetFirstNode(actorList);
		
		// adauga actorii pentru filmul curent in graf
		
		struct List *actorNodeList = List_Create();
		
		while (actorListNode) {
			struct GraphNode *actorNode =
				Graph_ContainsData(graph,
				                   ListNode_GetData(actorListNode),
				                   Actor_Data_comp_func_name);
			
			if (!actorNode) {
				actorNode =
					Graph_AddData(graph, ListNode_GetData(actorListNode));
			}
			
			const struct Data actorNodeWrapper =
				(struct Data){ &actorNode, sizeof actorNode };
			
			List_AddDataLast(actorNodeList, &actorNodeWrapper);
			
			actorListNode = ListNode_GetNext(actorListNode);
		}
		
		// fa legaturile intre actori
		
		struct ListNode *actorNodeListNode =
			List_GetFirstNode(actorNodeList);
		
		while (actorNodeListNode != List_GetLastNode(actorNodeList)) {
			const struct Data *actorNodeWrapper =
				ListNode_GetData(actorNodeListNode);
			
			struct GraphNode *actorNode =
				GraphNode_Data_Unwrap(actorNodeWrapper);
			
			struct ListNode *otherActorNodeListNode =
				ListNode_GetNext(actorNodeListNode);
			
			while (otherActorNodeListNode) {
				const struct Data *otherActorNodeWrapper =
					ListNode_GetData(otherActorNodeListNode);
				
				struct GraphNode *otherActorNode =
					GraphNode_Data_Unwrap(otherActorNodeWrapper);
				
				const struct Data *actorWrapper =
					GraphNode_GetData(actorNode);
				
				const struct Data *otherActorWrapper =
					GraphNode_GetData(otherActorNode);
				
				if (Actor_Data_comp_func_name(actorWrapper,
				                              otherActorWrapper) == 0)
				{
					goto skip_node;
				}
				
				if (!Graph_ContainsLink(graph,
				                        actorNode,
				                        otherActorNode))
				{
					Graph_AddLink(graph, actorNode, otherActorNode);
				}
			
			skip_node:
				otherActorNodeListNode =
					ListNode_GetNext(otherActorNodeListNode);
			}
			
			actorNodeListNode = ListNode_GetNext(actorNodeListNode);
		}
		
		List_Destroy(actorNodeList);
		
		movieListNode = ListNode_GetNext(movieListNode);
	}
}
