#include "Data.h"
#include "List.h"
#include "Point.h"

#define WINDOW_SIZE 5 // 'k' din cerinta

_Static_assert(WINDOW_SIZE > 0, "WINDOW_SIZE cannot be nonpositive");

struct List * noise_removal_a_mean(struct List *pointList,
                                   List_print_func_t *out_print_func) {
    struct List *newPointList = List_Create();
    
    struct ListNode *windowStart = List_GetFirstNode(pointList);
    
    while (windowStart) {
        // obtine fereastra
        
        struct ListNode *windowEnd;
        size_t span;
        
        List_GetSpanNode(pointList,
                         windowStart,
                         WINDOW_SIZE - 1,
                         true,
                         &windowEnd,
                         &span);
        
        struct ListNode *windowCenter = List_GetNodesMiddle(pointList,
                                                            windowStart,
                                                            windowEnd);
        
        size_t windowSize = span + 1;
        
        if (windowSize != WINDOW_SIZE) { goto skip_node; }
        
        // vom folosi timestamp-ul vechiului
        // mijloc al ferestrei pentru noua valoare
        unsigned timestamp =
            (**(struct Point **)ListNode_GetData(windowCenter)).timestamp;
        
        // calculeaza media aritmetica a ferestrei
        
        double avg = 0.0;
        
        struct ListNode *curr = windowStart;
        
        while (curr != ListNode_GetNext(windowEnd)) {
            avg += (**(struct Point **)ListNode_GetData(curr)).value;
            
            curr = ListNode_GetNext(curr);
        }
        
        avg /= windowSize; 
        
        struct Point avgPoint = (struct Point){ timestamp, avg };
        
        struct Data avgPointContainer =
            (struct Data){ &avgPoint, sizeof avgPoint };
        
        // adauga noul punct in lista temporara
        List_AddDataLast(newPointList, &avgPointContainer);
        
    skip_node:
        windowStart = ListNode_GetNext(windowStart);
    }
    
    *out_print_func = Point_List_print_func;
    
    return newPointList;
}
