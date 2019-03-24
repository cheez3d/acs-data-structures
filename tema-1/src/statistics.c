#include "Data.h"
#include "Interval.h"
#include "List.h"
#include "Point.h"

#include <math.h>

struct List * statistics(struct List *pointList,
                         unsigned delta,
                         List_print_func_t *out_print_func) {
    struct List *intervalList = List_Create();
    
    struct ListNode *curr = List_GetFirstNode(pointList);
    
    while (curr) {
        // extrage valoarea din nod
        double value = (*(struct Point **)ListNode_GetData(curr))->value;
        
        // calculeaza capetele intervalului caruia valoarea apartine
        double intervalStart = floor(value / delta) * delta;
        double intervalEnd = (floor(value / delta) + 1.0 ) * delta;
        
        // creeaza un interval de referinta ce va fi folosit
        // pentru cautare in lista de intervale pentru a vedea
        // daca intervalul a mai inregistrat puncte inainte
        struct Interval interval =
            (struct Interval){ intervalStart, intervalEnd, 0 };
        
        // impacheteza intervalul intr-un obiect Data pentru
        // a-l putea pasa functiei de cautare in lista
        struct Data intervalContainer =
            (struct Data){ &interval, sizeof interval };
        
        // cauta intervalul in lista de intervale
        struct ListNode *intervalNode =
            List_ContainsData(intervalList,
                              &intervalContainer,
                              Interval_List_comp_func);
        
        // daca intervalul nu a mai fost intalnit pana acum,
        // adauga-l la lista
        if (!intervalNode) {
            intervalNode = List_AddDataLast(intervalList, &intervalContainer);
        }
        
        // incrementeaza numarul de puncte ce apartin intervalului
        ++(*(struct Interval **)ListNode_GetData(intervalNode))->pointCount;
        
        curr = ListNode_GetNext(curr);
    }
    
    // sorteaza lista de intervale
    List_Sort(intervalList, Interval_List_comp_func);
    
    *out_print_func = Interval_List_print_func;
    
    return intervalList;
}
