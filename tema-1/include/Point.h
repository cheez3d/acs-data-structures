#pragma once

#include "LinkedList.h"

struct Point {
    unsigned timestamp;
    float value;
};

void Point_LinkedList_print_func(const struct LinkedListData *data);
