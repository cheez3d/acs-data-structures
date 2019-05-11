#include "Actor.h"

#include "List.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// #define NDEBUG
#include <assert.h>

// PRIVATE

static struct Actor * Create(const char *name) {
	assert(name);
	
	struct Actor *actor = malloc(sizeof *actor);
	
	actor->name = strdup(name);
	
	actor->movieList = List_Create();
	
	return actor;
}

static void AddMovie(struct Actor *actor, const struct Movie *movie) {
	const struct Data movieWrapper =
		(struct Data){ &movie, sizeof movie };
	
	List_AddDataLast(actor->movieList, &movieWrapper);
}

static void Destroy(struct Actor *actor) {
	assert(actor);
	
	free(actor->name);
	
	List_Destroy(actor->movieList);
	
	free(actor);
}

static int comp_func_name(const struct Actor *first,
                          const struct Actor *second)
{
	assert(first);
	assert(second);
	
	return strcmp(first->name, second->name);
}

static void print_func_name(const struct Actor *actor, FILE *stream) {
	assert(actor);
	assert(stream);
	
	fprintf(stream, "%s", actor->name);
}

static struct Actor * Data_Unwrap(const struct Data *data) {
	assert(data);
	assert(data->mem);
	
	return *(struct Actor **)data->mem;
}

static int Data_comp_func_name(const struct Data *first,
                               const struct Data *second)
{
	assert(first);
	assert(second);
	
	return comp_func_name(Data_Unwrap(first), Data_Unwrap(second));
}

static void Data_proc_func_Destroy(struct Data *data) {
	assert(data);
	
	Destroy(Data_Unwrap(data));
}

static void Data_print_func_name(const struct Data *data, FILE *stream) {
	assert(data);
	assert(stream);
	
	print_func_name(Data_Unwrap(data), stream);
}

// PUBLIC

struct Actor * Actor_Create(const char *name) {
	assert(name);
	
	return Create(name);
}

void Actor_AddMovie(struct Actor *actor, const struct Movie *movie) {
	assert(actor);
	assert(movie);
	
	AddMovie(actor, movie);
}

void Actor_Destroy(struct Actor *actor) {
	assert(actor);
	
	Destroy(actor);
}

struct Actor * Actor_Data_Unwrap(const struct Data *data) {
	assert(data);
	
	return Data_Unwrap(data);
}

int Actor_Data_comp_func_name(const struct Data *first,
                              const struct Data *second)
{
	assert(first);
	assert(second);
	
	return Data_comp_func_name(first, second);
}

void Actor_Data_proc_func_Destroy(struct Data *data) {
	assert(data);
	
	Data_proc_func_Destroy(data);
}

void Actor_Data_print_func_name(const struct Data *data, FILE *stream) {
	assert(data);
	assert(stream);
	
	Data_print_func_name(data, stream);
}
