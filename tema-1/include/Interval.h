#ifndef INTERVAL_H
#define INTERVAL_H

#include "Data.h"

#include <stddef.h>

struct Interval {
    double start;
    double end;
    
    size_t pointCount;
};

int Interval_List_comp_func(const struct Data *first,
                            const struct Data *second);

void Interval_List_print_func(const struct Data *data);

#endif // INTERVAL_H
