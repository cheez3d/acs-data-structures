#include "Data.h"
#include "List.h"
#include "Point.h"

#include <math.h>

#define WINDOW_SIZE 5 // 'k' din cerinta

_Static_assert(WINDOW_SIZE > 0, "WINDOW_SIZE cannot be nonpositive");

struct List * outlier_removal(struct List *pointList,
                              List_print_func_t *out_print_func) {
    // creeaza lista unde vor fi memorate nodurile ce vor fi sterse
    // dupa parcurgerea in totalitate a listei
    struct List *pendingRemoves = List_Create();
    
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
        
        // salveaza valoarea de referinta
        double value =
            (*(struct Point **)ListNode_GetData(windowCenter))->value;
        
        // calculeaza media aritmetica a ferestrei
        
        double avg = 0.0;
        
        struct ListNode *curr = windowStart;
        
        while (curr != ListNode_GetNext(windowEnd)) {
            avg += (*(struct Point **)ListNode_GetData(curr))->value;
            
            curr = ListNode_GetNext(curr);
        }
        
        avg /= windowSize; 
        
        // calculeaza deviata standard a ferestrei
        
        double sigma = 0.0;
        
        curr = windowStart;
        
        while (curr != ListNode_GetNext(windowEnd)) {
            sigma += pow(
                (*(struct Point **)ListNode_GetData(curr))->value - avg,
                2.0
            );
            
            curr = ListNode_GetNext(curr);
        }
        
        sigma /= windowSize;
        
        sigma = sqrt(sigma);
        
        // filtreaza valorile in functie de media aritmetica
        // si deviata standard; nodurile nu sunt sterse imediat,
        // ci sunt adaugate intr-o alta lista
        // care va fi parcursa la final pentru stergerea lor
        if ((value < (avg - sigma)) || (value > (avg + sigma))) {
            List_AddDataLast(pendingRemoves,
                             &(struct Data){
                                 &windowCenter,
                                 sizeof windowCenter
                             });
        }
        
    skip_node:
        windowStart = ListNode_GetNext(windowStart);
    }
    
    // sterge nodurile marcate pentru stergere din lista de puncte
    
    struct ListNode *curr = List_GetFirstNode(pendingRemoves);
    
    while (curr) {
        List_RemoveNode(
            pointList, **(struct ListNode ***)ListNode_GetData(curr),
            false
        );
        
        curr = ListNode_GetNext(curr);
    }
    
    // distruge lista temporara de noduri marcate pentru stergere
    List_Destroy(pendingRemoves);
    
    *out_print_func = Point_List_print_func;
    
    return pointList;
}
