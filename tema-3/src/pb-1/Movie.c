#include "Movie.h"

#include "List.h"

#include <stdlib.h>
#include <string.h>

#define NDEBUG
#include <assert.h>

// PRIVATE

static struct Movie * Create(const char *name) {
	assert(name);
	
	struct Movie *movie = malloc(sizeof *movie);
	
	movie->name = strdup(name);
	
	movie->actorList = List_Create();
	
	return movie;
}

static void AddActor(struct Movie *movie, const struct Actor *actor) {
	const struct Data actorWrapper =
		(struct Data){ &actor, sizeof actor };
	
	List_AddDataLast(movie->actorList, &actorWrapper);
}

static void Destroy(struct Movie *movie) {
	assert(movie);
	
	free(movie->name);
	
	List_Destroy(movie->actorList);
	
	free(movie);
}

static int comp_func_name(const struct Movie *first,
                          const struct Movie *second)
{
	assert(first);
	assert(second);
	
	return strcmp(first->name, second->name);
}

static void print_func_name(const struct Movie *movie, FILE *stream) {
	assert(movie);
	assert(stream);
	
	fprintf(stream, "%s", movie->name);
}

static struct Movie * Data_Unwrap(const struct Data *data) {
	assert(data);
	assert(data->mem);
	
	return *(struct Movie **)data->mem;
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

struct Movie * Movie_Create(const char *name) {
	assert(name);
	
	return Create(name);
}

void Movie_AddActor(struct Movie *movie, const struct Actor *actor) {
	assert(movie);
	assert(actor);
	
	AddActor(movie, actor);
}

void Movie_Destroy(struct Movie *movie) {
	assert(movie);
	
	Destroy(movie);
}

struct Movie * Movie_Data_Unwrap(const struct Data *data) {
	assert(data);
	
	return Data_Unwrap(data);
}

int Movie_Data_comp_func_name(const struct Data *first,
                              const struct Data *second)
{
	assert(first);
	assert(second);
	
	return Data_comp_func_name(first, second);
}

void Movie_Data_proc_func_Destroy(struct Data *data) {
	assert(data);
	
	Data_proc_func_Destroy(data);
}

void Movie_Data_print_func_name(const struct Data *data, FILE *stream) {
	assert(data);
	assert(stream);
	
	Data_print_func_name(data, stream);
}
