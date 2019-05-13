#include "Data.h"
#include "Room.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define NDEBUG
#include <assert.h>

// PRIVATE

static int time_comp_func(const unsigned first, const unsigned second) {
	if (first < second) { return -1; }
	if (first > second) { return 1; }
	
	return 0;
}

static int time_stdlib_comp_func(const void *first, const void *second) {
	assert(first);
	assert(second);
	
	return time_comp_func(*(const unsigned *)first,
	                      *(const unsigned *)second);
}

static struct Room * Create(size_t id,
                            bool isExit,
                            unsigned *guardTimes,
                            size_t guardTimeCount)
{
	// guardTimes poate fi si NULL
	
	struct Room *room = malloc(sizeof *room);
	
	room->id = id;
	
	room->isExit = isExit;
	
	qsort(guardTimes,
	      guardTimeCount,
	      sizeof *guardTimes,
	      time_stdlib_comp_func);
	
	room->guardTimes = guardTimes;
	room->guardTimeCount = guardTimeCount;
	
	return room;
}

bool IsExit(const struct Room *room) {
	assert(room);
	
	return room->isExit;
}

static bool ContainsGuardAtTime(const struct Room *room, unsigned time) {
	assert(room);
	
	return bsearch(&time,
	               room->guardTimes,
	               room->guardTimeCount,
	               sizeof *room->guardTimes,
	               time_stdlib_comp_func);
}

static void Destroy(struct Room *room) {
	assert(room);
	
	free(room->guardTimes);
	
	free(room);
}

static void print_func_id(const struct Room *room, FILE *stream) {
	assert(room);
	assert(stream);
	
	fprintf(stream, "%zu", room->id);
}

static struct Room * Data_Unwrap(const struct Data *data) {
	assert(data);
	assert(data->mem);
	
	return *(struct Room **)data->mem;
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

// PUBLIC

struct Room * Room_Create(size_t id,
                          bool isExit,
                          unsigned *guardTimes,
                          size_t guardTimeCount)
{
	// guardTimes poate fi si NULL
	
	return Create(id, isExit, guardTimes, guardTimeCount);
}

bool Room_IsExit(const struct Room *room) {
	assert(room);
	
	return IsExit(room);
}

bool Room_ContainsGuardAtTime(const struct Room *room, unsigned time) {
	assert(room);
	
	return ContainsGuardAtTime(room, time);
}

void Room_Destroy(struct Room *room) {
	assert(room);
	
	Destroy(room);
}

void Room_print_func_id(const struct Room *room, FILE *stream) {
	assert(room);
	assert(stream);
	
	print_func_id(room, stream);
}

struct Room * Room_Data_Unwrap(const struct Data *data) {
	assert(data);
	
	return Data_Unwrap(data);
}

void Room_Data_proc_func_Destroy(struct Data *data) {
	assert(data);
	
	Data_proc_func_Destroy(data);
}

void Room_Data_print_func_id(const struct Data *data, FILE *stream) {
	assert(data);
	assert(stream);
	
	Data_print_func_id(data, stream);
}
