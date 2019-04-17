#include "Point.h"

#include "Data.h"

#include <stdio.h>

#define NDEBUG
#include <assert.h>

// PRIVATE

static int comp_func_value(const struct Point *first,
                           const struct Point *second)
{
	assert(first);
	assert(second);
	
	if (first->value < second->value) { return -1; }
	if (first->value > second->value) { return 1; }
	
	return 0;
}

static void print_func(const struct Point *point, FILE *stream) {
	assert(point);
	assert(stream);
	
	fprintf(stream, "%u %.2lf", point->timestamp, point->value);
}

static struct Point * Data_Unwrap(const struct Data *data) {
	assert(data);
	
	return (struct Point *)data->mem;
}

int Data_comp_func_value(const struct Data *first,
                         const struct Data *second)
{
	assert(first);
	assert(second);
	
	return comp_func_value(Data_Unwrap(first), Data_Unwrap(second));
}

void Data_print_func(const struct Data *data, FILE *stream) {
	assert(data);
	assert(stream);
	
	print_func(Data_Unwrap(data), stream);
}

// PUBLIC

struct Point * Point_Data_Unwrap(const struct Data *data) {
	assert(data);
	
	return Data_Unwrap(data);
}

int Point_Data_comp_func_value(const struct Data *first,
                               const struct Data *second)
{
	assert(first);
	assert(second);
	
	return Data_comp_func_value(first, second);
}

void Point_Data_print_func(const struct Data *data, FILE *stream) {
	assert(data);
	assert(stream);
	
	Data_print_func(data, stream);
}
