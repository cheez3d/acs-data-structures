#include "Data.h"
#include "List.h"
#include "Point.h"

#include <math.h>

#define TOLERANCE 1000 // diferenta maxima dintre 2 valori temporale
                       // ce nu declanseaza procesul de completare
                       // cu date aditionale

#define FILLING_RATE 200 // din cate in cate millisecunde se adauga
                         // date noi

#define REACH 3 // 'k' din cerinta

_Static_assert(TOLERANCE > 0, "TOLERANCE cannot be nonpositive");
_Static_assert(FILLING_RATE > 0, "FILLING_RATE cannot be nonpositive");
_Static_assert(FILLING_RATE <= TOLERANCE, "FILLING_RATE cannot be greater than TOLERANCE");
_Static_assert(REACH > 0, "REACH cannot be nonpositive");

// functie pentru calcularea ponderilor
static inline double weight(size_t i, size_t k) {
    if (i >= k) { return 1.0; }
    
    double w = pow((double)i / (k - 1), 2.0) * 0.9 + 0.1;
    
    double sum = 0.0;
    
    for (size_t j = 0; j < k; ++j) {
        sum += pow((double)j / (k - 1), 2.0) * 0.9 + 0.1;
    }
    
    w /= sum;
    
    return w;
}

// functie pentru calcularea factorului de scalare 'C' din cerinta
static inline double scaling_factor(double timestamp,
                             double leftTimestamp,
                             double rightTimestamp) {
    if (leftTimestamp > rightTimestamp) {
        double temp = leftTimestamp;
        leftTimestamp = rightTimestamp;
        rightTimestamp = temp;
    }
    
    if (timestamp < leftTimestamp) { return 0.0; }
    if (timestamp > rightTimestamp) { return 1.0; }
    
    return (timestamp - leftTimestamp) / (rightTimestamp - leftTimestamp);
}

struct List * data_filling(struct List *pointList,
                           List_print_func_t *out_print_func) {
    // calculeaza ponderile
    
    double weights[REACH];
    
    for (size_t i = 0; i < REACH; ++i) { weights[i] = weight(i, REACH); }
    
    // parcurge lista de puncte
    
    struct ListNode *prev = List_GetFirstNode(pointList);
    struct ListNode *curr = ListNode_GetNext(prev);
    
    while (curr) {
        const struct Point *prevPoint =
            *(struct Point **)ListNode_GetData(prev);
        
        const struct Point *currPoint = 
            *(struct Point **)ListNode_GetData(curr);
        
        if (currPoint->timestamp - prevPoint->timestamp <= TOLERANCE) {
            goto skip_nodes;
        }
        
        // extrage valorile din stanga folosite pentru
        // calculul valorilor de completare
        
        double left[REACH];
        
        {
            struct ListNode *leftStart = prev;
            struct ListNode *leftEnd;
            
            size_t leftSpan;
            
            List_GetSpanNode(pointList,
                             leftStart,
                             REACH - 1,
                             false,
                             &leftEnd,
                             &leftSpan);
            
            if (leftSpan < REACH - 1) { goto skip_nodes; }
            
            struct ListNode *leftCurr = leftEnd;
            size_t i = 0;
            
            while (i < REACH) {
                left[i] =
                    (*(struct Point **)ListNode_GetData(leftCurr))->value;
                
                leftCurr = ListNode_GetNext(leftCurr);
                ++i;
            }
        }
        
        double right[REACH];
        
        // extrage valorile din dreapta folosite pentru
        // calculul valorilor de completare
        
        {
            struct ListNode *rightStart = curr;
            struct ListNode *rightEnd;
            
            size_t rightSpan;
            
            List_GetSpanNode(pointList,
                             rightStart,
                             REACH - 1,
                             true,
                             &rightEnd,
                             &rightSpan);
            
            if (rightSpan < REACH - 1) { goto skip_nodes; }
            
            struct ListNode *rightCurr = rightEnd;
            size_t i = 0;
            
            while (i < REACH) {
                right[i] =
                    (*(struct Point **)ListNode_GetData(rightCurr))->value;
                
                rightCurr = ListNode_GetPrev(rightCurr);
                ++i;
            }
        }
        
        // completeaza lista cu valori aditionale
        
        struct ListNode *lastFillingNode = prev;
        
        unsigned fillingTimestamp = prevPoint->timestamp + FILLING_RATE;
        
        while (fillingTimestamp < currPoint->timestamp) {
            double C = scaling_factor(fillingTimestamp,
                                      prevPoint->timestamp,
                                      currPoint->timestamp);
            
            double leftFilling = 0.0;
            
            for (size_t i = 0; i < REACH; ++i) {
                leftFilling += left[i] * weights[i];
            }
            
            leftFilling *= (1 - C);
            
            double rightFilling = 0.0;
            
            for (size_t i = 0; i < REACH; ++i) {
                rightFilling += right[i] * weights[i];
            }
            
            rightFilling *= C;
            
            double fillingValue = leftFilling + rightFilling;
            
            // creeaza punctul ce va fi adaugat in lista
            struct Point fillingPoint =
                (struct Point){ fillingTimestamp, fillingValue };
            
            // impacheteza punctul intr-un obiect Data pentru
            // a-l putea pasa functiei de adaugare in lista
            struct Data fillingPointContainer =
                (struct Data){ &fillingPoint, sizeof fillingPoint };
            
            lastFillingNode = List_AddDataAfter(pointList,
                                                &fillingPointContainer,
                                                lastFillingNode);
            
            fillingTimestamp += FILLING_RATE;
        }
        
        curr = lastFillingNode;
        
    skip_nodes:
        prev = curr;
        curr = ListNode_GetNext(curr);
    }
    
    *out_print_func = Point_List_print_func;
    
    return pointList;
}
