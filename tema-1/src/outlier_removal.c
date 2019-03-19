#include <math.h>
#include <stdio.h>

#include "LinkedList.h"
#include "Point.h"

const size_t K = 5;

float average(const struct LinkedList *pointList) {
    float avg = 0.f;
    
    const struct LinkedListNode *node = LinkedList_GetFirst(pointList);
    
    while (node) {
        const struct Point point = **(const struct Point **)LinkedListNode_GetData(node);
        
        avg += point.value;
        
        node = LinkedListNode_GetNext(node);
    }
    
    avg /= LinkedList_GetSize(pointList);
    
    return avg;
}

float standard_deviation(const struct LinkedList *pointList, float avg) {
    float sigma = 0.f;
    
    const struct LinkedListNode *node = LinkedList_GetFirst(pointList);
    
    while (node) {
        const struct Point point = **(const struct Point **)LinkedListNode_GetData(node);
        
        sigma += (point.value - avg) * (point.value - avg);
        
        node = LinkedListNode_GetNext(node);
    }
    
    sigma /= LinkedList_GetSize(pointList);
    
    sigma = sqrtf(sigma);
    
    return sigma;
}

void outlier_removal(struct LinkedList *pointList) {
    struct LinkedListNode *node = LinkedList_GetFirst(pointList);
    
    while (node) {
        struct LinkedList *window = LinkedList_GetSubCentered(pointList, node, K/2);
        
        struct LinkedListNode *nextNode = LinkedListNode_GetNext(node);
        
        if (window) {
            float value = (**(const struct Point **)LinkedListNode_GetData(node)).value;
            
            float avg = average(window);
            float sigma = standard_deviation(window, avg);
            
            if ((value < (avg - sigma)) || (value > (avg + sigma))) {
                LinkedList_Remove(pointList, node, false);
            }
        }
        
        node = nextNode;
        
        LinkedList_Destroy(window);
    }
}