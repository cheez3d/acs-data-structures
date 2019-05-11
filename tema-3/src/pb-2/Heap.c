#include "Data.h"
#include "Heap.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// #define NDEBUG
#include <assert.h>

// PRIVATE

static const size_t MIN_CAPACITY = 2;

struct Heap {
	size_t size;
	
	size_t capacity;
	
	struct Data **container;
	
	Data_comp_func_t comp_func;
	Data_update_func_t update_func;
	
	bool min;
};

static inline size_t index_left(size_t i) { return 2 * i + 1; }

static inline size_t index_right(size_t i) { return 2 * i + 2; }

static inline size_t index_parent(size_t i) { return (i - 1) / 2; }

static size_t GetSize(const struct Heap *heap) {
	assert(heap);
	
	return heap->size;
}

static size_t GetCapacity(const struct Heap *heap) {
	assert(heap);
	
	return heap->capacity;
}

static struct Data ** GetContainer(const struct Heap *heap) {
	assert(heap);
	
	return heap->container;
}

static Data_comp_func_t Get_comp_func(const struct Heap *heap) {
	assert(heap);
	
	return heap->comp_func;
}

static Data_update_func_t Get_update_func(const struct Heap *heap) {
	assert(heap);
	
	return heap->update_func;
}

static bool IsMin(const struct Heap *heap) {
	assert(heap);
	
	return heap->min;
}

static void SetSize(struct Heap *heap, size_t size) {
	assert(heap);
	
	heap->size = size;
}

static void SetCapacity(struct Heap *heap, size_t capacity) {
	assert(heap);
	
	assert(capacity >= GetSize(heap));
	
	heap->capacity = capacity;
}

static void Set_comp_func(struct Heap *heap, Data_comp_func_t comp_func) {
	assert(heap);
	assert(comp_func);
	
	heap->comp_func = comp_func;
}

static void Set_update_func(struct Heap *heap,
                            Data_update_func_t update_func)
{
	assert(heap);
	assert(update_func);
	
	heap->update_func = update_func;
}

static void SetMin(struct Heap *heap, bool min) {
	assert(heap);
	
	heap->min = min;
}

static void AdjustCapacity(struct Heap *heap,
                           size_t capacity)
{
	assert(heap);
	
	assert(capacity >= GetSize(heap));
	
	heap->container =
		realloc(heap->container, capacity * sizeof *heap->container);
	
	for (size_t i = GetCapacity(heap); i < capacity; ++i) {
		heap->container[i] = NULL;
	}
	
	SetCapacity(heap, capacity);
}

static struct Heap * Create(Data_comp_func_t comp_func,
                            Data_update_func_t update_func,
                            bool min)
{
	assert(comp_func);
	assert(update_func);
	
	assert(MIN_CAPACITY >= 2);
	
	struct Heap *heap = malloc(sizeof *heap);
	
	SetSize(heap, 0);
	
	SetCapacity(heap, 0);
	heap->container = NULL;
	AdjustCapacity(heap, MIN_CAPACITY);
	
	Set_comp_func(heap, comp_func);
	Set_update_func(heap, update_func);
	
	SetMin(heap, min);
	
	return heap;
}

static void Update(struct Heap *heap, size_t i) {
	assert(heap);
	
	assert(i < GetSize(heap));
	
	Data_comp_func_t comp_func = Get_comp_func(heap);
	int ref = IsMin(heap) ? -1 : 1;
	
	struct Data **container = GetContainer(heap);
	
	size_t left_i = index_left(i);
	size_t right_i = index_right(i);
	
	size_t candidate_i = i;
	
	if (left_i < GetSize(heap) &&
	    comp_func(container[left_i], container[candidate_i]) == ref)
	{
		candidate_i = left_i;
	}
	
	if (right_i < GetSize(heap) &&
	    comp_func(container[right_i], container[candidate_i]) == ref)
	{
		candidate_i = right_i;
	}
	
	if (candidate_i == i) { return; }
	
	struct Data *temp = container[i];
	container[i] = container[candidate_i];
	container[candidate_i] = temp;
	
	Update(heap, candidate_i);
}

static bool IsEmpty(const struct Heap *heap) {
	assert(heap);
	
	return GetSize(heap) == 0;
}

static struct Data *GetFirstData(const struct Heap *heap) {
	assert(heap);
	
