#include "Graph.h"
#include "List.h"
#include "Room.h"

#include <errno.h>
#include <error.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

static const char *IN_FILE_NAME = "labirint.in";

static const char *OUT_FILE_NAME = "labirint.out";

struct GraphNode * graph_construction(struct Graph *graph,
                                      struct Room **rooms,
                                      size_t roomCount,
                                      struct List **roomPortalLists,
                                      size_t startingRoomId);

void path_finding(struct Graph *graph,
                  struct GraphNode *startingRoomNode,
                  size_t exitCount,
                  struct List *out_path,
                  unsigned *out_time);

int main(void) {
	FILE *fin = fopen(IN_FILE_NAME, "r");
	
	if (!fin) {
		error(EXIT_FAILURE, errno,
		      "%s: Could not open file",
		      IN_FILE_NAME);
	}
	
	size_t roomCount;
	
	if (fscanf(fin, "%zu", &roomCount) != 1) {
		error(EXIT_FAILURE, errno,
		      "%s: Could not read room count",
		      IN_FILE_NAME);
	}
	
	size_t portalCount;
	
	if (fscanf(fin, "%zu", &portalCount) != 1) {
		error(EXIT_FAILURE, errno,
		      "%s: Could not read portal count",
		      IN_FILE_NAME);
	}
	
	size_t startingRoomId;
	
	if (fscanf(fin, "%zu", &startingRoomId) != 1) {
		error(EXIT_FAILURE, errno,
		      "%s: Could not read starting room id",
		      IN_FILE_NAME);
	}
	
	// citeste legaturile intre camere sub forma de indici
	
	struct List **roomPortalLists =
		malloc(roomCount * sizeof *roomPortalLists);
	
	for (size_t i = 0; i < roomCount; ++i) {
		roomPortalLists[i] = List_Create();
	}
	
	for (size_t i = 0; i < portalCount; ++i) {
		size_t firstRoomId;
		
		if (fscanf(fin, "%zu", &firstRoomId) != 1) {
			error(EXIT_FAILURE, errno,
			      "%s: Could not read portal '%zu' first room id",
			      IN_FILE_NAME, i + 1);
		}
		
		size_t secondRoomId;
		
		if (fscanf(fin, "%zu", &secondRoomId) != 1) {
			error(EXIT_FAILURE, errno,
			      "%s: Could not read portal '%zu' second room id",
			      IN_FILE_NAME, i + 1);
		}
		
		unsigned time;
		
		if (fscanf(fin, "%u", &time) != 1) {
			error(EXIT_FAILURE, errno,
			      "%s: Could not read portal '%zu' time",
			      IN_FILE_NAME, i + 1);
		}
		
		struct List *firstRoomPortalList = roomPortalLists[firstRoomId - 1];
		
		const struct Data secondRoomIdWrapper =
			(struct Data){ &secondRoomId, sizeof secondRoomId };
		
		List_AddDataLast(firstRoomPortalList, &secondRoomIdWrapper);
		
		const struct Data timeWrapper =
			(struct Data){ &time, sizeof time };
		
		List_AddDataLast(firstRoomPortalList, &timeWrapper);
	}
	
	// citeste si creeaza camerele
	
	struct Room **rooms = malloc(roomCount * sizeof *rooms);
	
	size_t exitCount = 0;
	
	for (size_t i = 0; i < roomCount; ++i) {
		char isExitChar;
		
		if (fscanf(fin, "\n%c", &isExitChar) != 1) {
			error(EXIT_FAILURE, errno,
			      "%s: Could not read wether room '%zu' is exit or not",
			      IN_FILE_NAME, i + 1);
		}
		
		bool isExit = (isExitChar == 'Y');
		
		size_t guardTimeCount;
	
		if (fscanf(fin, "%zu", &guardTimeCount) != 1) {
			error(EXIT_FAILURE, errno,
			      "%s: Could not read room '%zu' guard time count",
			      IN_FILE_NAME, i + 1);
		}
		
		unsigned *guardTimes = malloc(guardTimeCount * sizeof *guardTimes);
		
		for (size_t j = 0; j < guardTimeCount; ++j) {
			if (fscanf(fin, "%u", guardTimes + j) != 1) {
				error(EXIT_FAILURE, errno,
					  "%s: Could not read portal '%zu' time",
					  IN_FILE_NAME, i + 1);
			}
		}
		
		rooms[i] = Room_Create(i + 1, isExit, guardTimes, guardTimeCount);
		
		if (isExit) { ++exitCount; }
	}
	
	if (fclose(fin) == EOF) {
		error(EXIT_FAILURE, errno,
		      "%s: Could not close file",
		      IN_FILE_NAME); 
	}
	
	struct Graph *graph = Graph_Create(true);
	
	struct GraphNode *startingRoomNode =
		graph_construction(graph,
		                   rooms,
		                   roomCount,
		                   roomPortalLists,
		                   startingRoomId);
	
	free(rooms);
	
	for (size_t i = 0; i < roomCount; ++i) {
		List_Destroy(roomPortalLists[i]);
	}
	
	free(roomPortalLists);
	
	struct List *path = List_Create();
	unsigned time = UINT_MAX;
	
	path_finding(graph, startingRoomNode, exitCount, path, &time);
	
	FILE *fout = fopen(OUT_FILE_NAME, "w");
	
	if (!fout) {
		error(EXIT_FAILURE, errno,
		      "%s: Could not open file",
		      OUT_FILE_NAME);
	}
	
	if (!List_IsEmpty(path)) {
		fprintf(fout, "%u\n", time);
		
		List_Print(path, Room_Data_print_func_id, " ", "\n", fout);
	} else {
		fprintf(fout, "-1\n");
	}
	
	if (fclose(fout) == EOF) {
		error(EXIT_FAILURE, errno,
		      "%s: Could not close file",
		      OUT_FILE_NAME); 
	}
	
	List_Destroy(path);
	
	Graph_Process(graph, Room_Data_proc_func_Destroy);
	
	Graph_Destroy(graph);
	
	return EXIT_SUCCESS;
}
