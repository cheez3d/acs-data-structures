#include <stdio.h>

#include "Point.h"

void Point_LinkedList_print_func(const struct LinkedListData *data) { // TODO: move to Point.c
    if (!data) { return; }
    
    const struct Point point = **(const struct Point **)data;
    
    printf("%u %f", point.timestamp, point.value);
    printf("\n");
}
