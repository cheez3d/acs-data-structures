#include "User.h"

#include "Tree.h"

#include <stdlib.h>

#define NDEBUG
#include <assert.h>

// PRIVATE

static struct User * Create(size_t id, struct TreeNode *serverNode) {
	struct User *user = malloc(sizeof *user);
	
	user->id = id;
	
	user->serverNode = serverNode;
	
	return user;
}

static void Destroy(struct User *user) {
	assert(user);
	
	free(user);
}

static int comp_func_id(const struct User *first,
                        const struct User *second)
{
	assert(first);
	assert(second);
	
	if (first->id < second->id) { return -1; }
	if (first->id > second->id) { return 1; }
	
	return 0;
}

static int comp_func_serverNode(const struct User *first,
                                const struct User *second)
{
	assert(first);
	assert(second);
	
	if (first->serverNode < second->serverNode) { return -1; }
	if (first->serverNode > second->serverNode) { return 1; }
	
	return 0;
}

static struct User * Data_Unwrap(const struct Data *data) {
	assert(data);
	assert(data->mem);
	
	return *(struct User **)data->mem;
}

static int Data_comp_func_id(const struct Data *first,
                             const struct Data *second)
{
	return comp_func_id(Data_Unwrap(first), Data_Unwrap(second));
}

static void Data_proc_func_Destroy(struct Data *data) {
	Destroy(Data_Unwrap(data));
}

static int Data_comp_func_serverNode(const struct Data *first,
                                     const struct Data *second)
{
	return comp_func_serverNode(Data_Unwrap(first), Data_Unwrap(second));
}

// PUBLIC

struct User * User_Create(size_t id, struct TreeNode *serverNode) {
	return Create(id, serverNode);
}

void User_Destroy(struct User *user) {
	Destroy(user);
}

struct User * User_Data_Unwrap(const struct Data *data) {
	return Data_Unwrap(data);
}

int User_Data_comp_func_id(const struct Data *first,
                           const struct Data *second)
{
	return Data_comp_func_id(first, second);
}

int User_Data_comp_func_serverNode(const struct Data *first,
                                   const struct Data *second)
{
	return Data_comp_func_serverNode(first, second);
}

void User_Data_proc_func_Destroy(struct Data *data) {
	Data_proc_func_Destroy(data);
}
