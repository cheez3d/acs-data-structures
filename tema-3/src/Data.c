#include "Data.h"

#include <stdlib.h>
#include <string.h>

#define NDEBUG
#include <assert.h>

// PRIVATE

static struct Data * Create(const void *mem, size_t size) {
	assert(mem);
	assert(size > 0);
	
	struct Data *data = malloc(sizeof *data);
	
	data->mem = memcpy(malloc(size), mem, size);
	data->size = size;
	
	return data;
}

static struct Data * Copy(const struct Data *data) {
	assert(data);
	
	return Create(data->mem, data->size);
}

static void Destroy(struct Data *data) {
	assert(data);
	
	free(data->mem);
	
	free(data);
}

// PUBLIC

struct Data * Data_Create(const void *mem, size_t size) {
	assert(mem);
	
	return Create(mem, size);
}

struct Data * Data_Copy(const struct Data *data) {
	assert(data);
	
	return Copy(data);
}

void Data_Destroy(struct Data *data) {
	assert(data);
	
	Destroy(data);
}
