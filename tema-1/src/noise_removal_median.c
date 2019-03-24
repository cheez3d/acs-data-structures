#include "List.h"
#include "Point.h"

#define WINDOW_SIZE 5 // 'k' din cerinta

_Static_assert(WINDOW_SIZE > 0, "WINDOW_SIZE cannot be nonpositive");

struct List * noise_removal_median(struct List *pointList,
                                   List_print_func_t *out_print_func) {
    struct List *newPointList = List_Create();
    
    struct ListNode *windowStart = List_GetFirstNode(pointList);
    
    while (windowStart) {
        struct List *window = List_CreateSubSpan(pointList,
                                                 windowStart,
                                                 WINDOW_SIZE - 1,
                                                 true);
        
        if (List_GetSize(window) != WINDOW_SIZE) { goto skip_node; }
        
        struct ListNode *windowCenter = List_GetMiddleNode(window);
        
        // vom folosi timestamp-ul vechiului
        // mijloc al ferestrei pentru noua valoare
        unsigned timestamp =
            (*(struct Point **)ListNode_GetData(windowCenter))->timestamp;
        
        List_Sort(window, Point_List_comp_func); // sorteaza fereastra
        
        // copiaza nodul din mijlocul ferestrei in lista temporara
        struct ListNode *middle = List_AddNodeLast(newPointList, windowCenter);
        
        // seteaza timestamp-ul salvat mai sus pentru noua valoare
        (*(struct Point **)ListNode_GetData(middle))->timestamp = timestamp;
        
    skip_node:
        List_Destroy(window);
        
        windowStart = ListNode_GetNext(windowStart);
    }
    
    *out_print_func = Point_List_print_func;
    
    return newPointList;
}
