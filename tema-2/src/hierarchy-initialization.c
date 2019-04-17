#include "Data.h"
#include "List.h"
#include "Server.h"
#include "Tree.h"

#include <errno.h>
#include <error.h>
#include <stdio.h>
#include <stdlib.h>

static const char *HIERARCHY_OUT_FILE_NAME = "hierarchy.out";

void hierarchy_initialization(struct Tree *tree) {
	// mosteneste adresele copiilor pentru toate serverele
	// (de la frunze catre radacina)
	
	// obtine o lista cu frunzele arborelui
	struct List *leaves = Tree_GetLeafNodes(tree);
	
	// propaga adresele de la frunza catre radacina
	
	while (!List_IsEmpty(leaves)) {
		struct Data *leafWrapper = List_RemoveFirstNode(leaves, true);
		
		struct TreeNode *leaf = TreeNode_Data_Unwrap(leafWrapper);
		
		Data_Destroy(leafWrapper);
		
		struct TreeNode *node = leaf;
		struct TreeNode *parent = TreeNode_GetParent(leaf);
		
		while (parent) {
			struct Server *nodeServer =
				Server_Data_Unwrap(TreeNode_GetData(node));
			
			struct Server *parentServer =
				Server_Data_Unwrap(TreeNode_GetData(parent));
			
			Server_InheritAddresses(parentServer, nodeServer);
			
			node = parent;
			parent = TreeNode_GetParent(parent);
		}
	}
	
	List_Destroy(leaves);
	
	// afiseaza nodurile arborelui
	
	FILE *fout = fopen(HIERARCHY_OUT_FILE_NAME, "w");
	
	if (!fout) {
		error(EXIT_FAILURE, errno,
		      "%s: Could not open file",
		      HIERARCHY_OUT_FILE_NAME);
	}
	
	Tree_Print(tree, Server_Data_print_func, "\n", "\n", fout);
	
	if (fclose(fout) == EOF) {
		error(EXIT_FAILURE, errno,
		      "%s: Could not close file",
		      HIERARCHY_OUT_FILE_NAME);
	}
}
