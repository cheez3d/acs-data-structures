#include "Graph.h"
#include "List.h"
#include "Room.h"

#include <stdlib.h>

struct GraphNode * graph_construction(struct Graph *graph,
                                      struct Room **rooms,
                                      size_t roomCount,
                                      struct List **roomPortalLists,
                                      size_t startingRoomId)
{
	// creeaza nodurile camerelor in graf
	
	struct GraphNode **roomNodes = malloc(roomCount * sizeof *roomNodes);
	
	for (size_t i = 0; i < roomCount; ++i) {
		const struct Data roomWrapper =
			(struct Data){ &rooms[i], sizeof rooms[i] };
		
		roomNodes[i] = Graph_AddData(graph, &roomWrapper);
	}
	
	// fa legaturile intre camere
	
	for (size_t i = 0; i < roomCount; ++i) {
		struct GraphNode *firstRoomNode = roomNodes[i];
		
		struct List *firstRoomPortalList = roomPortalLists[i];
		
		while (!List_IsEmpty(firstRoomPortalList)) {
			struct Data *secondRoomIdWrapper =
				List_RemoveFirstNode(firstRoomPortalList, true);
			
			size_t secondRoomId = **(size_t **)secondRoomIdWrapper;
			
			Data_Destroy(secondRoomIdWrapper);
			
			struct Data *timeWrapper =
				List_RemoveFirstNode(firstRoomPortalList, true);
			
			unsigned time = **(unsigned **)timeWrapper;
			
			Data_Destroy(timeWrapper);
			
			struct GraphNode *secondRoomNode = roomNodes[secondRoomId - 1];
			
			Graph_AddWeightedLink(graph,
			                      firstRoomNode,
			                      secondRoomNode,
			                      time);
		}
	}
	
	struct GraphNode *startingRoomNode = roomNodes[startingRoomId - 1];
	
	free(roomNodes);
	
	return startingRoomNode;
}