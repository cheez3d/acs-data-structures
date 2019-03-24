#ifndef DATA_H
#define DATA_H

#include <stddef.h>

struct Data {
    void *mem;
    size_t size;
};

struct Data * Data_Create(const void *mem, size_t size);

struct Data * Data_Copy(const struct Data *data);

size_t Data_GetSize(const struct Data *data);

void Data_Destroy(struct Data *data);

#endif // DATA_H
