#ifndef HEAP_H
#define HEAP_H

#include "Data.h"

#include <stdbool.h>

struct Heap;

size_t Heap_index_left(size_t i);

size_t Heap_index_right(size_t i);

size_t Heap_index_parent(size_t i);

size_t Heap_GetSize(const struct Heap *heap);

struct Data ** Heap_GetContainer(const struct Heap *heap);

struct Heap * Heap_Create(bool min);

bool Heap_IsEmpty(const struct Heap *heap);

struct Data *Heap_GetFirstData(const struct Heap *heap);

size_t Heap_ContainsData(const struct Heap *heap,
                         const struct Data *data,
                         Data_comp_func_t comp_func);

size_t Heap_MoveDataUp(const struct Heap *heap,
                       size_t i,
                       Data_comp_func_t comp_func);

size_t Heap_AddData(struct Heap *heap,
                    const struct Data *data,
                    Data_comp_func_t comp_func);

size_t Heap_AdjustDataPriority(struct Heap *heap,
                               size_t i,
                               const struct Data *new,
                               Data_comp_func_t comp_func,
                               Data_update_func_t update_func);

struct Data *Heap_RemoveFirstData(struct Heap *heap,
                                  Data_comp_func_t comp_func);

void Heap_Process(struct Heap *heap, Data_proc_func_t proc_func);

void Heap_Print(const struct Heap *heap,
                Data_print_func_t print_func,
                const char *delim,
                const char *endMark,
                FILE *stream);

void Heap_PrintWithChildren(const struct Heap *heap,
                            Data_print_func_t print_func,
                            const char *delimChildren,
                            const char *delim,
                            const char *endMark,
                            FILE *stream);

void Heap_Destroy(struct Heap *heap);

#endif // HEAP_H
