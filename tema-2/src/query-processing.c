#include "Data.h"
#include "Server.h"
#include "Tree.h"
#include "User.h"

#include <errno.h>
#include <error.h>
#include <stdlib.h>

static const char *USERS_IN_FILE_NAME = "users.in";
static const char *QUERIES_IN_FILE_NAME = "queries.in";

static const char *QUERIES_OUT_FILE_NAME = "queries.out";

struct List * read_users(struct Tree *tree) {
	// citeste utilizatorii
	
	FILE *fin = fopen(USERS_IN_FILE_NAME, "r");
	
	if (!fin) {
		error(EXIT_FAILURE, errno,
		      "%s: Could not open file",
		      USERS_IN_FILE_NAME);
	}
	
	size_t userCount;
	
	if (fscanf(fin, "%zu", &userCount) != 1) {
		error(EXIT_FAILURE, errno,
		      "%s: Could not read user count",
		      USERS_IN_FILE_NAME);
	}
	
	// lista in care vor fi memorati utilizatorii
	struct List *users = List_Create();
	
	for (size_t i = 0; i < userCount; ++i) {
		size_t id;
		
		if (fscanf(fin, "%zu", &id) != 1) {
			error(EXIT_FAILURE, errno,
			      "%s: Could not read user id",
			      USERS_IN_FILE_NAME);
		}
		
		size_t serverId;
		
		if (fscanf(fin, "%zu", &serverId) != 1) {
			error(EXIT_FAILURE, errno,
			      "%s: Could not read user server id",
			      USERS_IN_FILE_NAME);
		}
		
		const struct Server *refServer = &(struct Server){ serverId, NULL };
		
		const struct Data refServerWrapper =
			(struct Data){ &refServer, sizeof refServer };
		
		struct TreeNode *serverNode =
			Tree_ContainsData(tree,
			                  &refServerWrapper,
			                  Server_Data_comp_func_id);
		
		if (!serverNode) {
			error(EXIT_FAILURE, errno,
			      "%s: Server with id '%zu' does not exist",
			      USERS_IN_FILE_NAME, serverId);
		}
		
		struct User *user = User_Create(id, serverNode);
		
		const struct Data userWrapper =
			(struct Data){ &user, sizeof user };
		
		List_AddDataLast(users, &userWrapper);
	}
	
	if (fclose(fin) == EOF) {
		error(EXIT_FAILURE, errno,
		      "%s: Could not close file",
		      USERS_IN_FILE_NAME);
	}
	
	return users;
}