	if (IsEmpty(heap)) { return NULL; }
	
	return GetContainer(heap)[0];
}

static size_t ContainsData(const struct Heap *heap,
                           const struct Data *data,
                           Data_comp_func_t comp_func)
{
	assert(heap);
	assert(data);
	assert(comp_func);
	
	struct Data **container = GetContainer(heap);
	
	for (size_t i = 0; i < GetSize(heap); ++i) {
		if (comp_func(container[i], data) == 0) {
			return i;
		}
	}
	
	return GetSize(heap);
}

static size_t AddData(struct Heap *heap, const struct Data *data) {
	assert(heap);
	assert(data);
	
	if (GetCapacity(heap) == GetSize(heap)) {
		AdjustCapacity(heap, 2 * GetCapacity(heap));
	}
	
	// indexul provizoriu la care va fi inserat elemnetul
	size_t i = GetSize(heap);
	
	SetSize(heap, GetSize(heap) + 1);
	
	struct Data **container = GetContainer(heap);
	
	container[i] = Data_Copy(data);
	
	// filtreaza in sus elementul
	
	size_t parent_i = index_parent(i);
	
	Data_comp_func_t comp_func = Get_comp_func(heap);
	int ref = IsMin(heap) ? -1 : 1;
	
	while (i > 0 && comp_func(container[i], container[parent_i]) == ref) {
		struct Data *temp = container[i];
		container[i] = container[parent_i];
		container[parent_i] = temp;
		
		i = parent_i;
		parent_i = index_parent(i);
	}
	
	return i;
}

static size_t AdjustDataPriority(struct Heap *heap,
                                 struct Data *data,
                                 const struct Data *new)
{
	assert(heap);
	assert(data);
	assert(new);
	
	Data_comp_func_t comp_func = Get_comp_func(heap);
	int ref = IsMin(heap) ? -1 : 1;
	
	struct Data **container = GetContainer(heap);
	
	size_t i = ContainsData(heap, data, comp_func);
	
	// verifica daca datele se afla in heap
	assert(i < GetSize(heap));
	
	assert(comp_func(new, data) == ref);
	
	Data_update_func_t update_func = Get_update_func(heap);
	
	update_func(data, new); // actualizeaza prioritatea
	
	// filtreaza in sus elementul
	
	size_t parent_i = index_parent(i);
	
	while (i > 0 && comp_func(container[i], container[parent_i]) == ref) {
		struct Data *temp = container[i];
		container[i] = container[parent_i];
		container[parent_i] = temp;
		
		i = parent_i;
		parent_i = index_parent(i);
	}
	
	return i;
}

static struct Data *RemoveFirstData(struct Heap *heap) {
	assert(heap);
	
	if (IsEmpty(heap)) { return NULL; }
	
	struct Data **container = GetContainer(heap);
	
	struct Data *first = container[0];
	
	if (GetSize(heap) == 1) {
		container[0] = NULL;
		
		SetSize(heap, GetSize(heap) - 1);
		
		return first;
	}
	
	size_t last_i = GetSize(heap) - 1;
	
	container[0] = container[last_i];
	
	container[last_i] = NULL;
	
	SetSize(heap, GetSize(heap) - 1);
	
	if (GetSize(heap) > MIN_CAPACITY / 4 &&
	    GetCapacity(heap) / 4 == GetSize(heap))
	{
		AdjustCapacity(heap, GetCapacity(heap) / 2);
	}
	
	Update(heap, 0);
	
	return first;
}

static void Process(struct Heap *heap, Data_proc_func_t proc_func) {
	assert(heap);
	assert(proc_func);
	
	for (size_t i = 0; i < GetSize(heap); ++i) {
		proc_func(GetContainer(heap)[i]);
	}
}

static void Print(const struct Heap *heap,
                  Data_print_func_t print_func,
                  const char *delim,
                  const char *endMark,
                  FILE *stream)
{
	assert(heap);
	assert(print_func);
	assert(stream);
	
	if (IsEmpty(heap)) { return; }
	
	struct Data **container = GetContainer(heap);
	
	for (size_t i = 0; i < GetSize(heap); ++i) {
		print_func(container[i], stream);
		
		if (i != GetSize(heap) - 1) {
			if (delim) { fprintf(stream, "%s", delim); }
		}
	}
	
	if (endMark) { fprintf(stream, "%s", endMark); }
}

