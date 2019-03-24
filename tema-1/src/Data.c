#include "Data.h"

#include <stdlib.h>
#include <string.h>

// PRIVATE

static struct Data * Create(const void *mem, size_t size) {
    if (!mem) { goto error; }
    if (size == 0) { goto error; }
    
    struct Data *data = malloc(sizeof *data);
    
    data->mem = memcpy(malloc(size), mem, size);
    data->size = size;
    
    return data;
    
error:
    return NULL;
}

static struct Data * Copy(const struct Data *data) {
    if (!data) { goto error; }
    
    return Create(data->mem, data->size);
    
error:
    return NULL;
}

static size_t GetSize(const struct Data *data) {
    if (!data) { goto error; }
    
    return data->size;
    
error:
    return 0;
}

static void Destroy(struct Data *data) {
    if (!data) { goto error; }
    
    free(data->mem);
    
    free(data);
    
    return;
    
error:
    return;
}

// PUBLIC

struct Data * Data_Create(const void *mem, size_t size) {
    return Create(mem, size);
}

struct Data * Data_Copy(const struct Data *data) {
    return Copy(data);
}

size_t Data_GetSize(const struct Data *data) {
    return GetSize(data);
}

void Data_Destroy(struct Data *data) {
    Destroy(data);
}
