#include "Actor.h"
#include "Graph.h"
#include "List.h"
#include "Movie.h"

#include "helpers.h"

#include <errno.h>
#include <error.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define NDEBUG
#include <assert.h>

enum Task {
	TASK1, // determinarea numarului de productii independente
	
	TASK2, // evaluarea gradului de inrudire dintre 2 actori
	
	TASK3, // determinarea actorilor-puncte-de-articulatie
	
	BONUS, // determinarea tuturor clicilor
	
	UNKNOWN = -1,
};

static const char *SWITCHES[] = {
	"c1", // TASK1
	"c2", // TASK2
	"c3", // TASK3
	"b",  // BONUS
};

enum Task process_switch(const char *arg) {
	if (arg[0] != '-') { // verifica daca parametrul nu este switch
		error(EXIT_FAILURE, errno, "Unrecognized option '%s'", arg);
	}
	
	size_t skippedChars = strspn(arg, "-");
	if (skippedChars > 2) { skippedChars = 2; }
	
	enum Task task = UNKNOWN;
	
	// switch-ul fara liniile din fata
	const char *sw = arg + skippedChars;
	
	if (strcmp(sw, SWITCHES[TASK1]) == 0)
	{ task = TASK1; }
	
	else if (strcmp(sw, SWITCHES[TASK2]) == 0)
	{ task = TASK2; }
	
	else if (strcmp(sw, SWITCHES[TASK3]) == 0)
	{ task = TASK3; }
	
	else if (strcmp(sw, SWITCHES[BONUS]) == 0)
	{ task = BONUS; }
	
	else {
		error(EXIT_FAILURE, errno, "Unrecognized task '%s'", sw);
	}
	
	return task;
}

void graph_construction(struct Graph *graph, struct List *movieList);

size_t task1(struct Graph *graph);

size_t task2(struct Graph *graph,
             struct GraphNode *first,
             struct GraphNode *second);

struct List * task3(struct Graph *graph);

struct List * bonus(struct Graph *graph);

