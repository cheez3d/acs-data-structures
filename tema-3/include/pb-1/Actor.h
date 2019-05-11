#ifndef ACTOR_H
#define ACTOR_H

#include "List.h"
#include "Movie.h"

struct Actor {
	char *name;
	
	struct List *movieList;
};

struct Movie;

struct Actor * Actor_Create(const char *name);

void Actor_AddMovie(struct Actor *actor, const struct Movie *movie);

void Actor_Destroy(struct Actor *actor);

struct Actor * Actor_Data_Unwrap(const struct Data *data);

int Actor_Data_comp_func_name(const struct Data *first,
                              const struct Data *second);

void Actor_Data_proc_func_Destroy(struct Data *data);

void Actor_Data_print_func_name(const struct Data *data, FILE *stream);

#endif // ACTOR_H