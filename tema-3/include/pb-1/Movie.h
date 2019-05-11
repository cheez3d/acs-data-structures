#ifndef MOVIE_H
#define MOVIE_H

#include "Actor.h"
#include "List.h"

struct Movie {
	char *name;
	
	struct List *actorList;
};

struct Actor;

struct Movie * Movie_Create(const char *name);

void Movie_AddActor(struct Movie *movie, const struct Actor *actor);

void Movie_Destroy(struct Movie *movie);

struct Movie * Movie_Data_Unwrap(const struct Data *data);

int Movie_Data_comp_func_name(const struct Data *first,
                              const struct Data *second);

void Movie_Data_proc_func_Destroy(struct Data *data);

void Movie_Data_print_func_name(const struct Data *data, FILE *stream);

#endif // MOVIE_H
