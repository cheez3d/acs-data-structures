#include "Data.h"
#include "List.h"
#include "Point.h"

#define NDEBUG
#include <assert.h>

static const size_t WINDOW_SIZE = 5; // 'k' din cerinta

struct List * noise_removal_median(struct List *pointList,
                                   Data_print_func_t *out_print_func)
{
	assert(WINDOW_SIZE > 0);
	
	struct List *newPointList = List_Create();
	
	struct ListNode *windowStart = List_GetFirstNode(pointList);
	
	while (windowStart) {
		struct List *window = List_CreateSubFromSpan(pointList,
		                                             windowStart,
		                                             WINDOW_SIZE - 1,
		                                             true);
		
		if (List_GetSize(window) != WINDOW_SIZE) { goto skip_node; }
		
		struct ListNode *windowCenter = List_GetMiddleNode(window);
		
		// vom folosi timestamp-ul vechiului
		// mijloc al ferestrei pentru noua valoare
		unsigned timestamp =
			Point_Data_Unwrap(ListNode_GetData(windowCenter))->timestamp;
		
		List_Sort(window, Point_Data_comp_func_value); // sorteaza fereastra
		
		// copiaza nodul din mijlocul ferestrei in lista temporara
		struct ListNode *middle =
			List_AddNodeCopyLast(newPointList, windowCenter);
		
		// seteaza timestamp-ul salvat mai sus pentru noua valoare
		Point_Data_Unwrap(ListNode_GetData(middle))->timestamp = timestamp;
		
	skip_node:
		List_Destroy(window);
		
		windowStart = ListNode_GetNext(windowStart);
	}
	
	*out_print_func = Point_Data_print_func;
	
	return newPointList;
}
