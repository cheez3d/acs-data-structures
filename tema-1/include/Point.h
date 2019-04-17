#ifndef POINT_H
#define POINT_H

#include "Data.h"

#include <stdio.h>

struct Point {
	unsigned timestamp;
	
	double value;
};

struct Point * Point_Data_Unwrap(const struct Data *data);

int Point_Data_comp_func_value(const struct Data *first,
                               const struct Data *second);

void Point_Data_print_func(const struct Data *data, FILE *stream);

#endif // POINT_H
