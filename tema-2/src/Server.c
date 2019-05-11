#include "Server.h"

#include "Data.h"
#include "List.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NDEBUG
#include <assert.h>

// PRIVATE

static int address_comp_func(const char *first,
                             const char *second)
{
	assert(first);
	assert(second);
	
	return strcmp(first, second);
}

static void address_print_func(const char *address, FILE *stream) {
	assert(address);
	assert(stream);
	
	fprintf(stream, "%s", address);
}

static char * address_Data_Unwrap(const struct Data *data) {
	assert(data);
	assert(data->mem);
	
	return *(char **)data->mem;
}

static int address_Data_comp_func(const struct Data *first,
                                  const struct Data *second)
{
	assert(first);
	assert(second);
	
	return address_comp_func(address_Data_Unwrap(first),
	                         address_Data_Unwrap(second));
}

static void address_Data_print_func(const struct Data *data, FILE *stream) {
	assert(data);
	assert(stream);
	
	address_print_func(address_Data_Unwrap(data), stream);
}

static struct Server * Create(size_t id) {
	struct Server *server = malloc(sizeof *server);
	
	server->id = id;
	
	server->addresses = List_Create();
	
	return server;
}

static bool ContainsAddress(const struct Server *server, const char *address) {
	assert(server);
	assert(address);
	
	const struct Data addressWrapper =
		(struct Data){ &address, sizeof address };
	
	return List_ContainsData(server->addresses,
		                     &addressWrapper,
		                     address_Data_comp_func);
}

static void AddAddress(struct Server *server, const char *address) {
	assert(server);
	assert(address);
	
	char *addressCopy = strdup(address);
	
	const struct Data addressCopyWrapper =
		(struct Data){ &addressCopy, sizeof addressCopy };
	
	List_AddDataLast(server->addresses, &addressCopyWrapper);
	
	// List_Sort(server->addresses, address_Data_comp_func);
}

static void InheritAddresses(struct Server *server,
                             const struct Server *child)
{
	assert(server);
	assert(child);
	
	struct ListNode *node = List_GetFirstNode(child->addresses);
	
	while (node) {
		char *address = address_Data_Unwrap(ListNode_GetData(node));
		
		if (!ContainsAddress(server, address)) {
			AddAddress(server, address);
		}
		
		node = ListNode_GetNext(node);
	}
}

static void Destroy(struct Server *server) {
	assert(server);
	
	while (!List_IsEmpty(server->addresses)) {
		struct Data *addressWrapper =
			List_RemoveFirstNode(server->addresses, true);
		
		char *address = address_Data_Unwrap(addressWrapper);
		
		free(address);
		
		Data_Destroy(addressWrapper);
	}
	
	List_Destroy(server->addresses);
	
	free(server);
}

static int comp_func_id(const struct Server *first,
                        const struct Server *second)
{
	assert(first);
	assert(second);
	
	if (first->id < second->id) { return -1; }
	if (first->id > second->id) { return 1; }
	
	return 0;
}

static void print_func_id(const struct Server *server, FILE *stream) {
	assert(server);
	assert(stream);
	
	fprintf(stream, "%zu", server->id);
}

static void print_func_addresses(const struct Server *server, FILE *stream) {
	assert(server);
	assert(stream);
	
	List_Print(server->addresses, address_Data_print_func, " ", NULL, stream);
}

static void print_func(const struct Server *server, FILE *stream) {
	assert(server);
	assert(stream);
	
	print_func_id(server, stream);
	
	fprintf(stream, " ");
	
	print_func_addresses(server, stream);
}

static struct Server * Data_Unwrap(const struct Data *data) {
	assert(data);
	assert(data->mem);
	
	return *(struct Server **)data->mem;
}

static int Data_comp_func_id(const struct Data *first,
                             const struct Data *second)
{
	assert(first);
	assert(second);
	
	return comp_func_id(Data_Unwrap(first), Data_Unwrap(second));
}

static void Data_proc_func_Destroy(struct Data *data) {
	assert(data);
	
	Destroy(Data_Unwrap(data));
}

static void Data_print_func_id(const struct Data *data, FILE *stream) {
	assert(data);
	assert(stream);
	
	print_func_id(Data_Unwrap(data), stream);
}

static void Data_print_func_addresses(const struct Data *data, FILE *stream) {
	assert(data);
	assert(stream);
	
	print_func_addresses(Data_Unwrap(data), stream);
}

static void Data_print_func(const struct Data *data, FILE *stream) {
	assert(data);
	assert(stream);
	
	print_func(Data_Unwrap(data), stream);
}

// PUBLIC

char * Server_address_Data_Unwrap(const struct Data *data) {
	assert(data);
	
	return address_Data_Unwrap(data);
}

struct Server * Server_Create(size_t id) {
	return Create(id);
}

bool Server_ContainsAddress(const struct Server *server, const char *address) {
	assert(server);
	assert(address);
	
	return ContainsAddress(server, address);
}

void Server_AddAddress(struct Server *server, const char *address) {
	assert(server);
	assert(address);
	
	AddAddress(server, address);
}

void Server_InheritAddresses(struct Server *server,
                             const struct Server *child)
{
	assert(server);
	assert(child);
	
	InheritAddresses(server, child);
}

void Server_Destroy(struct Server *server) {
	assert(server);
	
	Destroy(server);
}

struct Server * Server_Data_Unwrap(const struct Data *data) {
	assert(data);
	
	return Data_Unwrap(data);
}

int Server_Data_comp_func_id(const struct Data *first,
                             const struct Data *second)
{
	assert(first);
	assert(second);
	
	return Data_comp_func_id(first, second);
}

void Server_Data_proc_func_Destroy(struct Data *data) {
	assert(data);
	
	Data_proc_func_Destroy(data);
}

void Server_Data_print_func_id(const struct Data *data, FILE *stream) {
	assert(data);
	assert(stream);
	
	Data_print_func_id(data, stream);
}

void Server_Data_print_func_addresses(const struct Data *data, FILE *stream) {
	assert(data);
	assert(stream);
	
	Data_print_func_addresses(data, stream);
}

void Server_Data_print_func(const struct Data *data, FILE *stream) {
	assert(data);
	assert(stream);
	
	Data_print_func(data, stream);
}
