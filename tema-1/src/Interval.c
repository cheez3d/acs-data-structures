#include "Interval.h"

#include "Data.h"

#include <stdio.h>

#define NDEBUG
#include <assert.h>

// PRIVATE

static int comp_func_start(const struct Interval *first,
                           const struct Interval *second)
{
	assert(first);
	assert(second);
	
	if (first->start < second->start) { return -1; }
	if (first->start > second->start) { return 1; }
	
	return 0;
}

static void print_func(const struct Interval *interval, FILE *stream) {
	assert(interval);
	assert(stream);
	
	fprintf(stream,
	        "[%g, %g] %zu",
	        interval->start,
	        interval->end,
	        interval->pointCount);
}

static struct Interval * Data_Unwrap(const struct Data *data) {
	assert(data);
	
	return (struct Interval *)data->mem;
}

static int Data_comp_func_start(const struct Data *first,
                                const struct Data *second)
{
	assert(first);
	assert(second);
	
	return comp_func_start(Data_Unwrap(first), Data_Unwrap(second));
}

static void Data_print_func(const struct Data *data, FILE *stream) {
	assert(data);
	assert(stream);
	
	print_func(Data_Unwrap(data), stream);
}

// PUBLIC

struct Interval * Interval_Data_Unwrap(const struct Data *data) {
	assert(data);
	
	return Data_Unwrap(data);
}

int Interval_Data_comp_func_start(const struct Data *first,
                                  const struct Data *second)
{
	assert(first);
	assert(second);
	
	return Data_comp_func_start(first, second);
}

void Interval_Data_print_func(const struct Data *data, FILE *stream) {
	assert(data);
	assert(stream);
	
	Data_print_func(data, stream);
}
