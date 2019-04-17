#ifndef USER_H
#define USER_H

#include "Tree.h"

struct User {
	size_t id;
	
	struct TreeNode *serverNode;
};

struct User * User_Create(size_t id, struct TreeNode *serverNode);

void User_Destroy(struct User *user);

struct User * User_Data_Unwrap(const struct Data *data);

int User_Data_comp_func_id(const struct Data *first,
                           const struct Data *second);

int User_Data_comp_func_serverNode(const struct Data *first,
                                   const struct Data *second);

void User_Data_proc_func_Destroy(struct Data *data);

#endif // USER_H
