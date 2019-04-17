#ifndef SERVER_H
#define SERVER_H

#include "List.h"

#include <stdio.h>

struct Server {
	size_t id;
	
	struct List *addresses;
};

struct Data Server_address_Data_Wrap(const char *address);

char * Server_address_Data_Unwrap(const struct Data *data);

struct Server * Server_Create(size_t id);

bool Server_ContainsAddress(const struct Server *server, const char *address);

void Server_AddAddress(struct Server *server, const char *address);

void Server_InheritAddresses(struct Server *server,
                             const struct Server *child);

void Server_Destroy(struct Server *server);

struct Server * Server_Data_Unwrap(const struct Data *data);

int Server_Data_comp_func_id(const struct Data *first,
                             const struct Data *second);

void Server_Data_proc_func_Destroy(struct Data *data);

void Server_Data_print_func_id(const struct Data *data, FILE *stream);

void Server_Data_print_func_addresses(const struct Data *data, FILE *stream);

void Server_Data_print_func(const struct Data *data, FILE *stream);

#endif // SERVER_H
