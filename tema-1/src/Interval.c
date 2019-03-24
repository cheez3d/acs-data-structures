#include "Interval.h"

#include "Data.h"

#include <stdio.h>

int Interval_List_comp_func(const struct Data *first,
                            const struct Data *second) {
    if (!first) { return -1; }
    if (!second) { return 1; }
    
    double firstStart = (*(struct Interval **)first)->start;
    double secondStart = (*(struct Interval **)second)->start;
    
    if (firstStart < secondStart) { return -1; }
    if (firstStart > secondStart) { return  1; }
    
    return 0;
}

void Interval_List_print_func(const struct Data *data) {
    if (!data) { return; }
    
    const struct Interval *interval = *(struct Interval **)data;
    
    printf("[%g, %g] %zu",
           interval->start,
           interval->end,
           interval->pointCount);
    
    printf("\n");
}
