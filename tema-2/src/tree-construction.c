#include "Data.h"
#include "List.h"
#include "Server.h"
#include "Tree.h"

#include <errno.h>
#include <error.h>
#include <stdio.h>
#include <stdlib.h>

static const char *TREE_IN_FILE_NAME = "tree.in";

static const char *TREE_OUT_FILE_NAME = "tree.out";

void tree_construction(struct Tree *tree) {
	// citeste serverele
	
	FILE *fin = fopen(TREE_IN_FILE_NAME, "r");
	
	if (!fin) {
		error(EXIT_FAILURE, errno,
		      "%s: Could not open file",
		      TREE_IN_FILE_NAME);
	}
	
	size_t serverCount;
	
	if (fscanf(fin, "%zu", &serverCount) != 1) {
		error(EXIT_FAILURE, errno,
		      "%s: Could not read server count",
		      TREE_IN_FILE_NAME);
	}
	
	// vector ce retine serverele citite
	struct Server **servers = malloc(serverCount * sizeof *servers);
	for (size_t i = 0; i < serverCount; ++i) { servers[i] = NULL; }
	
	// vector ce retine id-urile parintilor serverelor
	int *parentIds = malloc(serverCount * sizeof *parentIds);
	
	for (size_t i = 0; i < serverCount; ++i) {
		size_t id;
		
		if (fscanf(fin, "%zu", &id) != 1) {
			error(EXIT_FAILURE, errno,
			      "%s: Could not read server id",
			      TREE_IN_FILE_NAME);
		}
		
		struct Server *server = Server_Create(id);
		
		servers[i] = server;
		
		int parentId;
		
		if (fscanf(fin, "%d", &parentId) != 1) {
			error(EXIT_FAILURE, errno,
			      "%s: Could not read server parent id",
			      TREE_IN_FILE_NAME);
		}
		
		parentIds[id] = parentId;
		
		size_t addressCount;
		
		if (fscanf(fin, "%zu", &addressCount) != 1) {
			error(EXIT_FAILURE, errno,
			      "%s: Could not read server address count",
			      TREE_IN_FILE_NAME);
		}
		
		for (size_t j = 0; j < addressCount; ++j) {
			char *address;
			
			if (fscanf(fin, "%ms", &address) != 1) {
				error(EXIT_FAILURE, errno,
			      "%s: Could not read server address",
			      TREE_IN_FILE_NAME);
			}
			
			Server_AddAddress(server, address);
			
			free(address);
		}
		
		// daca acesta e serverul radacina, adauga-l in arbore
		if (parentId == -1) {
			const struct Data serverWrapper =
				(struct Data){ &servers[id], sizeof servers[id] };
			
			Tree_AddData(tree, &serverWrapper, NULL);
		}
	}
	
	if (fclose(fin) == EOF) {
		error(EXIT_FAILURE, errno,
		      "%s: Could not close file",
		      TREE_IN_FILE_NAME); 
	}
	
	if (Tree_IsEmpty(tree)) { return; }
	
	// construieste arborele
	// (serverul radacina a fost introdus la pasul anterior,
	//  adauga si restul serverelor)
	
	// coada a nodurilor ai caror copii asteapta sa fie adaugati in arbore
	struct List *nodeQueue = List_Create();
	
	struct TreeNode *root = Tree_GetRoot(tree);
	
	const struct Data rootWrapper =
		(struct Data){ &root, sizeof root };
	
	List_AddDataLast(nodeQueue, &rootWrapper);
	
	while (!List_IsEmpty(nodeQueue)) {
		struct Data *nodeWrapper =
			List_RemoveFirstNode(nodeQueue, true);
		
		struct TreeNode *node = TreeNode_Data_Unwrap(nodeWrapper);
		
		int nodeId = Server_Data_Unwrap(TreeNode_GetData(node))->id;
		
		for (size_t i = 0; i < serverCount; ++i) {
			if (parentIds[i] != nodeId) { continue; }
			
			const struct Server *server = servers[i];
			
			const struct Data serverWrapper =
				(struct Data){ &server, sizeof server };
			
			const struct TreeNode *child =
				Tree_AddData(tree, &serverWrapper, node);
			
			const struct Data childWrapper =
				(struct Data){ &child, sizeof child };
			
			List_AddDataLast(nodeQueue, &childWrapper);
		}
		
		Data_Destroy(nodeWrapper);
	}
	
	List_Destroy(nodeQueue);
	
	free(parentIds);
	
	free(servers);
	
	// afiseaza structura arborelui
	
	FILE *fout = fopen(TREE_OUT_FILE_NAME, "w");
	
	if (!fout) {
		error(EXIT_FAILURE, errno,
		      "%s: Could not open file",
		      TREE_OUT_FILE_NAME);
	}
	
	Tree_PrintWithChildren(tree,
	                       Server_Data_print_func_id,
	                       " ", "\n", "\n",
	                       fout);
	
	if (fclose(fout) == EOF) {
		error(EXIT_FAILURE, errno,
		      "%s: Could not close file",
		      TREE_OUT_FILE_NAME);
	}
}
