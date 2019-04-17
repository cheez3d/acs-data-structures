#include "Server.h"
#include "Tree.h"

#include <stdlib.h>

void tree_construction(struct Tree *tree);

void hierarchy_initialization(struct Tree *tree);

void query_processing(struct Tree *tree);

int main(void) {
	struct Tree *tree = Tree_Create();
	
	tree_construction(tree);
	
	hierarchy_initialization(tree);
	
	query_processing(tree);
	
	// distruge toate serverele din arbore
	Tree_Process(tree, Server_Data_proc_func_Destroy);
	
	Tree_Destroy(tree); // distruge arborele
	
	return EXIT_SUCCESS;
}
