#ifndef POINT_H
#define POINT_H

#include "Data.h"

struct Point {
    unsigned timestamp;
    double value;
};

int Point_List_comp_func(const struct Data *first,
                         const struct Data *second);

void Point_List_print_func(const struct Data *data);

#endif // POINT_H