static void PrintWithChildren(const struct Heap *heap,
                              Data_print_func_t print_func,
                              const char *delimChildren,
                              const char *delim,
                              const char *endMark,
                              FILE *stream)
{
	assert(heap);
	assert(print_func);
	assert(stream);
	
	if (IsEmpty(heap)) { return; }
	
	struct Data **container = GetContainer(heap);
	
	for (size_t i = 0; i < GetSize(heap); ++i) {
		print_func(container[i], stream);
		
		size_t left_i = index_left(i);
		bool leftExists = left_i < GetSize(heap);
		
		size_t right_i = index_right(i);
		bool rightExists = right_i < GetSize(heap);
		
		if (leftExists) {
			if (delimChildren) { fprintf(stream, "%s", delimChildren); }
			
			print_func(container[left_i], stream);
		}
		
		if (rightExists) {
			if (delimChildren) { fprintf(stream, "%s", delimChildren); }
			
			print_func(container[right_i], stream);
		}
		
		if (i != GetSize(heap) - 1) {
			if (delim) { fprintf(stream, "%s", delim); }
		}
	}
	
	if (endMark) { fprintf(stream, "%s", endMark); }
}

static void Clear(struct Heap *heap) {
	assert(heap);
	
	for (size_t i = 0; i < GetSize(heap); ++i) {
		Data_Destroy(GetContainer(heap)[i]);
	}
}

static void Destroy(struct Heap *heap) {
	assert(heap);
	
	Clear(heap);
	
	free(GetContainer(heap));
	
	free(heap);
}

// PUBLIC

size_t Heap_index_left(size_t i) {
	return index_left(i);
}

size_t Heap_index_right(size_t i) {
	return index_right(i);
}

size_t Heap_index_parent(size_t i) {
	return index_parent(i);
}

size_t Heap_GetSize(const struct Heap *heap) {
	assert(heap);
	
	return GetSize(heap);
}

struct Data ** Heap_GetContainer(const struct Heap *heap) {
	assert(heap);
	
	return GetContainer(heap);
}

struct Heap * Heap_Create(Data_comp_func_t comp_func,
                          Data_update_func_t update_func,
                          bool min)
{
	assert(comp_func);
	assert(update_func);
	
	return Create(comp_func, update_func, min);
}

bool Heap_IsEmpty(const struct Heap *heap) {
	assert(heap);
	
	return IsEmpty(heap);
}

struct Data *Heap_GetFirstData(const struct Heap *heap) {
	assert(heap);
	
	return GetFirstData(heap);
}

size_t Heap_ContainsData(const struct Heap *heap,
                         const struct Data *data,
                         Data_comp_func_t comp_func)
{
	assert(heap);
	assert(data);
	assert(comp_func);
	
	return ContainsData(heap, data, comp_func);
}

size_t Heap_AddData(struct Heap *heap, const struct Data *data) {
	assert(heap);
	assert(data);
	
	return AddData(heap, data);
}

size_t Heap_AdjustDataPriority(struct Heap *heap,
                               struct Data *data,
                               const struct Data *new)
{
	assert(heap);
	assert(data);
	assert(new);
	
	return AdjustDataPriority(heap, data, new);
}

struct Data *Heap_RemoveFirstData(struct Heap *heap) {
	assert(heap);
	
	return RemoveFirstData(heap);
}

void Heap_Process(struct Heap *heap, Data_proc_func_t proc_func) {
	assert(heap);
	assert(proc_func);
	
	Process(heap, proc_func);
}

void Heap_Print(const struct Heap *heap,
                Data_print_func_t print_func,
                const char *delim,
                const char *endMark,
                FILE *stream)
{
	assert(heap);
	assert(print_func);
	assert(stream);
	
	Print(heap, print_func, delim, endMark, stream);
}

void Heap_PrintWithChildren(const struct Heap *heap,
                            Data_print_func_t print_func,
                            const char *delimChildren,
                            const char *delim,
                            const char *endMark,
                            FILE *stream)
{
	assert(heap);
	assert(print_func);
	assert(stream);
	
	PrintWithChildren(heap, print_func, delimChildren, delim, endMark, stream);
}

void Heap_Destroy(struct Heap *heap) {
	assert(heap);
	
	Destroy(heap);
}
