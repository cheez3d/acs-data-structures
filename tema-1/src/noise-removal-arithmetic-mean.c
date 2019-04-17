#include "Data.h"
#include "List.h"
#include "Point.h"

#define NDEBUG
#include <assert.h>

static const size_t WINDOW_SIZE = 5; // 'k' din cerinta

struct List * noise_removal_arithmetic_mean(struct List *pointList,
                                            Data_print_func_t *out_print_func)
{
	assert(WINDOW_SIZE > 0);
	
	struct List *newPointList = List_Create();
	
	struct ListNode *windowStart = List_GetFirstNode(pointList);
	
	while (windowStart) {
		// obtine fereastra
		
		struct ListNode *windowEnd;
		size_t span;
		
		ListNode_GetSpanNode(windowStart,
		                     WINDOW_SIZE - 1,
		                     true,
		                     &windowEnd,
		                     &span);
		
		struct ListNode *windowCenter =
			List_GetNodesMiddle(pointList, windowStart, windowEnd);
		
		size_t windowSize = span + 1;
		
		if (windowSize != WINDOW_SIZE) { goto skip_node; }
		
		// vom folosi timestamp-ul vechiului
		// mijloc al ferestrei pentru noua valoare
		unsigned timestamp =
			Point_Data_Unwrap(ListNode_GetData(windowCenter))->timestamp;
		
		// calculeaza media aritmetica a ferestrei
		
		double avg = 0.0;
		
		struct ListNode *curr = windowStart;
		
		while (curr != ListNode_GetNext(windowEnd)) {
			avg += Point_Data_Unwrap(ListNode_GetData(curr))->value;
			
			curr = ListNode_GetNext(curr);
		}
		
		avg /= windowSize; 
		
		struct Point avgPoint = (struct Point){ timestamp, avg };
		
		const struct Data avgPointWrapper =
			(struct Data){ &avgPoint, sizeof avgPoint };
		
		// adauga noul punct in lista temporara
		List_AddDataLast(newPointList, &avgPointWrapper);
		
	skip_node:
		windowStart = ListNode_GetNext(windowStart);
	}
	
	*out_print_func = Point_Data_print_func;
	
	return newPointList;
}
