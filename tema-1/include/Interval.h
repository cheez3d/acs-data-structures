#ifndef INTERVAL_H
#define INTERVAL_H

#include "Data.h"

#include <stdio.h>

struct Interval {
	double start;
	double end;
	
	size_t pointCount;
};

struct Interval * Interval_Data_Unwrap(const struct Data *data);

int Interval_Data_comp_func_start(const struct Data *first,
                                  const struct Data *second);

void Interval_Data_print_func(const struct Data *data, FILE *stream);

#endif // INTERVAL_H
