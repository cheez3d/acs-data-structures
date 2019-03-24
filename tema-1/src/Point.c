#include "Point.h"

#include "Data.h"

#include <stdio.h>

int Point_List_comp_func(const struct Data *first,
                         const struct Data *second) {
    if (!first) { return -1; }
    if (!second) { return 1; }
    
    double firstValue = (*(struct Point **)first)->value;
    double secondValue = (*(struct Point **)second)->value;
    
    if (firstValue < secondValue) { return -1; }
    if (firstValue > secondValue) { return  1; }
    
    return 0;
}

void Point_List_print_func(const struct Data *data) {
    if (!data) { return; }
    
    struct Point *point = *(struct Point **)data;
    
    printf("%u %.2lf", point->timestamp, point->value);
    printf("\n");
}