void process_queries(struct Tree *tree, struct List *users) {
	FILE *fout = fopen(QUERIES_OUT_FILE_NAME, "w");
	
	if (!fout) {
		error(EXIT_FAILURE, errno,
		      "%s: Could not open file",
		      QUERIES_OUT_FILE_NAME);
	}
	
	// citeste cererile
	
	FILE *fin = fopen(QUERIES_IN_FILE_NAME, "r");
	
	if (!fin) {
		error(EXIT_FAILURE, errno,
		      "%s: Could not open file",
		      QUERIES_IN_FILE_NAME);
	}
	
	size_t queryCount;
	
	if (fscanf(fin, "%zu", &queryCount) != 1) {
		error(EXIT_FAILURE, errno,
		      "%s: Could not read query count",
		      QUERIES_IN_FILE_NAME);
	}
	
	for (size_t i = 0; i < queryCount; ++i) {
		char query;
		// TODO: add more info to errors
		if (fscanf(fin, "\n%c", &query) != 1) {
			error(EXIT_FAILURE, errno,
			      "%s: Could not read query",
			      QUERIES_IN_FILE_NAME);
		}
		
		switch (query) {
			case 'q':
				size_t userId;
				
				if (fscanf(fin, "%zu", &userId) != 1) {
					error(EXIT_FAILURE, errno,
					      "%s: Could not read query user id",
					      QUERIES_IN_FILE_NAME);
				}
				
				char *address;
				
				if (fscanf(fin, "%ms", &address) != 1) {
					error(EXIT_FAILURE, errno,
					      "%s: Could not read query server address",
					      QUERIES_IN_FILE_NAME);
				}
				
				const struct User *refUser = &(struct User){ userId, NULL };
				
				const struct Data refUserWrapper =
					(struct Data){ &refUser, sizeof refUser };
				
				struct ListNode *userNode =
					List_ContainsData(users,
					                  &refUserWrapper,
					                  User_Data_comp_func_id);
				
				if (!userNode) {
					error(EXIT_FAILURE, errno,
					      "%s: User with id '%zu' does not exist",
					      QUERIES_IN_FILE_NAME, userId);
				}
				
				struct User *user = User_Data_Unwrap(ListNode_GetData(userNode));
				
				struct List *queryPath = List_Create();
				
				struct TreeNode *serverNode = user->serverNode;
				
				while (serverNode) {
					struct Data *serverWrapper = TreeNode_GetData(serverNode);
					
					struct Server *server = Server_Data_Unwrap(serverWrapper);
					
					List_AddDataLast(queryPath, serverWrapper);
					
					if (Server_ContainsAddress(server, address)) { break; }
					
					serverNode = TreeNode_GetParent(serverNode);
				}
				
				// daca adresa ceruta se afla in ierarhia de servere
				// (altfel nu face nimic)
				if (serverNode) {
					// afiseaza drumul parcurs in arbore
					// pentru rezolvarea adresei
					List_Print(queryPath,
					           Server_Data_print_func_id,
					           " ", "\n",
					           fout);
					
					// salveaza adresa in serverle care nu o au deja
					
					struct ListNode *node = List_GetFirstNode(queryPath);
					
					while (node) {
						struct Server *server =
							Server_Data_Unwrap(ListNode_GetData(node));
						
						if (!Server_ContainsAddress(server, address)) {
							Server_AddAddress(server, address);
						}
						
						node = ListNode_GetNext(node);
					}
				}
				
				List_Destroy(queryPath);
				
				free(address);
				
				break;
			
			case 'f':
				size_t serverId;
				
				if (fscanf(fin, "%zu", &serverId) != 1) {
					error(EXIT_FAILURE, errno,
					      "%s: Could not read query server id",
					      QUERIES_IN_FILE_NAME);
				}
				
				// schimba serverul utilizatorilor care aveau
				// asociat serverul care urmeaza sa cada cu parintele acestuia
				
				const struct Server *refServer =
					&(struct Server){ serverId, NULL };
				
				const struct Data refServerWrapper =
					(struct Data){ &refServer, sizeof refServer };
				
				struct TreeNode *serverNode =
					Tree_ContainsData(tree,
					                  &refServerWrapper,
					                  Server_Data_comp_func_id);
				
				if (!serverNode) {
					// error(EXIT_FAILURE, errno,
					      // "%s: Server with id '%zu' does not exist",
					      // QUERIES_IN_FILE_NAME, serverId);
					
					break;
				}
				
				const struct User *refUser = &(struct User){ 0, serverNode };
				
				const struct Data refUserWrapper =
					(struct Data){ &refUser, sizeof refUser };
				
				struct ListNode *userNode = NULL;
				
				while ((userNode = List_ContainsData(users,
					                  &refUserWrapper,
					                  User_Data_comp_func_serverNode)))
				{
					struct User *user =
						User_Data_Unwrap(ListNode_GetData(userNode));
					
					user->serverNode = TreeNode_GetParent(user->serverNode);
				}
				
				struct Data *serverWrapper =
					Tree_RemoveNode(tree, serverNode, true);
				
				Server_Data_proc_func_Destroy(serverWrapper);
				
				Data_Destroy(serverWrapper);
				
				break;
			
			default:
				error(EXIT_FAILURE, errno,
				      "%s: Unknown query '%c'",
				      QUERIES_IN_FILE_NAME, query);
		}
	}
	
	if (fclose(fin) == EOF) {
		error(EXIT_FAILURE, errno,
		      "%s: Could not close file",
		      QUERIES_IN_FILE_NAME);
	}
	
	if (fclose(fout) == EOF) {
		error(EXIT_FAILURE, errno,
		      "%s: Could not close file",
		      QUERIES_OUT_FILE_NAME);
	}
}

void query_processing(struct Tree *tree) {
	struct List *users = read_users(tree);
	
	process_queries(tree, users);
	
	List_Process(users, User_Data_proc_func_Destroy);
	
	List_Destroy(users);
}
