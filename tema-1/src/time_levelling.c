#include "List.h"
#include "Point.h"

struct List * time_levelling(struct List *pointList,
                             List_print_func_t *out_print_func) {
    if (List_GetSize(pointList) < 2) { goto end; }
    
    struct ListNode *prev = List_GetFirstNode(pointList);
    struct ListNode *curr = ListNode_GetNext(prev);
    
    while (curr) {
        struct Point *prevPoint = *(struct Point **)ListNode_GetData(prev);
        struct Point *currPoint = *(struct Point **)ListNode_GetData(curr);
        
        unsigned diff = currPoint->timestamp - prevPoint->timestamp;
        
        if ((100 <= diff) && (diff <= 1000)) {
            currPoint->timestamp += prevPoint->timestamp;
            currPoint->timestamp /= 2;
            
            currPoint->value += prevPoint->value;
            currPoint->value /= 2.0;
        }
        
        prev = curr;
        curr = ListNode_GetNext(curr);
    }
    
end:
    *out_print_func = Point_List_print_func;
    
    return pointList;
}
