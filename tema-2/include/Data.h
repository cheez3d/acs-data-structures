#ifndef DATA_H
#define DATA_H

#include <stdio.h>

struct Data {
	void *mem;
	size_t size;
};

typedef int (*Data_comp_func_t)(const struct Data *first,
                                const struct Data *second);

typedef void (*Data_proc_func_t)(struct Data *data);

typedef void (*Data_print_func_t)(const struct Data *data, FILE *stream);

struct Data * Data_Create(const void *mem, size_t size);

struct Data * Data_Copy(const struct Data *data);

void Data_Destroy(struct Data *data);

#endif // DATA_H