int main(int argc, const char *argv[]) {
	if (argc - 1 != 3) {
		error(EXIT_FAILURE, errno, "Invalid number of arguments");
	}
	
	enum Task task = process_switch(argv[1]);
	
	const char *inFileName = argv[2];
	const char *outFileName = argv[3];
	
	FILE *fin = fopen(inFileName, "r");
	
	if (!fin) {
		error(EXIT_FAILURE, errno,
		      "%s: Could not open file",
		      inFileName);
	}
	
	size_t movieCount;
	
	if (fscanf(fin, "%zu\n", &movieCount) != 1) {
		error(EXIT_FAILURE, errno, "Could not read movie count");
	}
	
	// lista ce retine actorii cititi
	struct List *actorList = List_Create();
	
	// lista ce retine filmele citite
	struct List *movieList = List_Create();
	
	for (size_t i = 0; i < movieCount; ++i) {
		char *movieName = fgetl(fin);
		
		if (!movieName) {
			error(EXIT_FAILURE, errno,
			      "Could not read movie %zu name", i);
		}
		
		struct Movie *movie = Movie_Create(movieName);
		
		const struct Data movieWrapper =
			(struct Data){ &movie, sizeof movie };
		
		List_AddDataLast(movieList, &movieWrapper);
		
		size_t actorCount;
		
		if (fscanf(fin, "%zu\n", &actorCount) != 1) {
			error(EXIT_FAILURE, errno,
			      "Could not read movie '%s' actor count", movieName);
		}
		
		for (size_t j = 0; j < actorCount; ++j) {
			char *actorName = fgetl(fin);
			
			if (!actorName) {
				error(EXIT_FAILURE, errno,
				      "Could not read movie '%s' actor name", movieName);
			}
			
			const struct Actor *refActor = &(struct Actor){ actorName, NULL };
			
			const struct Data refActorWrapper =
				(struct Data){ &refActor, sizeof refActor };
			
			struct ListNode *actorNode =
				List_ContainsData(actorList,
				                  &refActorWrapper,
				                  Actor_Data_comp_func_name);
			
			if (!actorNode) {
				struct Actor *actor = Actor_Create(actorName);
				
				const struct Data actorWrapper =
					(struct Data){ &actor, sizeof actor };
				
				actorNode = List_AddDataLast(actorList, &actorWrapper);
			}
			
			free(actorName);
			
			struct Actor *actor =
				Actor_Data_Unwrap(ListNode_GetData(actorNode));
			
			Movie_AddActor(movie, actor);
			
			// OPTIMIZATION: Actor_AddMovie(actor, movie);
			// (in tema nu este nevoie sa setezi filmele in care au jucat actorii)
		}
		
		free(movieName);
	}
	
	struct Graph *graph = Graph_Create(false);
	
	graph_construction(graph, movieList);
	
	FILE *fout = fopen(outFileName, "w");
	
	if (!fout) {
		error(EXIT_FAILURE, errno,
		      "%s: Could not open file",
		      outFileName);
	}
	
	switch (task) {
		case TASK1: {
			size_t productionCount = task1(graph);
			
			fprintf(fout, "%zu\n", productionCount);
			
			break;
		}
		
		case TASK2: {
			// citeste numele actorilor pentru care
			// sa se determine gradul de inrudire
			
			char *firstActorName = fgetl(fin);
			char *secondActorName = fgetl(fin);
			
			// gaseste nodurile actorilor in graf
			
			const struct Actor *refFirstActor =
				&(struct Actor){ firstActorName, NULL };
			
			const struct Data refFirstActorWrapper =
				(struct Data){ &refFirstActor, sizeof refFirstActor };
			
			const struct Actor *refSecondActor =
				&(struct Actor){ secondActorName, NULL };
			
			const struct Data refSecondActorWrapper =
				(struct Data){ &refSecondActor, sizeof refSecondActor };
			
			struct GraphNode *firstActorNode =
				Graph_ContainsData(graph,
				                   &refFirstActorWrapper,
				                   Actor_Data_comp_func_name);
			
			if (!firstActorNode) {
				error(EXIT_FAILURE, errno,
				      "%s: Actor with name '%s' does not exist",
				      inFileName, firstActorName);
			}
			
			struct GraphNode *secondActorNode =
				Graph_ContainsData(graph,
				                   &refSecondActorWrapper,
				                   Actor_Data_comp_func_name);
			
			if (!secondActorNode) {
				error(EXIT_FAILURE, errno,
				      "%s: Actor with name '%s' does not exist",
				      inFileName, secondActorName);
			}
			
			free(firstActorName);
			free(secondActorName);
			
			size_t degree = task2(graph, firstActorNode, secondActorNode);
			
			fprintf(fout, "%zu\n", degree);
			
			break;
		}
		
		case TASK3: {
			struct List *bridgeActorList = task3(graph);
			
			fprintf(fout, "%zu\n", List_GetSize(bridgeActorList));
			
			List_Print(bridgeActorList,
			           Actor_Data_print_func_name,
			           "\n", "\n",
			           fout);
			
			List_Destroy(bridgeActorList);
			
			break;
		}
		
		case BONUS: {
			struct List *maxActorClique = bonus(graph);
			
			fprintf(fout, "%zu\n", List_GetSize(maxActorClique));
			
			List_Print(maxActorClique,
			           Actor_Data_print_func_name,
			           "\n", "\n",
			           fout);
			
			List_Destroy(maxActorClique);
			
			break;
		}
		
		default:
			error(EXIT_FAILURE, errno, "Unknown task '%d'", task);
	}
	
	if (fclose(fin) == EOF) {
		error(EXIT_FAILURE, errno,
		      "%s: Could not close file",
		      inFileName); 
	}
	
	if (fclose(fout) == EOF) {
		error(EXIT_FAILURE, errno,
		      "%s: Could not close file",
		      outFileName);
	}
	
	Graph_Destroy(graph);
	
	List_Process(movieList, Movie_Data_proc_func_Destroy);
	
	List_Destroy(movieList);
	
	List_Process(actorList, Actor_Data_proc_func_Destroy);
	
	List_Destroy(actorList);
	
	return EXIT_SUCCESS;
}