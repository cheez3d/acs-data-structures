#ifndef GRAPH_H
#define GRAPH_H

#include "Data.h"

#include <stdbool.h>

struct GraphNode;
struct Graph;

struct Data * GraphNode_GetData(const struct GraphNode *node);

const struct List * GraphNode_GetNeighbors(const struct GraphNode *node);

struct List * GraphNode_GetMetadata(const struct GraphNode *node);

int GraphNode_comp_func(const struct GraphNode *first,
                        const struct GraphNode *second);

struct GraphNode * GraphNode_Data_Unwrap(const struct Data *data);

int GraphNode_Data_comp_func(const struct Data *first,
                             const struct Data *second);

struct List * Graph_GetNodes(const struct Graph *graph);

struct Graph * Graph_Create(bool isWeighted);

bool Graph_IsEmpty(const struct Graph *graph);

struct GraphNode * Graph_ContainsData(const struct Graph *graph,
                                      const struct Data *data,
                                      Data_comp_func_t comp_func);

struct GraphNode * Graph_AddData(struct Graph *graph,
                                 const struct Data *data);

bool Graph_ContainsOrientedLink(const struct Graph *graph,
                                const struct GraphNode *src,
                                const struct GraphNode *dest);

bool Graph_ContainsLink(const struct Graph *graph,
                        const struct GraphNode *first,
                        const struct GraphNode *second);

void Graph_AddOrientedLink(struct Graph *graph,
                           struct GraphNode *src,
                           struct GraphNode *dest);

void Graph_AddLink(struct Graph *graph,
                   struct GraphNode *first,
                   struct GraphNode *second);

void Graph_AddWeightedOrientedLink(struct Graph *graph,
                                   struct GraphNode *src,
                                   struct GraphNode *dest,
                                   unsigned weight);

void Graph_AddWeightedLink(struct Graph *graph,
                           struct GraphNode *first,
                           struct GraphNode *second,
                           unsigned weight);

size_t Graph_GetConnectedComponentCount(struct Graph *graph);

void Graph_Process(struct Graph *graph,
                   Data_proc_func_t proc_func);

void Graph_ClearNodesMetadata(struct Graph *graph);



void Graph_Clear(struct Graph *graph);

void Graph_Destroy(struct Graph *graph);

#endif // GRAPH_H